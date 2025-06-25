import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path
import numpy as np

# Configuração do diretório
output_dir = Path('/home/fares/projects/algorithms-benchmark/output')

# Lista para armazenar todos os DataFrames
dfs = []

# Mapeamento de tamanhos para valores numéricos
size_map = {
    '100k': 100000,
    '500k': 500000,
    '1000k': 1000000
}

# Mapeamento de casos de teste para ordem de exibição
test_case_order = {
    'random': 1,
    'nearly_sorted': 2,
    'reverse_sorted': 3
}

print("Arquivos encontrados:")
for file in output_dir.glob('selection_*_*_benchmark.csv'):
    print(f"- {file.name}")

print("\nProcessando arquivos...")
# Processar cada arquivo de seleção
for file in output_dir.glob('selection_*_*_benchmark.csv'):
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
            continue
            
        # Ler o arquivo CSV
        df = pd.read_csv(file)
        
        try:
            # Encontrar os nomes corretos das colunas
            select_linear_col = next(col for col in df.columns if 'Select' in col and 'Time' in col)
            quickselect_col = next(col for col in df.columns if 'Quick' in col and 'Time' in col)
            
            # Calcular médias
            mean_select_linear = df[select_linear_col].mean()
            mean_quickselect = df[quickselect_col].mean()
            
            print(f"Arquivo processado: {file.name} - Caso: {test_case}, Tamanho: {size}")
            print(f"  Médias - Select Linear: {mean_select_linear:.2f} ms, QuickSelect: {mean_quickselect:.2f} ms")
            
            # Adicionar ao DataFrame consolidado
            dfs.append({
                'Caso de Teste': test_case.replace('_', ' ').title(),
                'Ordem Caso de Teste': test_case_order[test_case],
                'Tamanho da Entrada': size,
                'Tamanho Numérico': size_map[size],
                'Select Linear (ms)': mean_select_linear,
                'QuickSelect (ms)': mean_quickselect
            })
            
        except (StopIteration, KeyError) as e:
            print(f"AVISO: Erro ao processar {file.name} - {str(e)}")
            continue
    except Exception as e:
        print(f"Erro ao processar arquivo {file.name}: {str(e)}")

# Criar DataFrame consolidado
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
    'Select Linear (ms)', 
    'QuickSelect (ms)'
]].copy()

# Arredondar valores para 2 casas decimais
display_df['Select Linear (ms)'] = display_df['Select Linear (ms)'].round(2)
display_df['QuickSelect (ms)'] = display_df['QuickSelect (ms)'].round(2)

# Configuração da figura para a tabela
plt.figure(figsize=(12, 8))  # Ajustado para caber mais linhas
plt.axis('off')  # Desativa os eixos

# Cria a tabela
col_labels = ['Caso de Teste', 'Tamanho', 'Select Linear (ms)', 'QuickSelect (ms)']
table_data = [col_labels] + display_df.values.tolist()

# Adiciona a tabela à figura
table = plt.table(
    cellText=table_data[1:],  # Dados sem o cabeçalho
    colLabels=table_data[0],  # Apenas o cabeçalho
    cellLoc='center',
    loc='center',
    colWidths=[0.25, 0.15, 0.3, 0.3]  # Larguras das colunas
)

# Estilização da tabela
table.auto_set_font_size(False)
table.set_fontsize(10)
table.scale(1.2, 1.8)  # Ajusta o tamanho das células

# Estilo do cabeçalho
for key, cell in table.get_celld().items():
    if key[0] == 0:  # Apenas a primeira linha (cabeçalho)
        cell.set_text_props(weight='bold')
    else:  # Remove negrito das outras células
        cell.set_text_props(weight='normal')

# Adiciona título
plt.title('Comparação de Desempenho: Select Linear vs QuickSelect\nTempos Médios de Execução (ms)', 
          pad=20, fontsize=14, fontweight='bold')

# Ajusta o layout
plt.tight_layout()

# Salva a tabela como imagem
output_path = output_dir / 'tabela_comparativa_algoritmos.png'
plt.savefig(output_path, dpi=300, bbox_inches='tight', pad_inches=0.5)
plt.close()

print(f"Tabela comparativa salva como: {output_path}")
print("\nDados da tabela:")
print(display_df.to_string(index=False, float_format='{:.2f}'.format))