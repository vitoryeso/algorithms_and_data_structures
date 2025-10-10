#!/usr/bin/env python3
"""
Script para gerar gráficos dos benchmarks das questões.
"""

import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
import seaborn as sns
import sys
import os
from pathlib import Path
import re

# Configuração de estilo
sns.set_style("whitegrid")
plt.rcParams['figure.figsize'] = (12, 8)
plt.rcParams['font.size'] = 11

def parse_metadata(metadata_str):
    """Extrai métricas do campo metadata"""
    if pd.isna(metadata_str) or metadata_str == '':
        return {}
    
    metrics = {}
    for pair in metadata_str.split(';'):
        if '=' in pair:
            key, value = pair.split('=', 1)
            try:
                metrics[key] = float(value)
            except:
                metrics[key] = value
    return metrics

def plot_q10_minmax(df, output_dir):
    """Plota resultados da Questão 10 (Min/Max Simultâneo)"""
    print("Gerando gráficos da Questão 10...")
    
    # Remove outliers simples (3*IQR) por grupo de (algorithm,data_type)
    # Mantém robustez dos gráficos
    def remove_outliers_iqr(df_in):
        def filt(g):
            q1 = g['time_s'].quantile(0.25)
            q3 = g['time_s'].quantile(0.75)
            iqr = q3 - q1
            lo = q1 - 3*iqr
            hi = q3 + 3*iqr
            return g[(g['time_s'] >= lo) and (g['time_s'] <= hi)]
        return df_in.groupby(['algorithm','data_type'], group_keys=False).apply(lambda g: g if len(g) < 5 else g[(g['time_s'] >= g['time_s'].quantile(0.25) - 3*(g['time_s'].quantile(0.75)-g['time_s'].quantile(0.25))) & (g['time_s'] <= g['time_s'].quantile(0.75) + 3*(g['time_s'].quantile(0.75)-g['time_s'].quantile(0.25)))])

    df = remove_outliers_iqr(df)

    # Agrupa por tamanho, algoritmo e tipo de dados
    grouped = df.groupby(['size', 'algorithm', 'data_type']).agg({
        'time_s': ['mean', 'std'],
        'comparisons': 'mean'
    }).reset_index()
    
    grouped.columns = ['size', 'algorithm', 'data_type', 'time_mean', 'time_std', 'comparisons']
    
    # Gráfico 1: Tempo vs Tamanho (por tipo de dados)
    for data_type in grouped['data_type'].unique():
        plt.figure(figsize=(12, 8))
        
        data = grouped[grouped['data_type'] == data_type]
        
        # Exibir somente algorithms principais: não simultaneo e simultaneo
        for algo in [a for a in data['algorithm'].unique() if a in ['não simultaneo','simultaneo']]:
            algo_data = data[data['algorithm'] == algo]
            plt.plot(algo_data['size'], algo_data['time_mean'] * 1000,
                     marker='o', label=algo, linewidth=2)
            plt.fill_between(algo_data['size'], 
                           (algo_data['time_mean'] - algo_data['time_std']) * 1000,
                           (algo_data['time_mean'] + algo_data['time_std']) * 1000,
                           alpha=0.2)
        
        plt.xlabel('Tamanho do Array (n)', fontsize=12)
        plt.ylabel('Tempo (ms)', fontsize=12)
        plt.title(f'Q10: Min/Max - Tempo vs Tamanho\nDistribuição: {data_type}', fontsize=14, fontweight='bold')
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        
        ax = plt.gca()
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
        filename = output_dir / f'q10_time_{data_type}.png'
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"  Salvo: {filename}")
        plt.close()
    
    # Gráfico 2: Comparações vs Tamanho (gera somente se houver valores > 0)
    if 'comparisons' in grouped.columns and (grouped['comparisons'] > 0).any():
        plt.figure(figsize=(12, 8))
        for algo in grouped['algorithm'].unique():
            algo_data = grouped[grouped['algorithm'] == algo].groupby('size')['comparisons'].mean().reset_index()
            plt.plot(algo_data['size'], algo_data['comparisons'], marker='o', label=algo, linewidth=2)
        plt.xlabel('Tamanho do Array (n)', fontsize=12)
        plt.ylabel('Número de Comparações', fontsize=12)
        plt.title('Q10: Número de Comparações vs Tamanho', fontsize=14, fontweight='bold')
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        filename = output_dir / 'q10_comparisons.png'
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"  Salvo: {filename}")
        plt.close()
    
    # Gráfico 3: Speedup (múltiplas variantes)
    # Para cada data_type, criaremos um gráfico com speedup de diferentes variantes vs 'separate'
    algo_variants = [
        ('simultaneo', 'Simultaneo'),
    ]

    for data_type in grouped['data_type'].unique():
        data = grouped[grouped['data_type'] == data_type]
        sep_data = data[data['algorithm'] == 'não simultaneo'].set_index('size')['time_mean']
        if sep_data.empty:
            continue

        plt.figure(figsize=(12, 8))
        for key, label in algo_variants:
            sim_series = data[data['algorithm'] == key].set_index('size')['time_mean']
            if sim_series.empty:
                continue
            joined = sep_data.to_frame('sep').join(sim_series.to_frame('sim'), how='inner')
            if joined.empty:
                continue
            speedup = joined['sep'] / joined['sim']
            plt.plot(speedup.index, speedup.values, marker='o', linewidth=2, label=label)

        plt.axhline(y=1.0, color='red', linestyle='--', label='Sem speedup', linewidth=1)
        plt.xlabel('Tamanho do Array (n)', fontsize=12)
        plt.ylabel('Speedup (Separate / Variante)', fontsize=12)
        plt.title(f'Q10: Speedup por variante\nDistribuição: {data_type}', fontsize=14, fontweight='bold')
        plt.legend(fontsize=11)
        plt.grid(True, alpha=0.3)
        plt.tight_layout()
        ax = plt.gca()
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
        filename = output_dir / f'q10_speedup_{data_type}.png'
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"  Salvo: {filename}")
        plt.close()

def plot_q14_hash_tables(df, output_dir):
    """Plota resultados da Questão 14 (Tabelas Hash)"""
    print("Gerando gráficos da Questão 14...")
    
    # Parse metadata
    metadata_df = df['metadata'].apply(parse_metadata).apply(pd.Series)
    df = pd.concat([df, metadata_df], axis=1)
    
    # Remove outliers
    def remove_outliers_iqr(df_in):
        return df_in.groupby(['algorithm','data_type'], group_keys=False).apply(
            lambda g: g if len(g) < 5 else g[
                (g['time_s'] >= g['time_s'].quantile(0.25) - 3*(g['time_s'].quantile(0.75)-g['time_s'].quantile(0.25))) & 
                (g['time_s'] <= g['time_s'].quantile(0.75) + 3*(g['time_s'].quantile(0.75)-g['time_s'].quantile(0.25)))
            ]
        )
    
    df = remove_outliers_iqr(df)
    
    # Agrupa por tamanho, algoritmo e operação
    agg_dict = {
        'time_s': ['mean', 'std', 'min', 'max']
    }
    
    # Adiciona métricas estruturais se disponíveis
    if 'load_factor' in df.columns:
        agg_dict['load_factor'] = 'mean'
    if 'collisions' in df.columns:
        agg_dict['collisions'] = 'mean'
    if 'max_chain' in df.columns:
        agg_dict['max_chain'] = 'mean'
    if 'total_probes' in df.columns:
        agg_dict['total_probes'] = 'mean'
    if 'max_cluster' in df.columns:
        agg_dict['max_cluster'] = 'mean'
    
    grouped = df.groupby(['size', 'algorithm', 'data_type']).agg(agg_dict).reset_index()
    
    # Flatten column names
    grouped.columns = ['_'.join(col).strip('_') if col[1] else col[0] 
                      for col in grouped.columns.values]
    
    # Dicionário de nomes bonitos
    algo_names = {
        'chaining': 'Encadeamento',
        'open_addressing': 'Endereçamento Aberto'
    }
    
    operation_names = {
        'insert': 'Inserção',
        'search': 'Busca (Hit)',
        'search_miss': 'Busca (Miss)',
        'remove': 'Remoção'
    }
    
    colors = {
        'chaining': '#2E86AB',
        'open_addressing': '#A23B72'
    }
    
    # Gráfico 1: Tempo por operação (4 subplots, um para cada operação)
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    axes = axes.flatten()
    
    for idx, operation in enumerate(['insert', 'search', 'search_miss', 'remove']):
        ax = axes[idx]
        data = grouped[grouped['data_type'] == operation]
        
        for algo in ['chaining', 'open_addressing']:
            algo_data = data[data['algorithm'] == algo]
            if algo_data.empty:
                continue
            
            ax.plot(algo_data['size'], algo_data['time_s_mean'] * 1000,
                   marker='o', label=algo_names[algo], linewidth=2.5, 
                   color=colors[algo], markersize=6)
            ax.fill_between(algo_data['size'],
                          (algo_data['time_s_mean'] - algo_data['time_s_std']) * 1000,
                          (algo_data['time_s_mean'] + algo_data['time_s_std']) * 1000,
                          alpha=0.2, color=colors[algo])
        
        ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
        ax.set_ylabel('Tempo (ms)', fontsize=11, fontweight='bold')
        ax.set_title(f'{operation_names[operation]}', fontsize=12, fontweight='bold')
        ax.legend(fontsize=10, loc='upper left')
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    plt.suptitle('Q14: Tabelas Hash - Tempo de Execução por Operação', 
                 fontsize=16, fontweight='bold', y=0.995)
    plt.tight_layout()
    filename = output_dir / 'q14_operations_time.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()
    
    # Gráfico 2: Throughput (operações por segundo)
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    axes = axes.flatten()
    
    for idx, operation in enumerate(['insert', 'search', 'search_miss', 'remove']):
        ax = axes[idx]
        data = grouped[grouped['data_type'] == operation].copy()
        data['throughput'] = data['size'] / data['time_s_mean']
        
        for algo in ['chaining', 'open_addressing']:
            algo_data = data[data['algorithm'] == algo]
            if algo_data.empty:
                continue
            
            ax.plot(algo_data['size'], algo_data['throughput'] / 1e6,
                   marker='o', label=algo_names[algo], linewidth=2.5,
                   color=colors[algo], markersize=6)
        
        ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
        ax.set_ylabel('Throughput (M ops/s)', fontsize=11, fontweight='bold')
        ax.set_title(f'{operation_names[operation]}', fontsize=12, fontweight='bold')
        ax.legend(fontsize=10, loc='best')
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    plt.suptitle('Q14: Tabelas Hash - Throughput por Operação', 
                 fontsize=16, fontweight='bold', y=0.995)
    plt.tight_layout()
    filename = output_dir / 'q14_operations_throughput.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()
    
    # Gráfico 3: Razão de performance (Chaining / Open Addressing)
    fig, ax = plt.subplots(figsize=(14, 8))
    
    for operation in ['insert', 'search', 'search_miss', 'remove']:
        data = grouped[grouped['data_type'] == operation]
        
        chain_data = data[data['algorithm'] == 'chaining'].set_index('size')['time_s_mean']
        open_data = data[data['algorithm'] == 'open_addressing'].set_index('size')['time_s_mean']
        
        if chain_data.empty or open_data.empty:
            continue
        
        joined = chain_data.to_frame('chain').join(open_data.to_frame('open'), how='inner')
        if joined.empty:
            continue
        
        ratio = joined['chain'] / joined['open']
        ax.plot(ratio.index, ratio.values, marker='o', linewidth=2.5, 
               label=operation_names[operation], markersize=7)
    
    ax.axhline(y=1.0, color='red', linestyle='--', linewidth=2, 
              label='Performance Igual', alpha=0.7)
    ax.set_xlabel('Número de Elementos (n)', fontsize=12, fontweight='bold')
    ax.set_ylabel('Razão (Tempo Encadeamento / Tempo End. Aberto)', fontsize=12, fontweight='bold')
    ax.set_title('Q14: Razão de Performance Relativa\n(valores > 1: End. Aberto é mais rápido)', 
                fontsize=14, fontweight='bold')
    ax.legend(fontsize=11, loc='best')
    ax.grid(True, alpha=0.3, linestyle='--')
    ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    plt.tight_layout()
    filename = output_dir / 'q14_performance_ratio.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()
    
    # Gráfico 4: Comparação direta - Barras agrupadas
    # Calcula médias gerais por operação
    summary = grouped.groupby(['algorithm', 'data_type'])['time_s_mean'].mean().reset_index()
    
    operations = ['insert', 'search', 'search_miss', 'remove']
    x = range(len(operations))
    width = 0.35
    
    fig, ax = plt.subplots(figsize=(12, 8))
    
    chain_times = [summary[(summary['algorithm'] == 'chaining') & 
                           (summary['data_type'] == op)]['time_s_mean'].values[0] * 1000 
                   if not summary[(summary['algorithm'] == 'chaining') & 
                                 (summary['data_type'] == op)].empty else 0
                   for op in operations]
    
    open_times = [summary[(summary['algorithm'] == 'open_addressing') & 
                          (summary['data_type'] == op)]['time_s_mean'].values[0] * 1000
                  if not summary[(summary['algorithm'] == 'open_addressing') & 
                                (summary['data_type'] == op)].empty else 0
                  for op in operations]
    
    bars1 = ax.bar([i - width/2 for i in x], chain_times, width, 
                   label='Encadeamento', color=colors['chaining'], alpha=0.8)
    bars2 = ax.bar([i + width/2 for i in x], open_times, width,
                   label='Endereçamento Aberto', color=colors['open_addressing'], alpha=0.8)
    
    ax.set_xlabel('Operação', fontsize=12, fontweight='bold')
    ax.set_ylabel('Tempo Médio (ms)', fontsize=12, fontweight='bold')
    ax.set_title('Q14: Comparação de Performance Média por Operação', 
                fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels([operation_names[op] for op in operations])
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3, axis='y', linestyle='--')
    
    # Adiciona valores nas barras
    def autolabel(bars):
        for bar in bars:
            height = bar.get_height()
            if height > 0:
                ax.annotate(f'{height:.2f}',
                           xy=(bar.get_x() + bar.get_width() / 2, height),
                           xytext=(0, 3),
                           textcoords="offset points",
                           ha='center', va='bottom', fontsize=9)
    
    autolabel(bars1)
    autolabel(bars2)
    
    plt.tight_layout()
    filename = output_dir / 'q14_comparison_bars.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()
    
    # Gráfico 5: Métricas Estruturais - Clustering e Colisões
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    
    # Subplot 1: Max Chain Length (Chaining)
    ax = axes[0, 0]
    if 'max_chain_mean' in grouped.columns:
        data = grouped[(grouped['algorithm'] == 'chaining') & 
                      (grouped['data_type'] == 'insert')]
        if not data.empty:
            ax.plot(data['size'], data['max_chain_mean'], marker='o', 
                   linewidth=2.5, color=colors['chaining'], markersize=6)
            ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
            ax.set_ylabel('Comprimento Máximo da Cadeia', fontsize=11, fontweight='bold')
            ax.set_title('Encadeamento: Maior Cadeia', fontsize=12, fontweight='bold')
            ax.grid(True, alpha=0.3, linestyle='--')
            ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    # Subplot 2: Collisions (Chaining)
    ax = axes[0, 1]
    if 'collisions_mean' in grouped.columns:
        data = grouped[(grouped['algorithm'] == 'chaining') & 
                      (grouped['data_type'] == 'insert')]
        if not data.empty:
            ax.plot(data['size'], data['collisions_mean'], marker='o', 
                   linewidth=2.5, color=colors['chaining'], markersize=6)
            ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
            ax.set_ylabel('Número de Posições com Colisões', fontsize=11, fontweight='bold')
            ax.set_title('Encadeamento: Colisões', fontsize=12, fontweight='bold')
            ax.grid(True, alpha=0.3, linestyle='--')
            ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    # Subplot 3: Max Cluster Size (Open Addressing)
    ax = axes[1, 0]
    if 'max_cluster_mean' in grouped.columns:
        data = grouped[(grouped['algorithm'] == 'open_addressing') & 
                      (grouped['data_type'] == 'insert')]
        if not data.empty:
            ax.plot(data['size'], data['max_cluster_mean'], marker='o', 
                   linewidth=2.5, color=colors['open_addressing'], markersize=6)
            ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
            ax.set_ylabel('Tamanho do Maior Cluster', fontsize=11, fontweight='bold')
            ax.set_title('End. Aberto: Clustering Primário', fontsize=12, fontweight='bold')
            ax.grid(True, alpha=0.3, linestyle='--')
            ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    # Subplot 4: Avg Probes per Element (Open Addressing)
    ax = axes[1, 1]
    if 'total_probes_mean' in grouped.columns:
        data = grouped[(grouped['algorithm'] == 'open_addressing') & 
                      (grouped['data_type'] == 'insert')].copy()
        if not data.empty:
            data['avg_probes'] = data['total_probes_mean'] / data['size']
            ax.plot(data['size'], data['avg_probes'], marker='o', 
                   linewidth=2.5, color=colors['open_addressing'], markersize=6)
            ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
            ax.set_ylabel('Probes Médios por Elemento', fontsize=11, fontweight='bold')
            ax.set_title('End. Aberto: Custo de Probing', fontsize=12, fontweight='bold')
            ax.grid(True, alpha=0.3, linestyle='--')
            ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    plt.suptitle('Q14: Métricas Estruturais das Tabelas Hash', 
                 fontsize=16, fontweight='bold', y=0.995)
    plt.tight_layout()
    filename = output_dir / 'q14_structural_metrics.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()
    
    # Gráfico 6: Load Factor ao longo dos tamanhos
    fig, ax = plt.subplots(figsize=(14, 8))
    
    if 'load_factor_mean' in grouped.columns:
        for algo in ['chaining', 'open_addressing']:
            data = grouped[(grouped['algorithm'] == algo) & 
                          (grouped['data_type'] == 'insert')]
            if not data.empty:
                ax.plot(data['size'], data['load_factor_mean'], marker='o', 
                       label=algo_names[algo], linewidth=2.5, 
                       color=colors[algo], markersize=6)
        
        ax.axhline(y=0.7, color='green', linestyle='--', linewidth=2, 
                  label='Load Factor Alvo (0.7)', alpha=0.7)
        ax.set_xlabel('Número de Elementos (n)', fontsize=12, fontweight='bold')
        ax.set_ylabel('Load Factor (α = n/m)', fontsize=12, fontweight='bold')
        ax.set_title('Q14: Load Factor Real das Tabelas Hash', 
                    fontsize=14, fontweight='bold')
        ax.legend(fontsize=11)
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
        
        plt.tight_layout()
        filename = output_dir / 'q14_load_factor.png'
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"  Salvo: {filename}")
        plt.close()

def plot_q15_hash_probing(df, output_dir):
    """Plota resultados da Questão 15 (Hash Probing)"""
    print("Gerando gráficos da Questão 15...")
    
    # Parse metadata
    metadata_df = df['metadata'].apply(parse_metadata).apply(pd.Series)
    df = pd.concat([df, metadata_df], axis=1)
    
    # Remove outliers
    def remove_outliers_iqr(df_in):
        return df_in.groupby(['algorithm','data_type'], group_keys=False).apply(
            lambda g: g if len(g) < 5 else g[
                (g['time_s'] >= g['time_s'].quantile(0.25) - 3*(g['time_s'].quantile(0.75)-g['time_s'].quantile(0.25))) & 
                (g['time_s'] <= g['time_s'].quantile(0.75) + 3*(g['time_s'].quantile(0.75)-g['time_s'].quantile(0.25)))
            ]
        )
    
    df = remove_outliers_iqr(df)
    
    # Agrupa
    agg_dict = {'time_s': ['mean', 'std']}
    if 'probes' in df.columns:
        agg_dict['probes'] = 'mean'
    if 'avg_probes' in df.columns:
        agg_dict['avg_probes'] = 'mean'
    if 'max_cluster' in df.columns:
        agg_dict['max_cluster'] = 'mean'
    
    grouped = df.groupby(['size', 'algorithm', 'data_type']).agg(agg_dict).reset_index()
    grouped.columns = ['_'.join(col).strip('_') if col[1] else col[0] for col in grouped.columns.values]
    
    algo_names = {
        'linear_probing': 'Linear Probing',
        'double_hashing': 'Double Hashing'
    }
    
    operation_names = {
        'insert': 'Inserção',
        'search': 'Busca (Hit)',
        'search_miss': 'Busca (Miss)',
        'remove': 'Remoção'
    }
    
    colors = {
        'linear_probing': '#E63946',
        'double_hashing': '#2A9D8F'
    }
    
    # Gráfico 1: Tempo por operação
    fig, axes = plt.subplots(2, 2, figsize=(16, 12))
    axes = axes.flatten()
    
    for idx, operation in enumerate(['insert', 'search', 'search_miss', 'remove']):
        ax = axes[idx]
        data = grouped[grouped['data_type'] == operation]
        
        for algo in ['linear_probing', 'double_hashing']:
            algo_data = data[data['algorithm'] == algo]
            if algo_data.empty:
                continue
            
            ax.plot(algo_data['size'], algo_data['time_s_mean'] * 1000,
                   marker='o', label=algo_names[algo], linewidth=2.5, 
                   color=colors[algo], markersize=6)
            ax.fill_between(algo_data['size'],
                          (algo_data['time_s_mean'] - algo_data['time_s_std']) * 1000,
                          (algo_data['time_s_mean'] + algo_data['time_s_std']) * 1000,
                          alpha=0.2, color=colors[algo])
        
        ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
        ax.set_ylabel('Tempo (ms)', fontsize=11, fontweight='bold')
        ax.set_title(f'{operation_names[operation]}', fontsize=12, fontweight='bold')
        ax.legend(fontsize=10, loc='upper left')
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    plt.suptitle('Q15: Hash Probing - Tempo de Execução por Operação', 
                 fontsize=16, fontweight='bold', y=0.995)
    plt.tight_layout()
    filename = output_dir / 'q15_operations_time.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()
    
    # Gráfico 2: Número de Probes por Operação
    if 'probes_mean' in grouped.columns:
        fig, axes = plt.subplots(2, 2, figsize=(16, 12))
        axes = axes.flatten()
        
        for idx, operation in enumerate(['insert', 'search', 'search_miss', 'remove']):
            ax = axes[idx]
            data = grouped[grouped['data_type'] == operation]
            
            for algo in ['linear_probing', 'double_hashing']:
                algo_data = data[data['algorithm'] == algo]
                if algo_data.empty or 'probes_mean' not in algo_data.columns:
                    continue
                
                ax.plot(algo_data['size'], algo_data['probes_mean'],
                       marker='o', label=algo_names[algo], linewidth=2.5, 
                       color=colors[algo], markersize=6)
            
            ax.set_xlabel('Número de Elementos (n)', fontsize=11, fontweight='bold')
            ax.set_ylabel('Total de Probes', fontsize=11, fontweight='bold')
            ax.set_title(f'{operation_names[operation]}', fontsize=12, fontweight='bold')
            ax.legend(fontsize=10, loc='upper left')
            ax.grid(True, alpha=0.3, linestyle='--')
            ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
            ax.yaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
        
        plt.suptitle('Q15: Hash Probing - Total de Probes por Operação', 
                     fontsize=16, fontweight='bold', y=0.995)
        plt.tight_layout()
        filename = output_dir / 'q15_total_probes.png'
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"  Salvo: {filename}")
        plt.close()
    
    # Gráfico 3: Probes Médios por Elemento
    if 'avg_probes_mean' in grouped.columns:
        fig, ax = plt.subplots(figsize=(14, 8))
        
        data = grouped[grouped['data_type'] == 'insert']
        for algo in ['linear_probing', 'double_hashing']:
            algo_data = data[data['algorithm'] == algo]
            if algo_data.empty:
                continue
            
            ax.plot(algo_data['size'], algo_data['avg_probes_mean'],
                   marker='o', label=algo_names[algo], linewidth=2.5, 
                   color=colors[algo], markersize=7)
        
        ax.set_xlabel('Número de Elementos (n)', fontsize=12, fontweight='bold')
        ax.set_ylabel('Probes Médios por Busca', fontsize=12, fontweight='bold')
        ax.set_title('Q15: Eficiência de Probing\n(Menor é melhor)', 
                    fontsize=14, fontweight='bold')
        ax.legend(fontsize=11)
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
        
        plt.tight_layout()
        filename = output_dir / 'q15_avg_probes.png'
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"  Salvo: {filename}")
        plt.close()
    
    # Gráfico 4: Clustering
    if 'max_cluster_mean' in grouped.columns:
        fig, ax = plt.subplots(figsize=(14, 8))
        
        data = grouped[grouped['data_type'] == 'insert']
        for algo in ['linear_probing', 'double_hashing']:
            algo_data = data[data['algorithm'] == algo]
            if algo_data.empty:
                continue
            
            ax.plot(algo_data['size'], algo_data['max_cluster_mean'],
                   marker='o', label=algo_names[algo], linewidth=2.5, 
                   color=colors[algo], markersize=7)
        
        ax.set_xlabel('Número de Elementos (n)', fontsize=12, fontweight='bold')
        ax.set_ylabel('Tamanho do Maior Cluster', fontsize=12, fontweight='bold')
        ax.set_title('Q15: Clustering Primário\n(Linear Probing sofre mais)', 
                    fontsize=14, fontweight='bold')
        ax.legend(fontsize=11)
        ax.grid(True, alpha=0.3, linestyle='--')
        ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
        
        plt.tight_layout()
        filename = output_dir / 'q15_clustering.png'
        plt.savefig(filename, dpi=300, bbox_inches='tight')
        print(f"  Salvo: {filename}")
        plt.close()
    
    # Gráfico 5: Razão de Performance
    fig, ax = plt.subplots(figsize=(14, 8))
    
    for operation in ['insert', 'search', 'search_miss', 'remove']:
        data = grouped[grouped['data_type'] == operation]
        
        linear_data = data[data['algorithm'] == 'linear_probing'].set_index('size')['time_s_mean']
        double_data = data[data['algorithm'] == 'double_hashing'].set_index('size')['time_s_mean']
        
        if linear_data.empty or double_data.empty:
            continue
        
        joined = linear_data.to_frame('linear').join(double_data.to_frame('double'), how='inner')
        if joined.empty:
            continue
        
        ratio = joined['linear'] / joined['double']
        ax.plot(ratio.index, ratio.values, marker='o', linewidth=2.5, 
               label=operation_names[operation], markersize=7)
    
    ax.axhline(y=1.0, color='red', linestyle='--', linewidth=2, 
              label='Performance Igual', alpha=0.7)
    ax.set_xlabel('Número de Elementos (n)', fontsize=12, fontweight='bold')
    ax.set_ylabel('Razão (Tempo Linear / Tempo Double)', fontsize=12, fontweight='bold')
    ax.set_title('Q15: Razão de Performance Relativa\n(valores > 1: Double Hashing é mais rápido)', 
                fontsize=14, fontweight='bold')
    ax.legend(fontsize=11, loc='best')
    ax.grid(True, alpha=0.3, linestyle='--')
    ax.xaxis.set_major_formatter(mtick.StrMethodFormatter('{x:,.0f}'))
    
    plt.tight_layout()
    filename = output_dir / 'q15_performance_ratio.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()
    
    # Gráfico 6: Comparação em Barras
    summary = grouped.groupby(['algorithm', 'data_type'])['time_s_mean'].mean().reset_index()
    
    operations = ['insert', 'search', 'search_miss', 'remove']
    x = range(len(operations))
    width = 0.35
    
    fig, ax = plt.subplots(figsize=(12, 8))
    
    linear_times = [summary[(summary['algorithm'] == 'linear_probing') & 
                           (summary['data_type'] == op)]['time_s_mean'].values[0] * 1000 
                   if not summary[(summary['algorithm'] == 'linear_probing') & 
                                 (summary['data_type'] == op)].empty else 0
                   for op in operations]
    
    double_times = [summary[(summary['algorithm'] == 'double_hashing') & 
                          (summary['data_type'] == op)]['time_s_mean'].values[0] * 1000
                  if not summary[(summary['algorithm'] == 'double_hashing') & 
                                (summary['data_type'] == op)].empty else 0
                  for op in operations]
    
    bars1 = ax.bar([i - width/2 for i in x], linear_times, width, 
                   label='Linear Probing', color=colors['linear_probing'], alpha=0.8)
    bars2 = ax.bar([i + width/2 for i in x], double_times, width,
                   label='Double Hashing', color=colors['double_hashing'], alpha=0.8)
    
    ax.set_xlabel('Operação', fontsize=12, fontweight='bold')
    ax.set_ylabel('Tempo Médio (ms)', fontsize=12, fontweight='bold')
    ax.set_title('Q15: Comparação de Performance Média por Operação', 
                fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels([operation_names[op] for op in operations])
    ax.legend(fontsize=11)
    ax.grid(True, alpha=0.3, axis='y', linestyle='--')
    
    def autolabel(bars):
        for bar in bars:
            height = bar.get_height()
            if height > 0:
                ax.annotate(f'{height:.2f}',
                           xy=(bar.get_x() + bar.get_width() / 2, height),
                           xytext=(0, 3),
                           textcoords="offset points",
                           ha='center', va='bottom', fontsize=9)
    
    autolabel(bars1)
    autolabel(bars2)
    
    plt.tight_layout()
    filename = output_dir / 'q15_comparison_bars.png'
    plt.savefig(filename, dpi=300, bbox_inches='tight')
    print(f"  Salvo: {filename}")
    plt.close()

def detect_question(df):
    """Detecta qual questão baseado nas colunas e valores"""
    if 'algorithm' in df.columns:
        algos = df['algorithm'].unique()
        
        # Q15: Hash probing
        if any(a in algos for a in ['linear_probing', 'double_hashing']):
            return 'q15'
        
        # Q14: Hash tables
        if any(a in algos for a in ['chaining', 'open_addressing']):
            return 'q14'
        
        # Q10: Min/Max
        has_ns = any(a in algos for a in ['separate', 'não simultaneo'])
        has_sim = any(a in algos for a in ['simultaneous', 'simultaneo'])
        if has_ns and has_sim:
            return 'q10'
        
        # Q11: Selection
        if 'randomized_select' in algos or 'deterministic_select' in algos:
            return 'q11'
    
    if 'data_structure' in df.columns:
        return 'q13'
    
    return None

def main():
    if len(sys.argv) < 2:
        print("Uso: python3 plot_questoes.py <arquivo.csv> [arquivo2.csv ...]")
        print("\nGera gráficos dos resultados dos benchmarks das questões.")
        sys.exit(1)
    
    output_dir = Path("mestrado/plots_questoes")
    output_dir.mkdir(parents=True, exist_ok=True)
    
    print("=" * 60)
    print("GERADOR DE GRÁFICOS - QUESTÕES DO MESTRADO")
    print("=" * 60)
    
    for csv_file in sys.argv[1:]:
        print(f"\nProcessando: {csv_file}")
        
        if not os.path.exists(csv_file):
            print(f"  ERRO: Arquivo não encontrado: {csv_file}")
            continue
        
        try:
            df = pd.read_csv(csv_file)
            print(f"  {len(df)} linhas carregadas")
            
            question = detect_question(df)
            
            if question == 'q10':
                plot_q10_minmax(df, output_dir)
            elif question == 'q14':
                plot_q14_hash_tables(df, output_dir)
            elif question == 'q15':
                plot_q15_hash_probing(df, output_dir)
            else:
                print(f"  Tipo de questão não identificado ou ainda não suportado")
                print(f"  Colunas disponíveis: {', '.join(df.columns)}")
        
        except Exception as e:
            print(f"  ERRO ao processar {csv_file}: {e}")
            import traceback
            traceback.print_exc()
    
    print("\n" + "=" * 60)
    print(f"Gráficos salvos em: {output_dir}")
    print("=" * 60)

if __name__ == "__main__":
    main()

