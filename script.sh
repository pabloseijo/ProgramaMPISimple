#!/bin/bash
#-------------------------------
# MPI job script for CESGA
#-------------------------------

#SBATCH -J pi_montecarlo_7       # Job name
#SBATCH -o mitr_%j.o             # Name of stdout output file
#SBATCH -e mitr_%j.e             # Name of stderr error file
#SBATCH -N 2                     # Total number of nodes requested
#SBATCH -n 8                     # Total number of mpi tasks requested
#SBATCH --ntasks-per-node=4      # Number of tasks per node (for example 4 tasks per node)
#SBATCH -t 00:10:00              # Run time (hh:mm:ss) - here 10 minutes
#SBATCH --mem=2G                 # Memory per core

# Load necessary modules for compilation and execution
module load gcc openmpi/4.1.4_ft3

# Nombre del archivo fuente y del ejecutable
SOURCE_FILE="integral8.c"     # Cambia esto por el nombre de tu archivo fuente.
EXECUTABLE="pi_montecarlo"

# Compilar el archivo fuente con mpicc
mpicc -o $EXECUTABLE $SOURCE_FILE -lm

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error al compilar el programa."
    exit 1
fi

rm "salida.txt"

# Ejecutar el programa con diferentes números de procesos (de 1 a 8)
for PROC in $(seq 1 8); do
    for ITER in $(seq 100000 100000 1000000); do
        echo "Ejecutando con $PROC proceso(s):"
        mpirun -np $PROC ./$EXECUTABLE $ITER "salida.txt"
        echo ""
    done
done

echo "Done"   # Write a message to the output file

#-------------------------------
