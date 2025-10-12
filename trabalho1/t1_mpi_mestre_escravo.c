#include <stdio.h>
#include<stdlib.h>
#include <mpi.h>
#include <time.h>

#define TAM 100000

int main(int argc, char *argv[]) {
    int rank, size;
    int x;
    double soma = 0.0, subtracao = 0.0, mult = 1.0;
    double soma_local = 0.0, subtracao_local = 0.0, mult_local = 1.0;
    double vet[TAM];
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    start_time = MPI_Wtime();

    if(size < 2){
        if (rank == 0) {
            printf("O programa deve ser executado com pelo menos 2 processos.\n");
        }
        MPI_Finalize();
        exit(0);
    }
    
    if (rank == 0) {
        for (x = 0; x < TAM; x++) {
            vet[x] = (((x%5)*0.001) + 1.0);
        }
        MPI_Bcast(vet, TAM, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        int inicio = 0, fim = 0;
        int elementos_por_processo = TAM / (size - 1);
        int resto = TAM % (size - 1);

        for(x=1;x<size;x++){
            inicio = fim;
            fim = inicio + elementos_por_processo + (x <= resto ? 1 : 0);
            MPI_Send(&inicio, 1, MPI_INT, x, 1, MPI_COMM_WORLD);
            MPI_Send(&fim, 1, MPI_INT, x, 2, MPI_COMM_WORLD);
        }

        MPI_Reduce(&soma_local, &soma, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&subtracao_local, &subtracao, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&mult_local, &mult, 1, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);    

        end_time = MPI_Wtime();

        printf("\n=== RESULTADOS PARA %d ELEMENTOS ===\n", TAM);
        printf("Soma = %f\n", soma);
        printf("Subtracao = %f\n", subtracao);
        printf("Multiplicacao = %f\n", mult);
        printf("Tempo paralelo (%d processos): %f segundos\n", size, end_time - start_time);
    } else {
        MPI_Bcast(vet, TAM, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        int inicio, fim;
        MPI_Recv(&inicio, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&fim, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //printf("i %d f %d \n", inicio, fim);
        
        for(x=inicio;x<fim;x++){
            soma_local = soma_local + vet[x];
            subtracao_local = subtracao_local - vet[x];
            mult_local = mult_local * vet[x];
        }

        MPI_Reduce(&soma_local, &soma, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&subtracao_local, &subtracao, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&mult_local, &mult, 1, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}
