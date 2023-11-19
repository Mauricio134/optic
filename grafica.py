import matplotlib.pyplot as plt
import numpy as np
import matplotlib.colors as mcolors

nombre_archivo = "clusters.txt"

colores_rgb = [mcolors.to_rgba(np.random.rand(3))[:3] for _ in range(1000)]

maximox = -1e9
minimox = 1e9
maximoy = -1e9
minimoy = 1e9

with open(nombre_archivo, "r") as archivo:
    for linea in archivo:
        partes = linea.split()
        if len(partes) == 3:
            valor1, valor2, indice = partes
            indice = int(indice)

            maximox = max(maximox, float(valor1))
            maximoy = max(maximoy, float(valor2))
            minimox = min(minimox, float(valor1))
            minimoy = min(minimoy, float(valor2))
            if indice != -1:
                plt.scatter(float(valor1), float(valor2), c=colores_rgb[indice-1], marker='o')
            else:
                plt.scatter(float(valor1), float(valor2), c="black", marker='o')

plt.xlabel('Eje X')
plt.ylabel('Eje Y')
# Asegúrate de usar abs(maximoy-minimoy) en lugar de abs(maximox-minimox)
num_ticks = 6  # Puedes ajustar este número según tus necesidades
plt.xticks(np.linspace(minimox, maximox, num_ticks))
plt.yticks(np.linspace(minimoy, maximoy, num_ticks))

plt.title('Gráfico de Optics')

plt.legend()

plt.show()
