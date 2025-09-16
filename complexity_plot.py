import numpy as np
import matplotlib.pyplot as plt

# Definindo o intervalo de n (tamanho da entrada)
n = np.arange(4, 20001, 1)

# Definindo as funções de complexidade
o_n3 = n ** 3
o_n2 = n ** 2
o_n281 = n ** 2.81

# Criando o gráfico
plt.figure(figsize=(12, 8))

# Plotando as funções
plt.plot(n, o_n3, label='O(n³)', linewidth=2)
plt.plot(n, o_n2, label='O(n²)', linewidth=2)
plt.plot(n, o_n281, label='O(n^2.81)', linewidth=2)

# Configurando a escala logarítmica no eixo Y
plt.yscale('log')

# Configurando os limites dos eixos
plt.xlim(0, 20000)
plt.ylim(1, max(o_n3) * 1.1)  # Começando em 1 para evitar log(0)

# Adicionando títulos e labels
plt.title('Comparação de Complexidades Teóricas', fontsize=16)
plt.xlabel('Tamanho da Entrada (n)', fontsize=14)
plt.ylabel('Valor da Função (escala logarítmica)', fontsize=14)

# Adicionando legenda
plt.legend(fontsize=12)

# Adicionando grid
plt.grid(True, alpha=0.3)

# Salvando o gráfico como arquivo PNG
plt.tight_layout()
plt.savefig('complexity_comparison.png', dpi=300, bbox_inches='tight')
print("Gráfico salvo como 'complexity_comparison.png'")

# Exibindo o gráfico
plt.show()
