#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

// Constantes para los semáforos y la memiria compartida
#define NOMBRE_SEMAFORO_PRODUCTOR "/sem_productor"
#define NOMBRE_SEMAFORO_CONSUMIDOR "/sem_consumidor"
#define NOMBRE_MEMORIA "/memoria_primos"

int main() {

    // Inicializa semáforos y memoria compartida
    sem_t *sem_productor, *sem_consumidor;
    int *memoria_compartida;
    int fd_memoria;

    // Abre los semáforos
    sem_productor = sem_open(NOMBRE_SEMAFORO_PRODUCTOR, 0);
    sem_consumidor = sem_open(NOMBRE_SEMAFORO_CONSUMIDOR, 0);
    if (sem_productor == SEM_FAILED || sem_consumidor == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Abre memoria compartida
    fd_memoria = shm_open(NOMBRE_MEMORIA, O_RDWR, 0666);
    if (fd_memoria == -1) {
        perror("shm_open");
        sem_close(sem_productor);
        sem_close(sem_consumidor);
        exit(EXIT_FAILURE);
    }

    // Configura el tamaño de la memoria compartida
    memoria_compartida = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_memoria, 0);
    if (memoria_compartida == MAP_FAILED) {
        perror("mmap");
        close(fd_memoria);
        sem_close(sem_productor);
        sem_close(sem_consumidor);
        exit(EXIT_FAILURE);
    }

    // Lee los números primos
    while (1) {
        sem_wait(sem_consumidor); // Espera al productor
        if (*memoria_compartida == -1) {
            break; // Fin de producción
        }
        printf("Consumidor: %d\n", *memoria_compartida);
        sem_post(sem_productor); // Notifica al productor
    }

    // Libera los recursos
    munmap(memoria_compartida, sizeof(int));
    close(fd_memoria);
    sem_close(sem_productor);
    sem_close(sem_consumidor);

    return 0;
}