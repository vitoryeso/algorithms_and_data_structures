import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Configurar estilo dos gráficos
plt.style.use('ggplot')

# Ler dados do CSV
df = pd.read_csv('insertion_sort_detailed_results.csv')

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
             linewidth=2, markersize=5, alpha=0.9, label=f'Insertion Sort - Vetor {tipo}')

# Adicionar curvas teóricas para comparação (ajuste por mínimos quadrados passando pela origem)
x_teorico = np.linspace(int(df['tamanho'].min()), int(df['tamanho'].max()), 200)

# Curva O(n) - ajustada ao melhor caso (ordenado)
tempo_ordenado = df[df['tipo_teste'] == 'ordenado']
x_ord = tempo_ordenado['tamanho'].to_numpy(dtype=float)
y_ord = tempo_ordenado['tempo_ms'].to_numpy(dtype=float)
den_n = float(np.dot(x_ord, x_ord))
c_n = (float(np.dot(x_ord, y_ord)) / den_n) if den_n != 0.0 else 0.0
curva_n = c_n * x_teorico

# Curva O(n²) - ajustada ao pior caso (invertido)
tempo_invertido = df[df['tipo_teste'] == 'invertido']
x_inv = tempo_invertido['tamanho'].to_numpy(dtype=float)
y_inv = tempo_invertido['tempo_ms'].to_numpy(dtype=float)
x2_inv = x_inv ** 2
den_n2 = float(np.dot(x2_inv, x2_inv))
c_n2 = (float(np.dot(x2_inv, y_inv)) / den_n2) if den_n2 != 0.0 else 0.0
curva_n2 = c_n2 * (x_teorico ** 2)

plt.plot(x_teorico, curva_n, linestyle='--', linewidth=2.5, alpha=0.9, color='#CC79A7', label='f(n) = n (Teórico)')
plt.plot(x_teorico, curva_n2, linestyle='--', linewidth=2.5, alpha=0.9, color='#999999', label='g(n) = n² (Teórico)')

plt.xlabel('Tamanho do Vetor (n)', fontsize=12)
plt.ylabel('Tempo de Execução (ms)', fontsize=12)
plt.title('Comparação Insertion Sort', fontsize=14, fontweight='bold')
plt.legend(fontsize=10)
plt.grid(True, alpha=0.3)

# Usar escala linear (sem log)
# plt.yscale('linear')  # Redundante, mas explícito
# plt.xscale('linear')  # Redundante, mas explícito

# Formatar eixos
plt.gca().tick_params(axis='both', which='major', labelsize=10)

# Remover anotações para não poluir o gráfico com muitos dados
# Com 35 pontos por linha, as anotações ficariam muito poluídas

plt.tight_layout()
plt.savefig('insertion_sort_detailed_performance.png', dpi=300, bbox_inches='tight')
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

# R² para O(n) com dados ordenados
y_pred_ord = c_n * x_ord
r2_ord = r2_score(y_ord, y_pred_ord)
print(f"Ordenado vs f(n)=n: R² = {r2_ord:.4f}")

# R² para O(n²) com dados invertidos
y_pred_inv = c_n2 * (x_inv ** 2)
r2_inv = r2_score(y_inv, y_pred_inv)
print(f"Invertido vs g(n)=n²: R² = {r2_inv:.4f}")
