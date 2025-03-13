#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define LENGTH 1000
int numeros_p[LENGTH];

typedef struct Limite
{
    int limInf;
    int limSup;
}Limite;


void *rutina(void *arg){
    int *contador = 0;

    for(int i = limInf; i <= limSup; i++){
        if(i % 2 == 0){
            contador++;
        }
    }
}

int main(){

    int n_hilos = 5;
    pthread_t hilos[n_hilos];

    int l_segmento = LENGTH / n_hilos;

    // ARREGLO DE REFERENCIA
    for (int i = 0; i < LENGTH; i++)
    {
        numeros_p[i] = 0;
    }
    
    // CREA LOS HILOS
    for(int i = 0; i < n_hilos; i++){
        Limite *l = malloc(sizeof(Limite));
        l->limInf = i * l_segmento + 1;
        l->limSup = (i + 1) * l_segmento;

        if(pthread_create(&hilos[i], NULL, &rutina, (void*)l ) != 0){
        }
    }

    return 0;
}