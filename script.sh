#!/bin/bash

# Nombre del archivo fuente y del ejecutable
SOURCE_FILE="integral8.c"     # Cambia "tu_programa.c" por el nombre de tu archivo fuente.
EXECUTABLE="pi_montecarlo"

# Compilar el archivo fuente con mpicc
mpicc -o $EXECUTABLE $SOURCE_FILE -lm

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error al compilar el programa."
    exit 1
fi

# Ejecutar el programa con diferentes números de procesos (de 1 a 32)
for PROC in $(seq 1 32); do
    echo "Ejecutando con $PROC proceso(s):"
    mpirun -np $PROC ./$EXECUTABLE
    echo ""
done
