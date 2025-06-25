import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path
import numpy as np

# Output directory
output_dir = Path(__file__).parent

# Size mapping for selection
size_map = {
    '100k': 100000,
    '500k': 500000,
    '1000k': 1000000
}

# Test case order
test_case_order = {
    'random': 1,
    'nearly_sorted': 2,
    'reverse_sorted': 3
}

def load_and_process_data():
    """Load and process all selection benchmark files."""
    dfs = []
    print("Arquivos encontrados:")
    
    # Find all selection benchmark files
    benchmark_files = list(output_dir.glob('selection_*_benchmark.csv'))
    for file in benchmark_files:
        print(f"- {file.name}")
    
    print("\nProcessando arquivos...")
    
    # Process each file
    for file in benchmark_files:
        try:
            print(f"Processando: {file.name}")
            # Extract information from filename
            parts = file.stem.split('_')
            if len(parts) < 4:  # Ensure we have at least 4 parts
                continue
                
            # Extract test case and size from filename
            # Format: selection_<test_case>_<size>_benchmark.csv
            test_case = '_'.join(parts[1:-2])  # All parts except first and last two
            size = parts[-2]  # Size is the second to last part
            
            # Skip if size is not in the mapping
            if size not in size_map or test_case not in test_case_order:
                print(f"  Pular: {file.name} - Tamanho ou caso de teste inválido")
                continue
                
            # Read CSV file
            df = pd.read_csv(file)
            
            # Add test case and size columns
            df['test_case'] = test_case
            df['size'] = size
            df['size_numeric'] = size_map[size]
            df['test_case_order'] = test_case_order[test_case]
            
            dfs.append(df)
            print(f"  Sucesso: {len(df)} linhas processadas")
            
        except Exception as e:
            print(f"  Erro ao processar {file.name}: {str(e)}")
    
    if not dfs:
        print("Nenhum arquivo válido encontrado.")
        return None
    
    # Combine all DataFrames
    combined_df = pd.concat(dfs, ignore_index=True)
    
    # Sort by test case order and size
    combined_df = combined_df.sort_values(['test_case_order', 'size_numeric'])
    
    return combined_df

def analyze_comparisons(df):
    """Analyze and plot comparison counts."""
    if df is None:
        return
        
    print("\nAnalisando número de comparações...")
    
    # Get all comparison columns
    comparison_columns = [col for col in df.columns if 'Comparisons' in col]
    
    # Melt the dataframe to have algorithm as a column
    melted_comparisons = pd.melt(
        df,
        id_vars=['test_case', 'size', 'size_numeric', 'test_case_order'],
        value_vars=comparison_columns,
        var_name='algorithm',
        value_name='comparison_count'
    )
    
    # Extract algorithm name from column (e.g., 'Comparisons Select Linear' -> 'Select Linear')
    melted_comparisons['algorithm'] = melted_comparisons['algorithm'].str.replace('Comparisons ', '', regex=False)
    
    # Convert to pivot table
    comparison_pivot = melted_comparisons.pivot_table(
        index=['test_case', 'size'],
        columns='algorithm',
        values='comparison_count',
        aggfunc='mean'
    ).reset_index()
    
    # Sort by test case order and size
    comparison_pivot['test_case_order'] = comparison_pivot['test_case'].map(test_case_order)
    comparison_pivot['size_numeric'] = comparison_pivot['size'].map(size_map)
    comparison_pivot = comparison_pivot.sort_values(['test_case_order', 'size_numeric'])
    comparison_pivot = comparison_pivot.drop(columns=['test_case_order', 'size_numeric'])
    
    # Save to CSV
    comparison_csv_path = output_dir / 'selection_comparison_counts_summary.csv'
    comparison_pivot.to_csv(comparison_csv_path, index=False)
    print(f"Resumo de comparações salvo em: {comparison_csv_path}")
    
    # Plot comparisons by algorithm and test case
    for test_case in test_case_order:
        test_case_mask = melted_comparisons['test_case'] == test_case
        if not test_case_mask.any():
            continue
            
        plt.figure(figsize=(12, 6))
        for algorithm in melted_comparisons['algorithm'].unique():
            algo_mask = (melted_comparisons['algorithm'] == algorithm) & test_case_mask
            algo_df = melted_comparisons[algo_mask]
            
            # Plot comparison counts
            plt.plot(algo_df['size'], algo_df['comparison_count'], 
                    marker='o', label=algorithm)
        
        plt.title(f'Número de Comparações - {test_case.replace("_", " ").title()}')
        plt.xlabel('Tamanho do Conjunto de Dados')
        plt.ylabel('Número de Comparações')
        plt.xticks(rotation=45)
        plt.legend()
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.tight_layout()
        
        # Save plot
        plot_path = output_dir / f'selection_comparisons_{test_case}.png'
        plt.savefig(plot_path, dpi=300, bbox_inches='tight')
        plt.close()
        print(f"  Gráfico salvo em: {plot_path}")

def analyze_execution_time(df):
    """Analyze and plot execution time results."""
    if df is None:
        return
        
    print("\nAnalisando tempos de execução...")
    
    # Get all execution time columns
    time_columns = [col for col in df.columns if 'Execution Time' in col]
    
    # Melt the dataframe to have algorithm as a column
    melted_df = pd.melt(
        df,
        id_vars=['test_case', 'size', 'size_numeric', 'test_case_order'],
        value_vars=time_columns,
        var_name='algorithm',
        value_name='execution_time_ms'
    )
    
    # Extract algorithm name from column (e.g., 'Execution Time (ms) Select Linear' -> 'Select Linear')
    melted_df['algorithm'] = melted_df['algorithm'].str.replace(r'Execution Time \(ms\) ', '', regex=True)
    
    # Convert to pivot table
    time_pivot = melted_df.pivot_table(
        index=['test_case', 'size'],
        columns='algorithm',
        values='execution_time_ms',
        aggfunc='mean'
    ).reset_index()
    
    # Convert milliseconds to seconds
    for col in time_pivot.columns:
        if col not in ['test_case', 'size']:
            time_pivot[col] = time_pivot[col] / 1000  # Convert ms to s
    
    # Sort by test case order and size
    time_pivot['test_case_order'] = time_pivot['test_case'].map(test_case_order)
    time_pivot['size_numeric'] = time_pivot['size'].map(size_map)
    time_pivot = time_pivot.sort_values(['test_case_order', 'size_numeric'])
    time_pivot = time_pivot.drop(columns=['test_case_order', 'size_numeric'])
    
    # Save to CSV
    time_csv_path = output_dir / 'selection_execution_time_summary.csv'
    time_pivot.to_csv(time_csv_path, index=False)
    print(f"Resumo de tempos de execução salvo em: {time_csv_path}")
    
    # Plot execution time by algorithm and test case
    for test_case in test_case_order:
        test_case_mask = melted_df['test_case'] == test_case
        if not test_case_mask.any():
            continue
            
        plt.figure(figsize=(12, 6))
        for algorithm in melted_df['algorithm'].unique():
            algo_mask = (melted_df['algorithm'] == algorithm) & test_case_mask
            algo_df = melted_df[algo_mask]
            
            # Convert to seconds and plot
            plt.plot(algo_df['size'], algo_df['execution_time_ms'] / 1000, 
                    marker='o', label=algorithm)
        
        plt.title(f'Tempo de Execução - {test_case.replace("_", " ").title()}')
        plt.xlabel('Tamanho do Conjunto de Dados')
        plt.ylabel('Tempo (segundos)')
        plt.xticks(rotation=45)
        plt.legend()
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.tight_layout()
        
        # Save plot
        plot_path = output_dir / f'selection_execution_time_{test_case}.png'
        plt.savefig(plot_path, dpi=300, bbox_inches='tight')
        plt.close()
        print(f"  Gráfico salvo em: {plot_path}")

def analyze_memory_usage(df):
    """Analyze and plot memory usage results."""
    if df is None:
        return
        
    print("\nAnalisando uso de memória...")
    
    # Get all memory usage columns
    memory_columns = [col for col in df.columns if 'Memory Usage' in col]
    
    # Melt the dataframe to have algorithm as a column
    melted_memory = pd.melt(
        df,
        id_vars=['test_case', 'size', 'size_numeric', 'test_case_order'],
        value_vars=memory_columns,
        var_name='algorithm',
        value_name='memory_usage_bytes'
    )
    
    # Extract algorithm name from column
    melted_memory['algorithm'] = melted_memory['algorithm'].str.replace(r'Memory Usage \(bytes\) ', '', regex=True)
    
    # Convert bytes to MB
    melted_memory['memory_usage_mb'] = melted_memory['memory_usage_bytes'] / (1024 * 1024)
    
    # Convert to pivot table
    memory_pivot = melted_memory.pivot_table(
        index=['test_case', 'size'],
        columns='algorithm',
        values='memory_usage_mb',
        aggfunc='mean'
    ).reset_index()
    
    # Sort by test case order and size
    memory_pivot['test_case_order'] = memory_pivot['test_case'].map(test_case_order)
    memory_pivot['size_numeric'] = memory_pivot['size'].map(size_map)
    memory_pivot = memory_pivot.sort_values(['test_case_order', 'size_numeric'])
    memory_pivot = memory_pivot.drop(columns=['test_case_order', 'size_numeric'])
    
    # Save to CSV
    memory_csv_path = output_dir / 'selection_memory_usage_summary.csv'
    memory_pivot.to_csv(memory_csv_path, index=False)
    print(f"Resumo de uso de memória salvo em: {memory_csv_path}")
    
    # Plot memory usage by algorithm and test case
    for test_case in test_case_order:
        test_case_mask = melted_memory['test_case'] == test_case
        if not test_case_mask.any():
            continue
            
        plt.figure(figsize=(12, 6))
        for algorithm in melted_memory['algorithm'].unique():
            algo_mask = (melted_memory['algorithm'] == algorithm) & test_case_mask
            algo_df = melted_memory[algo_mask]
            
            # Convert to MB and plot
            plt.plot(algo_df['size'], algo_df['memory_usage_bytes'] / (1024 * 1024), 
                    marker='o', label=algorithm)
        
        plt.title(f'Uso de Memória - {test_case.replace("_", " ").title()}')
        plt.xlabel('Tamanho do Conjunto de Dados')
        plt.ylabel('Uso de Memória (MB)')
        plt.xticks(rotation=45)
        plt.legend()
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.tight_layout()
        
        # Save plot
        plot_path = output_dir / f'selection_memory_usage_{test_case}.png'
        plt.savefig(plot_path, dpi=300, bbox_inches='tight')
        plt.close()
        print(f"  Gráfico salvo em: {plot_path}")

def create_comparison_table(df):
    """Create a comparison table of all algorithms."""
    if df is None:
        return
        
    print("\nCriando tabela comparativa...")
    
    # Get all time, memory, and comparison columns
    time_columns = [col for col in df.columns if 'Execution Time' in col]
    memory_columns = [col for col in df.columns if 'Memory Usage' in col]
    comparison_columns = [col for col in df.columns if 'Comparisons' in col]  # Add comparison columns
    
    # Create a list to store all the data
    all_data = []
    
    # Process each row
    for _, row in df.iterrows():
        for time_col, mem_col, comp_col in zip(time_columns, memory_columns, comparison_columns):
            algorithm = time_col.replace('Execution Time (ms) ', '')
            all_data.append({
                'test_case': row['test_case'],
                'size': row['size'],
                'algorithm': algorithm,
                'execution_time_seconds': row[time_col] / 1000,  # Convert ms to s
                'memory_usage_mb': row[mem_col] / (1024 * 1024),  # Convert bytes to MB
                'comparison_count': row[comp_col]  # Add comparison count
            })
    
    # Create a new DataFrame
    comparison_df = pd.DataFrame(all_data)
    
    # Create a pivot table with mean values
    pivot = comparison_df.pivot_table(
        index=['test_case', 'size'],
        columns='algorithm',
        aggfunc={
            'execution_time_seconds': 'mean',
            'memory_usage_mb': 'mean',
            'comparison_count': 'mean'  # Add comparison count to aggregation
        }
    ).round(4)
    
    # Flatten multi-index columns
    pivot.columns = [f"{col[1]}_{col[0]}" for col in pivot.columns]
    pivot = pivot.reset_index()
    
    # Sort by test case order and size
    pivot['test_case_order'] = pivot['test_case'].map(test_case_order)
    pivot['size_numeric'] = pivot['size'].map(size_map)
    pivot = pivot.sort_values(['test_case_order', 'size_numeric'])
    pivot = pivot.drop(columns=['test_case_order', 'size_numeric'])
    
    # Save to CSV
    csv_path = output_dir / 'selection_comparison_table.csv'
    pivot.to_csv(csv_path, index=False)
    print(f"Tabela comparativa salva em: {csv_path}")
    
    # Create a formatted table for visualization
    plt.figure(figsize=(14, 8))
    ax = plt.subplot(111, frame_on=False)
    ax.xaxis.set_visible(False)
    ax.yaxis.set_visible(False)
    
    # Prepare data for table
    table_data = pivot.copy()
    
    # Format column names for better display
    columns = ['Caso de Teste', 'Tamanho']
    for col in table_data.columns[2:]:  # Skip test_case and size
        if 'time' in col.lower():
            algo = col.split('_')[0]
            columns.append(f"{algo}\nTempo (s)")
        elif 'memory' in col.lower():
            algo = col.split('_')[0]
            columns.append(f"{algo}\nMemória (MB)")
        elif 'comparison' in col.lower():
            algo = col.split('_')[0]
            columns.append(f"{algo}\nComparações")
    
    # Create table
    table = plt.table(
        cellText=table_data.values,
        colLabels=columns,
        cellLoc='center',
        loc='center',
        bbox=[0, 0, 1, 1]  # x, y, width, height
    )
    
    # Style table
    table.auto_set_font_size(False)
    table.set_fontsize(8)
    table.scale(1.2, 1.5)
    
    # Make header row bold
    for (row, col), cell in table.get_celld().items():
        if row == 0:  # Header row
            cell.set_text_props(weight='bold')
    
    # Save table as image
    table_path = output_dir / 'selection_comparison_table.png'
    plt.savefig(table_path, dpi=300, bbox_inches='tight', pad_inches=0.5)
    plt.close()
    print(f"Tabela comparativa salva como imagem em: {table_path}")

def main():
    """Main function to run the analysis."""
    print("="*80)
    print("ANÁLISE DE ALGORITMOS DE SELEÇÃO")
    print("="*80 + "\n")
    
    # Load and process data
    df = load_and_process_data()
    
    if df is not None:
        # Perform analyses
        analyze_execution_time(df)
        analyze_memory_usage(df)
        analyze_comparisons(df)
        create_comparison_table(df)
        
        print("\n" + "="*80)
        print("ANÁLISE CONCLUÍDA COM SUCESSO!")
        print("="*80)
    else:
        print("\nNenhum dado válido para análise.")

if __name__ == "__main__":
    main()