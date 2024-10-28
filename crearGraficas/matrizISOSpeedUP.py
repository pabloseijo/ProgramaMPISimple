import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# Leer archivo de datos
filename = "salida1.txt"  # Nombre del archivo actualizado
data = pd.read_csv(filename, sep=" ", names=["nProcesos", "nIteraciones", "valorCalculado", "tiempoCalculo", "formulaUtilizada"])

# Crear una columna de SpeedUp en el DataFrame
# Primero, obtener el tiempo secuencial (cuando nProcesos es 1) para cada grupo de nIteraciones
for iteracion in data["nIteraciones"].unique():
    tiempo_secuencial = data[(data["nProcesos"] == 1) & (data["nIteraciones"] == iteracion)]["tiempoCalculo"].values[0]
    data.loc[data["nIteraciones"] == iteracion, "SpeedUp"] = tiempo_secuencial / data[data["nIteraciones"] == iteracion]["tiempoCalculo"]

# Convertir el DataFrame a formato de tabla para la gráfica
heatmap_data = data.pivot(index="nProcesos", columns="nIteraciones", values="SpeedUp")

# Crear la gráfica de calor
plt.figure(figsize=(12, 10))  # Ajusta el tamaño de la figura
sns.heatmap(heatmap_data, annot=True, cmap="coolwarm", fmt=".2f", linewidths=0.5, cbar_kws={"label": "SpeedUp"}, square=False)
plt.xlabel("Número de Iteraciones")
plt.ylabel("Número de Procesos")
plt.title("SpeedUp en función de Número de Procesos y Número de Iteraciones")

# Invertir el eje y para que los procesos más altos estén en la parte superior
plt.gca().invert_yaxis()

plt.show()
