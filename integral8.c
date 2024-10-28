#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>
#include <math.h>

#define numeroE 2.718281828459045235360

//NOTA: rivera grafica la eficiencia en funcion de el numero de cores y las iter

//SpeedUp = Tsec/Tparalelo (menor o igual nprocesos)
//Eficiencia = SpeedUp/nprocesos (menor o igual que 1)

double i1(long iter);
double i2(long iter);
double i3(long iter);
double i4(long iter);
double i5(long iter);
double i6(long iter);
double i7(long iter);
double i8(long iter);
double i9(long iter);
double i10(long iter);
double f1(double local_data);
double f2(double local_data);
double f3(double local_data);
double f4(double local_data);
double f5(double local_data);
double f6(double local_data);
double f7(double local_data);
double f8(double local_data);
double f9(double local_data);
double f10(double local_data);
long  factorial(long n);

int main(int argc, char* argv[]) {
  int rank, size,step=1,metodoElegido=atoi(argv[3]);
    double start_time, end_time;

    double (*operacionIterativa[11])(long) = {NULL, i1, i2, i3, i4, i5, i6, i7, i8, i9, i10};
    double (*completarOperacion[11])(double) = {NULL, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10};

   

    long iteraciones_totales_montecarlo=atoi(argv[1]);

    // Inicialización del entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Obtener el identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Obtener el número total de procesos


    // Iniciar el contador de tiempo
    start_time = MPI_Wtime();

    long iteraciones_por_proceso = iteraciones_totales_montecarlo / size;
    // Cada proceso realiza una parte de las iteraciones
    double local_data  = operacionIterativa[metodoElegido](iteraciones_por_proceso);

    // Recopilar resultados parciales de cada proceso
    while (step < size) {
        if (rank % (2 * step) == 0) {
            // Los procesos en posiciones múltiplos de 2*step reciben datos
            if (rank + step < size) {
                double received_data;
                MPI_Recv(&received_data, 1, MPI_DOUBLE, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_data += received_data;  // Aquí puedes combinar datos o almacenarlos en un array
            }
        } else {
            // Los otros procesos envían sus datos y salen del bucle
            int target = rank - step;
            MPI_Send(&local_data, 1, MPI_DOUBLE, target, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;  // Aumentar el rango de la distancia en el árbol binario
    }



    if (rank == 0) {
        // Calcular el valor de pi a partir de la integral total
        local_data /= size;
        double pi = completarOperacion[metodoElegido](local_data);

        // Finalizar el contador de tiempo
        end_time = MPI_Wtime();
        double cpu_time_used = end_time - start_time;

        // Imprimir el resultado
        //printf("El valor aproximado de π es: %.15f\n", pi);
        //printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
	FILE* salida = fopen(argv[2],"a");
	fprintf(salida,"%d %ld %lf %lf %d\n",size,iteraciones_totales_montecarlo, pi, cpu_time_used, metodoElegido);
    }

    // Finalización del entorno MPI
    MPI_Finalize();

    return 0;
}


double f1(double local_data){
    return local_data*local_data;
}

double i1(long iter){
    srand(getpid());
    long aciertos=0;

    for(long i=0;i<iter;i++){
        double x = (double)rand() - RAND_MAX/2;
        double y = (double)rand() - RAND_MAX/2;

        if(pow(numeroE,-(x*x))>y) aciertos++;
    }
    return (double)aciertos*RAND_MAX/iter;
}

double f2(double local_data){
    return 4.0*local_data;
}

double i2(long iter){
    srand(getpid());  // Semilla diferente para cada proceso
    long aciertos = 0;

    for (long i = 0; i < iter; i++) {
        // Generar un número aleatorio entre 0 y 1
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (sqrt(1.0-x*x)>y) aciertos++;
    }
    return (double)aciertos/iter;
}

double f3(double local_data){
    return local_data;
}

double i3(long iter){
    srand(getpid());  // Semilla diferente para cada proceso
    long aciertos = 0;

    for (long i = 0; i < iter; i++) {
        // Generar un número aleatorio entre -1 y 1
        double x = ((float)rand() / RAND_MAX) * 2.0 - 1.0;
        double y = ((float)rand() / RAND_MAX) * 2.0 - 1.0;

        if (1.0/sqrt(1-x*x) > y) aciertos++;
    }
    return (double)aciertos*2/iter;
}

double f4(double local_data){
    return 2.0*local_data;
}

double i4(long iter){
    double termino=1;
    for (long i = 1; i <= iter; i++) {
        termino*=(((2.0 * i)/(2.0*i-1))*((2.0 * i)/(2.0*i+1)));
    }
    return termino;
}

double f5(double local_data){
    return sqrt(local_data);
}

double i5(long iter){
    double res=0.0;
    for(long i=0;i<iter;i++){
        res+=6.0/(i*i);
    }
    return res;
}

double f6(double local_data){
    return local_data;
}

double i6(long iter){
    double res=0.0;
    for (int i = 0; i < iter; i++) {
        res += (1.0 / pow(16, i)) *
               (4.0 / (8 * i + 1) -
                2.0 / (8 * i + 4) -
                1.0 / (8 * i + 5) -
                1.0 / (8 * i + 6));
    }
    return res;
}

double f7(double local_data){
    return (double)22/7-local_data;
}

double i7(long iter) {
    srand(getpid());  // Semilla diferente para cada proceso
    long aciertos = 0;

    for (long i = 0; i < iter; i++) {
        // Generar un número aleatorio entre 0 y 1
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (((x * x * x * x) * ((1 - x) * (1 - x) * (1 - x) * (1 - x))) / ((1 + x) * (1 + x)) > y) aciertos++;
    }

    // La integral es la proporción de aciertos respecto al número de iteraciones
    return (double)aciertos/iter;
}

double f8(double local_data){
    return 9801.0/sqrt(8)*local_data;
}

long  factorial(long n) {
    long resultado = 1;
    for (int i = 1; i <= n; i++) {
        resultado *= i;
    }
    return resultado;
}

double i8(long iter){
    double suma = 0.0;
    for (int k = 0; k < iter; k++) {
        long num_factorial = factorial(4 * k);
        long den_factorial = factorial(k);

        // Cambiamos el orden para dividir antes de multiplicar
        double termino = (1103 + 26390 * k) / (double)(pow(396, 4 * k));
        termino *= num_factorial / (double)(pow(den_factorial, 4));
        suma += termino;
    }

    return suma;
}

double f9(double local_data){
    return 4.0*local_data;
}

double i9(long iter){
    double suma = 0.0;

    for (int i = 1; i <= iter; i++) {
        // Calculamos cada término de la serie
        double termino = (i % 2 == 0 ? -1.0 : 1.0) / (2 * i - 1);
        suma += termino;
    }
    return suma;
}

double f10(double local_data){
    return local_data/(double)3;
}

double i10(long iter){
    srand(getpid());  // Semilla diferente para cada proceso
    long aciertos = 0;

    for (long i = 0; i < iter; i++) {
        // Generar un número aleatorio entre -3 y 3
        double x = ((float)rand() / RAND_MAX) * 6.0 - 3.0;
        double y = ((float)rand() / RAND_MAX) * 6.0 - 3.0;

        if (((x+3)/ (sqrt(9-x*x))) > y) aciertos++;
    }
    // La integral es la proporción de aciertos respecto al número de iteraciones
    return (double)aciertos*6.0/iter;
}






