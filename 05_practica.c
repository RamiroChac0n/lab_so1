#include <stdio.h>
#include <pthread.h>
#include <string.h> // Para memset

#define MAX 1000
#define NUM_HILOS 5

typedef struct {
    int inicio;
    int fin;
} Rango;

int primos[MAX]; // Arreglo global para marcar los primos

int es_primo(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

void* buscar_primos(void* arg) {
    Rango* rango = (Rango*) arg;

    for (int i = rango->inicio; i <= rango->fin; i++) {
        if (es_primo(i)) {
            primos[i] = 1; // Marcar como primo
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_HILOS];
    Rango rangos[NUM_HILOS];

    memset(primos, 0, sizeof(primos)); // Inicializar el arreglo con 0

    int intervalo = MAX / NUM_HILOS;

    for (int i = 0; i < NUM_HILOS; i++) {
        rangos[i].inicio = i * intervalo + 1;
        rangos[i].fin = (i == NUM_HILOS - 1) ? MAX : (i + 1) * intervalo;
        pthread_create(&hilos[i], NULL, buscar_primos, (void*)&rangos[i]);
    }

    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Imprimir el resultado con la posición y el valor
    printf("Posición -> Valor\n");
    for (int i = 1; i < MAX; i++) {
        printf("%d -> %d\n", i, primos[i]);
    }

    return 0;
}

