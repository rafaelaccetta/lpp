#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define TAM 100000

int main(int argc, char *argv[]) {
    int rank, size;
    int x, op;
    double soma = 0.0, subtracao = 0.0, mult = 1.0;
    double vet[TAM];
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
        
    start_time = MPI_Wtime();

    if (size != 4) {
        if (rank == 0) {
            printf("O programa deve ser executado apenas com 4 processos.\n");
        }
        MPI_Finalize();
        exit(0);
    }
    
    if (rank == 0) {
        for (x = 0; x < TAM; x++) {
            vet[x] = (((x%5)*0.001) + 1.0);
        }

        for(x=1;x<4;x++){
            MPI_Send(vet, TAM, MPI_DOUBLE, x, 1, MPI_COMM_WORLD);
            op = x;
            MPI_Send(&op, 1, MPI_INT, x, 2, MPI_COMM_WORLD);
        }

        MPI_Recv(&soma, 1, MPI_DOUBLE, 1, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&subtracao, 2, MPI_DOUBLE, 2, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&mult, 1, MPI_DOUBLE, 3, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        end_time = MPI_Wtime();

        printf("\n=== RESULTADOS PARA %d ELEMENTOS ===\n", TAM);
        printf("Soma = %f\n", soma);
        printf("Subtracao = %f\n", subtracao);
        printf("Multiplicacao = %f\n", mult);
        printf("Tempo paralelo (%d processos): %f segundos\n", size, end_time - start_time);
       
    } else {
        MPI_Recv(vet, TAM, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&op, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (op == 1){
            for(x=0;x<TAM;x++)
                soma = soma + vet[x];
            MPI_Send(&soma, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
        } else if (op == 2) {
            for(x=0;x<TAM;x++)
                subtracao = subtracao - vet[x];
            MPI_Send(&subtracao, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
        } else {
            for(x=0;x<TAM;x++)
                mult = mult * vet[x];
            MPI_Send(&mult, 1, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
        }
    }
    
    MPI_Finalize();
    return 0;
}