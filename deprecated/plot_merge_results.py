import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Configurar estilo dos gráficos
plt.style.use('ggplot')

# Ler dados do CSV
df = pd.read_csv('merge_sort_detailed_results.csv')

# Criar o gráfico
plt.figure(figsize=(14, 10))

# Plotar cada tipo de teste
color_map = {'ordenado': '#009E73', 'invertido': '#D55E00', 'aleatorio': '#0072B2'}
marker_map = {'ordenado': 'o', 'invertido': 's', 'aleatorio': '^'}
for tipo in ['ordenado', 'invertido', 'aleatorio']:
    dados_tipo = df[df['tipo_teste'] == tipo]
    if dados_tipo.empty:
        continue
    plt.plot(dados_tipo['tamanho'], dados_tipo['tempo_ms'],
             color=color_map.get(tipo), marker=marker_map.get(tipo, 'o'),
             linewidth=2, markersize=5, alpha=0.9, label=f'Merge Sort - Vetor {tipo}')

# Adicionar curva teórica n log n (ajuste por mínimos quadrados)
x_teorico = np.linspace(int(df['tamanho'].min()), int(df['tamanho'].max()), 200)

# Para merge sort, esperamos O(n log n) em todos os casos
# Vamos usar dados de um dos tipos para calibrar a constante
tempo_ref = df[df['tipo_teste'] == 'aleatorio']  # Usar aleatório como referência
x_ref = tempo_ref['tamanho'].to_numpy(dtype=float)
y_ref = tempo_ref['tempo_ms'].to_numpy(dtype=float)

# Ajustar para f(n) = c * n * log(n)
x_nlogn = x_ref * np.log2(x_ref)
den_nlogn = float(np.dot(x_nlogn, x_nlogn))
c_nlogn = (float(np.dot(x_nlogn, y_ref)) / den_nlogn) if den_nlogn != 0.0 else 0.0
curva_nlogn = c_nlogn * x_teorico * np.log2(x_teorico)

plt.plot(x_teorico, curva_nlogn, linestyle='--', linewidth=2.5, alpha=0.9, color='#CC79A7', label='f(n) = n log n (Teórico)')

plt.xlabel('Tamanho do Vetor (n)', fontsize=12)
plt.ylabel('Tempo de Execução (ms)', fontsize=12)
plt.title('Comparação Merge Sort', fontsize=14, fontweight='bold')
plt.legend(fontsize=10)
plt.grid(True, alpha=0.3)

# Formatar eixos
plt.gca().tick_params(axis='both', which='major', labelsize=10)

plt.tight_layout()
plt.savefig('merge_sort_detailed_performance.png', dpi=300, bbox_inches='tight')
plt.show()

# Mostrar dados na tabela
print("Resultados dos testes:")
print("=" * 50)
print(df.to_string(index=False))
print("\n")

# Análise dos resultados
print("Análise dos resultados:")
print("=" * 50)
for tamanho in sorted(df['tamanho'].unique()):
    dados_tamanho = df[df['tamanho'] == tamanho]
    print(f"\nTamanho {tamanho}:")
    for _, row in dados_tamanho.iterrows():
        print(f"  {row['tipo_teste'].capitalize()}: {row['tempo_ms']:.2f} ms")
    
    # Calcular razão entre pior e melhor caso evitando divisão por zero
    tempo_max = float(dados_tamanho['tempo_ms'].max())
    tempo_min = float(dados_tamanho['tempo_ms'].min())
    if tempo_min <= 0:
        print("  Razão pior/melhor caso: indefinida (melhor caso ~0 ms)")
    else:
        razao = tempo_max / tempo_min
        print(f"  Razão pior/melhor caso: {razao:.1f}x")

# Cálculo de R² para confirmação de aderência teórica
def r2_score(y_true: np.ndarray, y_pred: np.ndarray) -> float:
    ss_res = float(np.sum((y_true - y_pred) ** 2))
    ss_tot = float(np.sum((y_true - np.mean(y_true)) ** 2))
    return 1.0 - ss_res / ss_tot if ss_tot != 0.0 else 1.0

print("\nAderência teórica (R²):")
print("=" * 50)

# R² para cada tipo vs n log n
for tipo in ['ordenado', 'invertido', 'aleatorio']:
    dados_tipo = df[df['tipo_teste'] == tipo]
    if dados_tipo.empty:
        continue
    x_tipo = dados_tipo['tamanho'].to_numpy(dtype=float)
    y_tipo = dados_tipo['tempo_ms'].to_numpy(dtype=float)
    
    # Ajustar constante específica para este tipo
    x_nlogn_tipo = x_tipo * np.log2(x_tipo)
    den_tipo = float(np.dot(x_nlogn_tipo, x_nlogn_tipo))
    c_tipo = (float(np.dot(x_nlogn_tipo, y_tipo)) / den_tipo) if den_tipo != 0.0 else 0.0
    y_pred_tipo = c_tipo * x_nlogn_tipo
    
    r2_tipo = r2_score(y_tipo, y_pred_tipo)
    print(f"{tipo.capitalize()} vs f(n)=n log n: R² = {r2_tipo:.4f}")

