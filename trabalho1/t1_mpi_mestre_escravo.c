#include <stdio.h>
#include <mpi.h>
#include <time.h>

#define TAM 1000000
#define VETOR_SIZE 1000000

int main(int argc, char *argv[]) {
    int rank, size;
    int x, vet[TAM];
    long long soma_local = 0, subtracao_local = 0;
    double mult_local = 1.0;  
    long long soma_total, subtracao_total;
    double mult_total;
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    start_time = MPI_Wtime();
    
    if (rank == 0) {
        for (x = 0; x < VETOR_SIZE; x++) {
            vet[x] = (x % 1000) + 1;  
        }
    }
    
    MPI_Bcast(vet, VETOR_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    
    int elementos_por_processo = VETOR_SIZE / size;
    int resto = VETOR_SIZE % size;
    
    int inicio = 0;
    int fim = 0;
    
    for (int i = 0; i < size; i++) {
        int elementos = elementos_por_processo + (i < resto ? 1 : 0);
        if (i == rank) {
            inicio = fim;
            fim = inicio + elementos;
            break;
        }
        fim += elementos;
    }
    
    for (x = inicio; x < fim; x++) {
        soma_local += vet[x];
        subtracao_local -= vet[x];
        mult_local *= vet[x];
        
        if (mult_local > 1e100 || mult_local < 1e-100) {
            mult_local = 1.0;  
        }
    }
    
    MPI_Reduce(&soma_local, &soma_total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&subtracao_local, &subtracao_total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    
    MPI_Reduce(&mult_local, &mult_total, 1, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);
    
    end_time = MPI_Wtime();
    
    if (rank == 0) {
        double tempo_sequencial_estimado = 0.008410; // informação de tempo para calculo de metricas
        double tempo_paralelo = end_time - start_time;
        double speed_up = tempo_sequencial_estimado / tempo_paralelo;
        double eficiencia = (speed_up / size) * 100;
        
        printf("=== VERSÃO MESTRE/ESCRAVO - %d PROCESSOS ===\n", size);
        printf("Soma = %lld\n", soma_total);
        printf("Subtracao = %lld\n", subtracao_total);
        printf("Multiplicacao = %e\n", mult_total);
        printf("Tempo paralelo: %f segundos\n", tempo_paralelo);
        printf("Speed Up: %.2f\n", speed_up);
        printf("Eficiencia: %.1f%%\n", eficiencia);
        printf("Tempo sequencial estimado: %f segundos\n", tempo_sequencial_estimado);
    }
    
    MPI_Finalize();
    return 0;
}