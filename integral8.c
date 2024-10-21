#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>     

#define iteraciones_totales_montecarlo 10000000

double operacionIntraIntegral(double x);
int integralMonteCarlo(int iteraciones);

int main(int argc, char* argv[]) {
    int rank, size;
    double start_time, end_time;

    // Inicialización del entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtener el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtener el número total de procesos

    if (rank == 0) {
        printf("Calculando π usando %d procesos y %d iteraciones en total\n", size, iteraciones_totales_montecarlo);
    }

    // Iniciar el contador de tiempo
    start_time = MPI_Wtime();

    int iteraciones_por_proceso = iteraciones_totales_montecarlo / size;
    // Cada proceso realiza una parte de las iteraciones
    int integral_local = integralMonteCarlo(iteraciones_por_proceso);

    // Recopilar resultados parciales de cada proceso
    double integral_total = 0.0;
    MPI_Reduce(&integral_local, &integral_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    integral_total /= iteraciones_totales_montecarlo;  

    if (rank == 0) {
        // Calcular el valor de pi a partir de la integral total
        integral_total /= size;  // Dividimos por el número de procesos para obtener la media
        double pi = (double) 22 / 7 - integral_total;

        // Finalizar el contador de tiempo
        end_time = MPI_Wtime();
        double cpu_time_used = end_time - start_time;

        // Imprimir el resultado
        printf("El valor de π es: %.15f\n", pi);
        printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
    }

    // Finalización del entorno MPI
    MPI_Finalize();

    return 0;
}

/**
 * @brief Calcular la función del calculo de pi
 * @param x Valor de x
 * @return Valor de la integral
*/
double operacionIntraIntegral(double x){
    return ((x * x * x * x) * ((1-x) * (1-x) * (1-x) * (1-x)) ) / (1+x) * (1+x) ;
}

/**
 * @brief Calcular la integral de una función mediante el método de Monte Carlo
 * @param function función a integrar
 * @param min Valor mínimo del intervalo
 * @param max Valor máximo del intervalo
 * @return Valor de la integral
*/
int integralMonteCarlo(int iteraciones){
    // Para asegurarnos que en cada ejecución del método montecarlo se generen números aleatorios diferentes, utilizamos el PID del programa
    srand(getpid());
    int aciertos=0;
    
    for(int i = 0; i < iteraciones; i++){
        // Generar un número aleatorio entre 0 y 1
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        
        if(operacionIntraIntegral(x) > y) aciertos++;
    }

    return aciertos;
}