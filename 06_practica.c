#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX 10 //Tamaño máximo del buffer en que se escribirán y leerán los valores.
#define FIN -1 //Valor para marcar el fin de la generación de aleatorios, se colocará en la última posición que acceda el productor

int buffer[MAX];
//El semáforo vacios se usará para sincronizar la producción
//El semáforo elementos sincronizará el consumo
sem_t vacios, elementos;

int generar_dato() { return random() % 256; }
int numero_aleatorio() { return random() % 100; }


/**
 * TODO: Usar mecanismos para sincronizar la producción, de manera que se ejecute
 *          siempre que haya espacios vacíos en buffer. Después de que el consumidor
 *          lee un valor, el espacio correspondiente se considera vacío.
 */
void *productor(void *p)
{
    int pos_productor = 0; //Posición en la que debe guardar el productor
    int num, dato, n;

    n = numero_aleatorio();
    printf("Productor con %d datos\n", n);
    for (num = 0; num < n; num++)
    {
        //Obtener aleatorio con generar_dato(), asignarlo a la posición que corresponde
        //Actualizar pos_productor
        dato = generar_dato();
        sem_wait(&vacios);
        buffer[pos_productor] = dato;
        printf("Productor: buffer[%d] <= %d\n", pos_productor, dato);
        pos_productor = (pos_productor + 1) % MAX;
        sem_post(&elementos);

    }

    //Usa la constante fin para marcar que se ha concluido el trabajo del productor
    sem_wait(&vacios);
    
    buffer[pos_productor] = FIN;

    sem_post(&elementos);  

    pthread_exit(NULL);
}


/**
 * TODO: Usar mecanismos para sincronizar al consumidor, de manera que lea un elemento cada
 *          que se exista al menos un elemento recién generado por el productor para leer.
 */
void *consumidor(void *p)
{
    int pos_consumidor = 0, dato, contador = 0; //pos_consumidor: posición a leer
    int continuar = 1; //Variable para controlar salida del while

    while (continuar)
    {
        dato = buffer[pos_consumidor];
        contador++;
        if(dato == FIN) { //Cuando se ha leído el marcador de fin, se debe terminar el ciclo
            continuar = 0;
        }else {
            //Actualizar pos_consumidor
            sem_wait(&elementos);
            dato = buffer[pos_consumidor];
            pos_consumidor = (pos_consumidor + 1) % MAX;
            sem_post(&vacios);

            printf("Numero aleatorio %d: buffer[%d] => %d\n", contador, pos_consumidor, dato);
            if(dato == FIN) {
                continuar = 0;
            }
        }              
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {

    //Inicializar el generador de números aleatorios
    srandom(time(NULL));

    //Definicion de hilos productor y consumidor
    pthread_t h_productor, h_consumidor;

    //Inicialización de semáforos
    // Vacios
    // Elementos
    sem_init(&vacios, 0, MAX);
    sem_init(&elementos, 0, 0);

    //Inicializar hilos
    pthread_create(&h_productor, NULL, productor, NULL);
    pthread_create(&h_consumidor, NULL, consumidor, NULL);

    //Join de los hilos
    pthread_join(h_productor, NULL);
    pthread_join(h_consumidor, NULL);

    //Destroy de semaforos
    sem_destroy(&vacios);
    sem_destroy(&elementos);

    return 0;

}