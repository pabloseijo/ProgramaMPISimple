import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Valor de referencia de π para calcular el error
pi_reference = 3.1415926535897932384626433832795028841971693993751058209749446

# Leer el archivo
data = pd.read_csv('../resultados/salida1', sep=" ", header=None, names=["procesos", "iteraciones", "pi_valor", "tiempo_ejecucion", "formula"])

# Calcular el error en la aproximación de π
data['error'] = np.abs(data['pi_valor'] - pi_reference)

# Calcular la calidad del resultado como C = 1 / (T * E)
data['calidad'] = 1 / (data['tiempo_ejecucion'] * data['error'])

# Crear un diccionario para el tiempo de ejecución del proceso único (1 proceso) en cada número de iteraciones
single_process_times = data[data['procesos'] == 1].set_index('iteraciones')['tiempo_ejecucion'].to_dict()

# Calcular Speed-up y Eficiencia
data['speed_up'] = data.apply(lambda row: min(single_process_times[row['iteraciones']] / row['tiempo_ejecucion'], row['procesos']) if row['procesos'] > 1 else 1, axis=1)
data['eficiencia'] = data['speed_up'] / data['procesos']

# Calcular medias agrupadas por iteraciones
mean_data = data.groupby('iteraciones').mean().reset_index()

# Graficar el Error en la Aproximación de π vs Número de Iteraciones
plt.figure(figsize=(10, 6))
for proceso in data['procesos'].unique():
    subset = data[data['procesos'] == proceso]
    plt.plot(subset['iteraciones'], subset['error'], label=f'Procesos: {proceso}')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Error en la Aproximación de π')
plt.title('Error en la Aproximación de π vs Número de Iteraciones')
plt.grid(True)
plt.show()

# Graficar la Calidad del Resultado vs Número de Iteraciones
plt.figure(figsize=(10, 6))
for proceso in data['procesos'].unique():
    subset = data[data['procesos'] == proceso]
    plt.plot(subset['iteraciones'], subset['calidad'], label=f'Procesos: {proceso}')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Calidad del Resultado (C)')
plt.title('Calidad del Resultado vs Número de Iteraciones')
plt.grid(True)
plt.show()

# Graficar el Speed-up vs Número de Iteraciones para cada proceso
plt.figure(figsize=(10, 6))
for proceso in data['procesos'].unique():
    subset = data[data['procesos'] == proceso]
    plt.plot(subset['iteraciones'], subset['speed_up'], label=f'Procesos: {proceso}')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Speed-up')
plt.title('Speed-up vs Número de Iteraciones')
plt.grid(True)
plt.show()

# Graficar la Eficiencia vs Número de Iteraciones para cada proceso
plt.figure(figsize=(10, 6))
for proceso in data['procesos'].unique():
    subset = data[data['procesos'] == proceso]
    plt.plot(subset['iteraciones'], subset['eficiencia'], label=f'Procesos: {proceso}')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Eficiencia')
plt.title('Eficiencia vs Número de Iteraciones')
plt.grid(True)
plt.show()

# Graficar la media del Error en la Aproximación de π vs Número de Iteraciones
plt.figure(figsize=(10, 6))
plt.plot(mean_data['iteraciones'], mean_data['error'], label='Media de Error')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Media del Error en la Aproximación de π')
plt.title('Media del Error en la Aproximación de π vs Número de Iteraciones')
plt.grid(True)
plt.legend()
plt.show()

# Graficar la media de la Calidad del Resultado vs Número de Iteraciones
plt.figure(figsize=(10, 6))
plt.plot(mean_data['iteraciones'], mean_data['calidad'], label='Media de Calidad')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Media de Calidad del Resultado (C)')
plt.title('Media de Calidad del Resultado vs Número de Iteraciones')
plt.grid(True)
plt.legend()
plt.show()

# Graficar la media del Speed-up vs Número de Iteraciones
plt.figure(figsize=(10, 6))
plt.plot(mean_data['iteraciones'], mean_data['speed_up'], label='Media de Speed-up')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Media del Speed-up')
plt.title('Media del Speed-up vs Número de Iteraciones')
plt.grid(True)
plt.legend()
plt.show()

# Graficar la media de la Eficiencia vs Número de Iteraciones
plt.figure(figsize=(10, 6))
plt.plot(mean_data['iteraciones'], mean_data['eficiencia'], label='Media de Eficiencia')
plt.xlabel('Número de Iteraciones')
plt.ylabel('Media de Eficiencia')
plt.title('Media de Eficiencia vs Número de Iteraciones')
plt.grid(True)
plt.legend()
plt.show()