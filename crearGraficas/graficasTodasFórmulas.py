import glob
import pandas as pd
import matplotlib.pyplot as plt

# Cargar todos los archivos de salida en una lista
archivos = glob.glob('../resultados/salida2-*.txt')

# Crear un dataframe vacío para almacenar todos los datos
columnas = ['procesadores', 'iteraciones', 'resultado_pi', 'tiempo_ejecucion', 'formula_id']
df_total = pd.DataFrame(columns=columnas)

# Cargar los datos de cada archivo y agregarlos al dataframe total
for archivo in archivos:
    df = pd.read_csv(archivo, delim_whitespace=True, header=None, names=columnas)
    df_total = pd.concat([df_total, df], ignore_index=True)

# Calcular el error respecto al valor de referencia de pi
pi_referencia = 3.141592653589793
df_total['error'] = abs(df_total['resultado_pi'] - pi_referencia)

# Calcular la calidad como inversa del producto del tiempo de ejecución por el error
df_total['calidad'] = 1 / (df_total['tiempo_ejecucion'] * df_total['error'])

# Agregar métricas de Speed-up y Eficiencia
# Supongamos que tenemos el tiempo de ejecución en el caso secuencial para cada fórmula y número de iteraciones
# Aquí reemplaza con los valores correctos de tiempos secuenciales o ajusta según tu caso específico
tiempo_secuencial = df_total[df_total['procesadores'] == 1].set_index(['formula_id', 'iteraciones'])['tiempo_ejecucion']
df_total = df_total.join(tiempo_secuencial, on=['formula_id', 'iteraciones'], rsuffix='_secuencial')
df_total['speedup'] = df_total['tiempo_ejecucion_secuencial'] / df_total['tiempo_ejecucion']
df_total['eficiencia'] = df_total['speedup'] / df_total['procesadores']

# Calcular la media de tiempo, error, calidad, speedup y eficiencia por fórmula y número de iteraciones
df_media = df_total.groupby(['formula_id', 'iteraciones']).agg({
    'resultado_pi': 'mean',
    'tiempo_ejecucion': 'mean',
    'error': 'mean',
    'calidad': 'mean',
    'speedup': 'mean',
    'eficiencia': 'mean'
}).reset_index()

# Calcular las medias globales por número de iteraciones para añadir a las gráficas
df_media_global = df_total.groupby('iteraciones').agg({
    'error': 'mean',
    'calidad': 'mean',
    'speedup': 'mean',
    'eficiencia': 'mean'
}).reset_index()

# Generar gráficos de media de cada métrica por fórmula y superponer las medias globales
metricas = ['error', 'calidad', 'speedup', 'eficiencia']

for metrica in metricas:
    plt.figure()
    for formula_id in df_media['formula_id'].unique():
        datos_formula = df_media[df_media['formula_id'] == formula_id]
        plt.plot(datos_formula['iteraciones'], datos_formula[metrica], label=f'Fórmula {formula_id}')
    
    # Superponer la media global
    plt.plot(df_media_global['iteraciones'], df_media_global[metrica], color='black', linestyle='--', label=f'Media de {metrica.capitalize()}')
    
    plt.xlabel('Número de Iteraciones')
    plt.ylabel(f'Media {metrica.capitalize()}')
    plt.title(f'Media de {metrica.capitalize()} por Fórmula y Número de Iteraciones')
    plt.legend()
    plt.show()

print("Análisis completado. Se muestran los gráficos de medias por fórmula con la media global superpuesta.")
