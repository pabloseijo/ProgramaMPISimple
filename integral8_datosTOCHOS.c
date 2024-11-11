#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>

//NOTA: rivera grafica la eficiencia en funcion de el numero de cores y las iter

//SpeedUp = Tsec/Tparalelo (menor o igual nprocesos)
//Eficiencia = SpeedUp/nprocesos (menor o igual que 1)

long double operacionIntraIntegral(long double x);
long double integralMonteCarlo(long long iteraciones);

int main(int argc, char* argv[]) {
    int rank, size,step=1;
    double start_time, end_time;

    long long iteraciones_totales_montecarlo=atoll(argv[1]);

    // Inicialización del entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtener el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtener el número total de procesos


    // Iniciar el contador de tiempo
    start_time = MPI_Wtime();

    long long iteraciones_por_proceso = iteraciones_totales_montecarlo / size;
    // Cada proceso realiza una parte de las iteraciones
    long double local_data  = integralMonteCarlo(iteraciones_por_proceso);

    // Recopilar resultados parciales de cada proceso
    while (step < size) {
        if (rank % (2 * step) == 0) {
            // Los procesos en posiciones múltiplos de 2*step reciben datos
            if (rank + step < size) {
                long double received_data;
                MPI_Recv(&received_data, 1, MPI_LONG_DOUBLE, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_data += received_data;  // Aquí puedes combinar datos o almacenarlos en un array
            }
        } else {
            // Los otros procesos envían sus datos y salen del bucle
            int target = rank - step;
            MPI_Send(&local_data, 1, MPI_LONG_DOUBLE, target, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;  // Aumentar el rango de la distancia en el árbol binario
    }

      

    if (rank == 0) {
        // Calcular el valor de pi a partir de la integral total
        local_data /= size;       
        long double pi = (long double)22/7 - local_data;
	
        // Finalizar el contador de tiempo
        end_time = MPI_Wtime();
        double cpu_time_used = end_time - start_time;

        // Imprimir el resultado
        //printf("El valor aproximado de π es: %.15f\n", pi);
        //printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
	FILE* salida = fopen(argv[2],"a");
	fprintf(salida,"%d %lld %.20Lf %lf %d\n",size,iteraciones_totales_montecarlo, pi, cpu_time_used, 7);
    }

    // Finalización del entorno MPI
    MPI_Finalize();

    return 0;
}

/**
 * @brief Calcular la función del cálculo de pi
 * @param x Valor de x
 * @return Valor de la integral
 */
long double operacionIntraIntegral(long double x) {
    return ((x * x * x * x) * ((1 - x) * (1 - x) * (1 - x) * (1 - x))) / ((1 + x) * (1 + x));
}

/**
 * @brief Calcular la integral de una función mediante el método de Monte Carlo
 * @param iteraciones Número de iteraciones
 * @return Aproximación de la integral
 */
long double integralMonteCarlo(long long iteraciones) {
    srand(getpid());  // Semilla diferente para cada proceso
    long long aciertos = 0;    
    for (long long i = 0; i < iteraciones; i++) {
        // Generar un número aleatorio entre 0 y 1
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        
        if (operacionIntraIntegral(x) > y) aciertos++;
    }

    // La integral es la proporción de aciertos respecto al número de iteraciones
    return (long double)aciertos/iteraciones;
}
