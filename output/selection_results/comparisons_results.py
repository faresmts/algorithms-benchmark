import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path
import numpy as np

# Diretório de saída
output_dir = Path(__file__).parent

# Mapeamento de tamanhos para ordenação
size_map = {
    '100k': 100000,
    '500k': 500000,
    '1000k': 1000000
}

# Ordem dos casos de teste
test_case_order = {
    'random': 1,
    'nearly_sorted': 2,
    'reverse_sorted': 3
}

# Lista para armazenar os DataFrames de cada arquivo
dfs = []

# Encontrar todos os arquivos CSV de benchmark
print("Arquivos encontrados:")
benchmark_files = list(output_dir.glob('selection_*_benchmark.csv'))
for file in benchmark_files:
    print(f"- {file.name}")

print("\nProcessando arquivos...")

# Processar cada arquivo
for file in benchmark_files:
    try:
        print(f"Processando: {file.name}")
        # Extrair informações do nome do arquivo
        parts = file.stem.split('_')
        if len(parts) < 4:  # Garante que temos pelo menos 4 partes
            continue
            
        # Extrair o caso de teste e o tamanho do nome do arquivo
        # O formato do nome do arquivo é: selection_<test_case>_<size>_benchmark.csv
        # Exemplo: selection_nearly_sorted_100k_benchmark.csv
        
        # O caso de teste pode conter underscores, então pegamos todas as partes exceto a primeira e as duas últimas
        test_case = '_'.join(parts[1:-2])
        
        # O tamanho é a penúltima parte
        size = parts[-2]
        
        # Pular se o tamanho não estiver no mapeamento
        if size not in size_map or test_case not in test_case_order:
            print(f"  Pular: {file.name} - Tamanho ou caso de teste inválido")
            continue
            
        # Ler o arquivo CSV
        df = pd.read_csv(file)
        
        try:
            # Encontrar as colunas de comparação
            select_linear_col = next(col for col in df.columns if 'Select Linear' in col and 'Comparisons' in col)
            quickselect_col = next(col for col in df.columns if 'QuickSelect' in col and 'Comparisons' in col)
            
            # Calcular médias
            mean_select_linear = df[select_linear_col].mean()
            mean_quickselect = df[quickselect_col].mean()
            
            print(f"Arquivo processado: {file.name} - Caso: {test_case}, Tamanho: {size}")
            print(f"  Médias - Select Linear: {mean_select_linear:,.0f} comparações, QuickSelect: {mean_quickselect:,.0f} comparações")
            
            # Adicionar ao DataFrame consolidado
            dfs.append({
                'Caso de Teste': test_case.replace('_', ' ').title(),
                'Ordem Caso de Teste': test_case_order[test_case],
                'Tamanho da Entrada': size,
                'Tamanho Numérico': size_map[size],
                'Select Linear (comparações)': mean_select_linear,
                'QuickSelect (comparações)': mean_quickselect
            })
            
        except (StopIteration, KeyError) as e:
            print(f"  AVISO: Erro ao processar {file.name} - {str(e)}")
            continue
            
    except Exception as e:
        print(f"Erro ao processar arquivo {file.name}: {str(e)}")

# Criar DataFrame com todos os resultados
if not dfs:
    print("Nenhum dado válido encontrado para gerar a tabela.")
    exit()

results_df = pd.DataFrame(dfs)

# Ordenar por caso de teste e tamanho numérico
results_df = results_df.sort_values(['Ordem Caso de Teste', 'Tamanho Numérico'])

# Formatar a tabela para exibição
display_df = results_df[[
    'Caso de Teste', 
    'Tamanho da Entrada', 
    'Select Linear (comparações)', 
    'QuickSelect (comparações)'
]].copy()

# Arredondar e formatar os valores numéricos
display_df['Select Linear (comparações)'] = display_df['Select Linear (comparações)'].round().astype(int).apply(lambda x: f"{x:,}")
display_df['QuickSelect (comparações)'] = display_df['QuickSelect (comparações)'].round().astype(int).apply(lambda x: f"{x:,}")

# Configuração da figura para a tabela
plt.figure(figsize=(14, 8))  # Ajustado para caber mais linhas e colunas mais largas
plt.axis('off')  # Desativa os eixos

# Cria a tabela sem cabeçalho
col_labels = ['Caso de Teste', 'Tamanho', 'Select Linear\n(comparações)', 'QuickSelect\n(comparações)']
table_data = display_df.values.tolist()

# Adiciona a tabela à figura
table = plt.table(
    cellText=table_data,  # Dados
    colLabels=col_labels,  # Cabeçalho
    cellLoc='center',
    loc='center',
    colWidths=[0.2, 0.1, 0.25, 0.25]  # Larguras das colunas ajustadas
)

# Estilização da tabela
table.auto_set_font_size(False)
table.set_fontsize(10)
table.scale(1.2, 1.8)  # Ajusta o tamanho das células

# Aplicar negrito apenas no cabeçalho
for (row, col), cell in table.get_celld().items():
    if row == 0:  # Apenas a primeira linha (cabeçalho)
        cell.set_text_props(weight='bold')
    else:
        cell.set_text_props(weight='normal')

# Adiciona título
plt.title('Comparação de Número de Comparações\nSelect Linear vs QuickSelect', 
          pad=20, fontsize=14, fontweight='bold')

# Ajusta o layout
plt.tight_layout()

# Salva a tabela como imagem
output_path = output_dir / 'tabela_comparativa_comparacoes.png'
plt.savefig(output_path, dpi=300, bbox_inches='tight', pad_inches=0.5)
plt.close()

print(f"\nTabela comparativa de comparações salva como: {output_path}")
print("\nDados da tabela de comparações:")
print(display_df.to_string(index=False))
