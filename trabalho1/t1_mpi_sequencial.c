#include <stdio.h>
#include <time.h>

#define TAM 100000

int main() {
    int x;
    double soma = 0, subtracao = 0, mult = 1.0;  
    double vet[TAM];
    clock_t start_time, end_time;
        
    for (x=0;x<TAM;x++) {
        vet[x] = (((x%5)*0.001) + 1.0);
    }

    start_time = clock();

    for (x=0;x<TAM;x++) {
        soma = soma + vet[x];
        subtracao = subtracao - vet[x];
        mult = mult * vet[x];
    }
    
    end_time = clock();
        
    printf("=== SEQUENCIAL COM %d ELEMENTOS ===\n", TAM);
    printf("Soma = %f\n", soma);
    printf("Subtracao = %f\n", subtracao);
    printf("Multiplicacao = %f\n", mult);
    printf("Tempo sequencial: %f segundos\n", (double) (end_time - start_time) / CLOCKS_PER_SEC);
    
    return 0;
}