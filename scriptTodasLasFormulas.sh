#!/bin/bash
#-------------------------------
# MPI job script for CESGA
#-------------------------------

#SBATCH -J pi_montecarlo_7       # Job name
#SBATCH -o mitr_%j.o             # Name of stdout output file
#SBATCH -e mitr_%j.e             # Name of stderr error file
#SBATCH -N 2                     # Total number of nodes requested
#SBATCH -n 32                    # Total number of mpi tasks requested
#SBATCH --ntasks-per-node=16     # Number of tasks per node
#SBATCH -t 24:00:00              # Run time (hh:mm:ss)
#SBATCH --mem=2G                 # Memory per core

# Load necessary modules for compilation and execution
module load gcc openmpi/4.1.4_ft3

# Nombre del archivo fuente y del ejecutable
SOURCE_FILE="integralTodasLasFormulas.c"     # Cambia esto por el nombre de tu archivo fuente.
EXECUTABLE="ejecutable"

# Verificar que el usuario pase el argumento de fórmula
if [ -z "$1" ]; then
    echo "Error: Debes proporcionar un número del 1 al 10 como parámetro para FORM."
    exit 1
fi

FORM=$1  # Asignar el primer argumento como el número de fórmula

# Validar que FORM sea un número entre 1 y 10, excluyendo 8
if ! [[ "$FORM" =~ ^[1-9]$|^10$ ]] || [ "$FORM" -eq 8 ]; then
    echo "Error: FORM debe ser un número del 1 al 10, excluyendo 8."
    exit 1
fi

# Compilar el archivo fuente con mpicc
mpicc -o $EXECUTABLE $SOURCE_FILE -lm

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error al compilar el programa."
    exit 1
fi

# Nombre del archivo de salida específico para la fórmula
OUTPUT_FILE="salida${FORM}.txt"
# Eliminar el archivo de salida si existe
rm -f "$OUTPUT_FILE"

# Ejecutar el programa con diferentes números de procesos y de iteraciones
for PROC in $(seq 1 32); do
    for ITER in $(seq 1000000000 1000000000 10000000000); do
        # Ejecutar el programa con el número de procesos y escribir en el archivo específico de fórmula
        mpirun -np $PROC ./$EXECUTABLE $ITER "$OUTPUT_FILE" $FORM
    done
done

echo "Done"   # Write a message to the output file

#-------------------------------
