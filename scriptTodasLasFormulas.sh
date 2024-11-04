#!/bin/bash
#-------------------------------
# MPI job script for CESGA
#-------------------------------

#SBATCH -J pi_montecarlo_7       # Job name
#SBATCH -o mitr_%j.o             # Name of stdout output file
#SBATCH -e mitr_%j.e             # Name of stderr error file
#SBATCH -N 2                     # Total number of nodes requested
#SBATCH -n 32                     # Total number of mpi tasks requested
#SBATCH --ntasks-per-node=16      # Number of tasks per node (for example 4 tasks per node)
#SBATCH -t 20:00:00              # Run time (hh:mm:ss) - here 10 minutes
#SBATCH --mem=2G                 # Memory per core

# Load necessary modules for compilation and execution
module load gcc openmpi/4.1.4_ft3

# Nombre del archivo fuente y del ejecutable
SOURCE_FILE="integralTodasLasFormulas.c"     # Cambia esto por el nombre de tu archivo fuente.
EXECUTABLE="ejecutable"

# Compilar el archivo fuente con mpicc
mpicc -o $EXECUTABLE $SOURCE_FILE -lm

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error al compilar el programa."
    exit 1
fi

rm "salida.txt"

# Ejecutar el programa con diferentes números de procesos (de 1 a 8)
for FORM in $(seq 1 10); do
    if [ "$FORM" -eq 8 ]; do
        continue
    fi
    for PROC in $(seq 1 32); do
	for ITER in $(seq 1000000000 1000000000 10000000000); do
            # echo "Ejecutando con $PROC proceso(s):"
            mpirun -np $PROC ./$EXECUTABLE $ITER "salida.txt" $FORM
            # echo ""
	done
    done
done

echo "Done"   # Write a message to the output file

#-------------------------------
