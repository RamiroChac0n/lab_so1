#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define MAX 1000
#define NUM_HILOS 5

typedef struct {
    int inicio;
    int fin;
} Rango;

int primos[MAX];
int total_primos = 0;

// Función encargada de verificar si la posición es prima o no.
int es_primo(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Función encargada de buscar los números primos en un rango.
void* buscar_primos(void* arg) {
    Rango* rango = (Rango*) arg;
    int contador = 0;

    // Busca los números primos en el rango.
    for (int i = rango->inicio; i <= rango->fin; i++) {

        // Si es primo, reemplaza el 0 por 1, sino; deja el cero.
        if (es_primo(i)) {
            primos[i] = 1;
            contador++;
        }
    }

    printf("Hilo %ld: %d números primos encontrados\n", pthread_self(), contador);

    // Suma los primos encontrados al total.
    /*
        No hay pierde, como es una suma
        no importa que la suma se haga en desorden
        jaja
    */
    total_primos += contador;

    // El hilo termina cuando llego al final del rango.
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_HILOS];
    Rango rangos[NUM_HILOS];

    // memset reserva un espacio de memoria y lo llena con 0.
    memset(primos, 0, sizeof(primos));

    // Calcula el intervalo de cada hilo.
    int intervalo = MAX / NUM_HILOS;

    // Crea los hilos con su rango correspondiente.
    for (int i = 0; i < NUM_HILOS; i++) {
        rangos[i].inicio = i * intervalo + 1;

        // Si es el último hilo se le asigna el valor máximo.
        rangos[i].fin = (i == NUM_HILOS - 1) ? MAX : (i + 1) * intervalo;

        // Crea el hilo con la función buscar_primos y el rango.
        pthread_create(&hilos[i], NULL, buscar_primos, (void*)&rangos[i]);
    }

    // Espera a que los hilos terminen.
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Total de primos.
    printf("Total de números primos encontrados: %d\n", total_primos);

    // Imprime según el arreglo.
    printf("Posición -> Valor\n");
    for (int i = 1; i < MAX; i++) {
        printf("%d -> %d\n", i, primos[i]);
    }

    return 0;
}

