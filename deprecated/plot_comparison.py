import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Configurar estilo dos gráficos
plt.style.use('ggplot')

# Ler dados dos CSVs
df_insertion = pd.read_csv('insertion_sort_detailed_results.csv')
df_merge = pd.read_csv('merge_sort_detailed_results.csv')

# Criar o gráfico
plt.figure(figsize=(16, 10))

# Cores para insertion sort (tons mais claros)
insertion_colors = {'ordenado': '#66C2A5', 'invertido': '#FC8D62', 'aleatorio': '#8DA0CB'}
insertion_markers = {'ordenado': 'o', 'invertido': 's', 'aleatorio': '^'}

# Cores para merge sort (tons mais escuros)
merge_colors = {'ordenado': '#009E73', 'invertido': '#D55E00', 'aleatorio': '#0072B2'}
merge_markers = {'ordenado': 'D', 'invertido': 'X', 'aleatorio': 'v'}

# Plotar insertion sort
for tipo in ['ordenado', 'invertido', 'aleatorio']:
    dados_tipo = df_insertion[df_insertion['tipo_teste'] == tipo]
    if dados_tipo.empty:
        continue
    plt.plot(dados_tipo['tamanho'], dados_tipo['tempo_ms'],
             color=insertion_colors.get(tipo), marker=insertion_markers.get(tipo, 'o'),
             linewidth=2, markersize=5, alpha=0.8, 
             label=f'Insertion Sort - {tipo}', linestyle='-')

# Plotar merge sort
for tipo in ['ordenado', 'invertido', 'aleatorio']:
    dados_tipo = df_merge[df_merge['tipo_teste'] == tipo]
    if dados_tipo.empty:
        continue
    plt.plot(dados_tipo['tamanho'], dados_tipo['tempo_ms'],
             color=merge_colors.get(tipo), marker=merge_markers.get(tipo, 'o'),
             linewidth=2, markersize=5, alpha=0.9,
             label=f'Merge Sort - {tipo}', linestyle='--')

# Adicionar curvas teóricas
x_teorico = np.linspace(100, 20000, 200)

# Curva O(n) - baseada no melhor caso do insertion sort
tempo_ordenado_ins = df_insertion[df_insertion['tipo_teste'] == 'ordenado']
x_ord_ins = tempo_ordenado_ins['tamanho'].to_numpy(dtype=float)
y_ord_ins = tempo_ordenado_ins['tempo_ms'].to_numpy(dtype=float)
pontos_grandes = tempo_ordenado_ins[tempo_ordenado_ins['tamanho'] >= 15000]
if not pontos_grandes.empty:
    constante_n = np.mean(pontos_grandes['tempo_ms'] / pontos_grandes['tamanho'])
else:
    constante_n = np.mean(y_ord_ins / x_ord_ins)
curva_n = constante_n * x_teorico

# Curva O(n²) - baseada no pior caso do insertion sort
tempo_invertido_ins = df_insertion[df_insertion['tipo_teste'] == 'invertido']
x_inv_ins = tempo_invertido_ins['tamanho'].to_numpy(dtype=float)
y_inv_ins = tempo_invertido_ins['tempo_ms'].to_numpy(dtype=float)
x2_inv = x_inv_ins ** 2
den_n2 = float(np.dot(x2_inv, x2_inv))
constante_n2 = (float(np.dot(x2_inv, y_inv_ins)) / den_n2) if den_n2 != 0.0 else 0.0
curva_n2 = constante_n2 * (x_teorico ** 2)

# Curva O(n log n) - baseada no merge sort
tempo_merge_ref = df_merge[df_merge['tipo_teste'] == 'aleatorio']
x_merge = tempo_merge_ref['tamanho'].to_numpy(dtype=float)
y_merge = tempo_merge_ref['tempo_ms'].to_numpy(dtype=float)
x_nlogn = x_merge * np.log2(x_merge)
den_nlogn = float(np.dot(x_nlogn, x_nlogn))
constante_nlogn = (float(np.dot(x_nlogn, y_merge)) / den_nlogn) if den_nlogn != 0.0 else 0.0
curva_nlogn = constante_nlogn * x_teorico * np.log2(x_teorico)

# Plotar curvas teóricas
plt.plot(x_teorico, curva_n, linestyle=':', linewidth=3, alpha=0.7, color='#2E8B57', 
         label='O(n) - Teórico')
plt.plot(x_teorico, curva_n2, linestyle=':', linewidth=3, alpha=0.7, color='#B22222', 
         label='O(n²) - Teórico')
plt.plot(x_teorico, curva_nlogn, linestyle=':', linewidth=3, alpha=0.7, color='#4B0082', 
         label='O(n log n) - Teórico')

plt.xlabel('Tamanho do Vetor (n)', fontsize=14)
plt.ylabel('Tempo de Execução (ms)', fontsize=14)
plt.title('Comparação: Insertion Sort vs Merge Sort', fontsize=16, fontweight='bold')

# Organizar legenda em duas colunas
plt.legend(fontsize=10, ncol=2, loc='upper left')
plt.grid(True, alpha=0.3)

# Usar escala logarítmica no eixo Y para melhor visualização
plt.yscale('log')

# Formatar eixos
plt.gca().tick_params(axis='both', which='major', labelsize=12)

plt.tight_layout()
plt.savefig('sorting_algorithms_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

print("Comparação dos Algoritmos:")
print("=" * 60)
print("Insertion Sort:")
print("  - Melhor caso (ordenado): O(n)")
print("  - Pior caso (invertido): O(n²)")
print("  - Caso médio (aleatório): O(n²)")
print("\nMerge Sort:")
print("  - Todos os casos: O(n log n)")
print("\nObservações:")
print("  - Merge sort é consistente independente da entrada")
print("  - Insertion sort varia drasticamente com a ordem inicial")
print("  - Para vetores pequenos, insertion sort pode ser mais rápido")
print("  - Para vetores grandes, merge sort é sempre superior")

# Análise de crossover
print(f"\nAnálise de crossover:")
for tamanho in [1000, 5000, 10000, 15000, 20000]:
    ins_data = df_insertion[df_insertion['tamanho'] == tamanho]
    merge_data = df_merge[df_merge['tamanho'] == tamanho]
    
    if not ins_data.empty and not merge_data.empty:
        print(f"\nTamanho {tamanho}:")
        for tipo in ['ordenado', 'invertido', 'aleatorio']:
            ins_tempo = ins_data[ins_data['tipo_teste'] == tipo]['tempo_ms'].iloc[0]
            merge_tempo = merge_data[merge_data['tipo_teste'] == tipo]['tempo_ms'].iloc[0]
            ratio = ins_tempo / merge_tempo if merge_tempo > 0 else float('inf')
            print(f"  {tipo}: Insertion {ins_tempo:.2f}ms vs Merge {merge_tempo:.2f}ms (ratio: {ratio:.1f}x)")
