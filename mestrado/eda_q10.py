#!/usr/bin/env python3
"""
Script de Análise Exploratória de Dados (EDA) para resultados da Questão 10
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from pathlib import Path

# Configuração de estilo
sns.set_style("whitegrid")
plt.rcParams['figure.figsize'] = (14, 8)

def load_data(csv_path):
    """Carrega e faz limpeza básica dos dados"""
    df = pd.read_csv(csv_path)
    print(f"✓ Dados carregados: {len(df)} linhas, {len(df.columns)} colunas\n")
    return df

def basic_stats(df):
    """Estatísticas básicas"""
    print("=" * 80)
    print("ESTATÍSTICAS BÁSICAS")
    print("=" * 80)
    
    print("\n1. Informações Gerais:")
    print(f"   - Tamanhos testados: {sorted(df['size'].unique())}")
    print(f"   - Algoritmos: {df['algorithm'].unique().tolist()}")
    print(f"   - Tipos de dados: {df['data_type'].unique().tolist()}")
    print(f"   - Número de runs: {df['run_index'].nunique()}")
    
    print("\n2. Estatísticas de Tempo (em segundos):")
    print(df.groupby('algorithm')['time_s'].describe())
    
    print("\n3. Tempo Médio por Algoritmo e Tipo de Dados:")
    pivot = df.groupby(['algorithm', 'data_type'])['time_s'].mean().unstack()
    print(pivot)
    
    print("\n4. Speedup Médio (não simultâneo / simultâneo):")
    for data_type in df['data_type'].unique():
        data = df[df['data_type'] == data_type]
        sep = data[data['algorithm'] == 'não simultaneo'].groupby('size')['time_s'].mean()
        sim = data[data['algorithm'] == 'simultaneo'].groupby('size')['time_s'].mean()
        speedup = (sep / sim).mean()
        print(f"   - {data_type:15s}: {speedup:.3f}x")

def outlier_analysis(df):
    """Análise de outliers"""
    print("\n" + "=" * 80)
    print("ANÁLISE DE OUTLIERS")
    print("=" * 80)
    
    for algo in df['algorithm'].unique():
        algo_data = df[df['algorithm'] == algo]['time_s']
        Q1 = algo_data.quantile(0.25)
        Q3 = algo_data.quantile(0.75)
        IQR = Q3 - Q1
        outliers = algo_data[(algo_data < Q1 - 3*IQR) | (algo_data > Q3 + 3*IQR)]
        print(f"\n   {algo}: {len(outliers)} outliers (3*IQR) de {len(algo_data)} pontos ({100*len(outliers)/len(algo_data):.1f}%)")

def performance_by_size(df):
    """Análise de desempenho por tamanho"""
    print("\n" + "=" * 80)
    print("DESEMPENHO POR TAMANHO (n)")
    print("=" * 80)
    
    sizes_of_interest = [1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000, 2050000000]
    
    for size in sizes_of_interest:
        if size not in df['size'].values:
            continue
            
        print(f"\n   n = {size:,}")
        size_data = df[(df['size'] == size) & (df['data_type'] == 'random')]
        
        for algo in ['não simultaneo', 'simultaneo']:
            algo_data = size_data[size_data['algorithm'] == algo]['time_s']
            if len(algo_data) > 0:
                print(f"      {algo:20s}: {algo_data.mean()*1000:8.3f} ms ± {algo_data.std()*1000:6.3f} ms")
        
        # Calcula speedup
        sep_mean = size_data[size_data['algorithm'] == 'não simultaneo']['time_s'].mean()
        sim_mean = size_data[size_data['algorithm'] == 'simultaneo']['time_s'].mean()
        if sim_mean > 0:
            speedup = sep_mean / sim_mean
            print(f"      {'Speedup':20s}: {speedup:.3f}x")

def distribution_comparison(df):
    """Comparação entre distribuições de dados"""
    print("\n" + "=" * 80)
    print("COMPARAÇÃO ENTRE DISTRIBUIÇÕES")
    print("=" * 80)
    
    # Usa o maior tamanho disponível
    max_size = df['size'].max()
    print(f"\n   Análise para n = {max_size:,}")
    
    data = df[df['size'] == max_size].groupby(['algorithm', 'data_type'])['time_s'].mean().unstack()
    print(data)
    
    print("\n   Speedup por distribuição (não simultâneo / simultâneo):")
    for data_type in df['data_type'].unique():
        sep = data.loc['não simultaneo', data_type]
        sim = data.loc['simultaneo', data_type]
        speedup = sep / sim
        print(f"      {data_type:15s}: {speedup:.3f}x")

def plot_summary(df, output_dir):
    """Gera gráficos de resumo"""
    print("\n" + "=" * 80)
    print("GERANDO GRÁFICOS DE RESUMO")
    print("=" * 80)
    
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # 1. Boxplot de tempos por algoritmo
    plt.figure(figsize=(12, 6))
    df_plot = df[df['algorithm'].isin(['não simultaneo', 'simultaneo'])]
    sns.boxplot(data=df_plot, x='algorithm', y='time_s', hue='data_type')
    plt.ylabel('Tempo (s)')
    plt.xlabel('Algoritmo')
    plt.title('Distribuição de Tempos por Algoritmo e Tipo de Dados')
    plt.yscale('log')
    plt.legend(title='Tipo de Dados', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    filename = output_dir / 'eda_boxplot_times.png'
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    print(f"   ✓ Salvo: {filename}")
    plt.close()
    
    # 2. Speedup por tamanho (todas as distribuições)
    plt.figure(figsize=(14, 8))
    for data_type in df['data_type'].unique():
        data = df[df['data_type'] == data_type]
        sep = data[data['algorithm'] == 'não simultaneo'].groupby('size')['time_s'].mean()
        sim = data[data['algorithm'] == 'simultaneo'].groupby('size')['time_s'].mean()
        speedup = sep / sim
        plt.plot(speedup.index, speedup.values, marker='o', label=data_type, linewidth=2)
    
    plt.axhline(y=1.0, color='red', linestyle='--', label='Sem speedup', linewidth=1)
    plt.xlabel('Tamanho do Array (n)')
    plt.ylabel('Speedup (não simultâneo / simultâneo)')
    plt.title('Speedup por Tamanho e Distribuição')
    plt.xscale('log')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    filename = output_dir / 'eda_speedup_all.png'
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    print(f"   ✓ Salvo: {filename}")
    plt.close()
    
    # 3. Heatmap de tempos médios
    plt.figure(figsize=(10, 6))
    # Seleciona alguns tamanhos representativos
    selected_sizes = [1000, 10000, 100000, 1000000, 10000000, 100000000]
    selected_sizes = [s for s in selected_sizes if s in df['size'].values]
    
    df_subset = df[df['size'].isin(selected_sizes)]
    pivot = df_subset.groupby(['size', 'algorithm'])['time_s'].mean().unstack()
    pivot = pivot[['não simultaneo', 'simultaneo']]
    
    sns.heatmap(pivot, annot=True, fmt='.4f', cmap='YlOrRd', cbar_kws={'label': 'Tempo (s)'})
    plt.ylabel('Tamanho (n)')
    plt.xlabel('Algoritmo')
    plt.title('Heatmap de Tempos Médios')
    plt.tight_layout()
    filename = output_dir / 'eda_heatmap_times.png'
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    print(f"   ✓ Salvo: {filename}")
    plt.close()
    
    # 4. Variabilidade (CV) por tamanho
    plt.figure(figsize=(12, 6))
    for algo in ['não simultaneo', 'simultaneo']:
        cv_data = df[df['algorithm'] == algo].groupby('size')['time_s'].agg(['mean', 'std'])
        cv = (cv_data['std'] / cv_data['mean']) * 100
        plt.plot(cv.index, cv.values, marker='o', label=algo, linewidth=2)
    
    plt.xlabel('Tamanho do Array (n)')
    plt.ylabel('Coeficiente de Variação (%)')
    plt.title('Variabilidade do Tempo de Execução por Tamanho')
    plt.xscale('log')
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    filename = output_dir / 'eda_variability.png'
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    print(f"   ✓ Salvo: {filename}")
    plt.close()

def main():
    csv_path = 'reports/q10_all.csv'
    output_dir = 'mestrado/eda_plots'
    
    print("\n" + "=" * 80)
    print("ANÁLISE EXPLORATÓRIA DE DADOS - QUESTÃO 10")
    print("=" * 80 + "\n")
    
    # Carrega dados
    df = load_data(csv_path)
    
    # Análises
    basic_stats(df)
    outlier_analysis(df)
    performance_by_size(df)
    distribution_comparison(df)
    plot_summary(df, output_dir)
    
    print("\n" + "=" * 80)
    print("ANÁLISE CONCLUÍDA!")
    print("=" * 80 + "\n")

if __name__ == '__main__':
    main()

