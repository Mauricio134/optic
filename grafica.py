import matplotlib.pyplot as plt
import numpy as np

nombre_archivo = "clusters.txt"

colors = ["red", "green", "blue", "orange", "yellow", "brown", "purple"]

with open(nombre_archivo, "r") as archivo:
    for linea in archivo:
        partes = linea.split()
        if len(partes) == 3:
            valor1, valor2, indice = partes
            indice = int(indice)
            if indice != -1:
                plt.scatter(valor1, valor2, c=colors[indice%len(colors)], marker='o')
            else:
                plt.scatter(valor1, valor2, c="black", marker='o')

plt.xlabel('Eje X')
plt.ylabel('Eje Y')

plt.title('Gráfico de Optics')

plt.legend()

plt.show()