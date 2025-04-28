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
#define NOMBRE_ARCHIVO "primosPOSIX"

// Función para verificar si un número es primo
int es_primo(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main() {

    // Inicializa semáforos y memoria compartida
    sem_t *sem_productor, *sem_consumidor;
    int *memoria_compartida;
    int fd_memoria;

    // Crea los semáforos
    sem_productor = sem_open(NOMBRE_SEMAFORO_PRODUCTOR, O_CREAT | O_EXCL, 0666, 1);
    sem_consumidor = sem_open(NOMBRE_SEMAFORO_CONSUMIDOR, O_CREAT | O_EXCL, 0666, 0);
    if (sem_productor == SEM_FAILED || sem_consumidor == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Crea la memoria compartida
    fd_memoria = shm_open(NOMBRE_MEMORIA, O_CREAT | O_RDWR, 0666);
    if (fd_memoria == -1) {
        perror("shm_open");
        sem_close(sem_productor);
        sem_close(sem_consumidor);
        sem_unlink(NOMBRE_SEMAFORO_PRODUCTOR);
        sem_unlink(NOMBRE_SEMAFORO_CONSUMIDOR);
        exit(EXIT_FAILURE);
    }

    // Configura el tamaño de la memoria compartida
    ftruncate(fd_memoria, sizeof(int));

    // Mapea la memoria compartida
    memoria_compartida = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_memoria, 0);

    // Verifica si la memoria compartida se creó correctamente
    if (memoria_compartida == MAP_FAILED) {
        perror("mmap");
        close(fd_memoria);
        sem_close(sem_productor);
        sem_close(sem_consumidor);
        sem_unlink(NOMBRE_SEMAFORO_PRODUCTOR);
        sem_unlink(NOMBRE_SEMAFORO_CONSUMIDOR);
        shm_unlink(NOMBRE_MEMORIA);
        exit(EXIT_FAILURE);
    }

    // Abre el archivo para escritura
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "w");
    if (archivo == NULL) {
        perror("fopen");
        munmap(memoria_compartida, sizeof(int));
        close(fd_memoria);
        sem_close(sem_productor);
        sem_close(sem_consumidor);
        sem_unlink(NOMBRE_SEMAFORO_PRODUCTOR);
        sem_unlink(NOMBRE_SEMAFORO_CONSUMIDOR);
        shm_unlink(NOMBRE_MEMORIA);
        exit(EXIT_FAILURE);
    }

    // Genera los números primos
    for (int i = 2; i <= 1000; i++) {
        if (es_primo(i)) {
            sem_wait(sem_productor); // Espera a que el consumidor lea
            *memoria_compartida = i; // Escribe el número primo en memoria compartida
            fprintf(archivo, "%d\n", i); // Escribe el número primo en el archivo
            fflush(archivo); // Asegura que se escriba inmediatamente
            sem_post(sem_consumidor); // Notifica al consumidor
        }
    }

    // Indica fin de producción
    sem_wait(sem_productor);
    *memoria_compartida = -1; // Señal de fin
    fprintf(archivo, "-1\n"); // Escribe la señal de fin en el archivo
    fflush(archivo);
    sem_post(sem_consumidor);

    // Libera lis recursos
    fclose(archivo);
    munmap(memoria_compartida, sizeof(int));
    close(fd_memoria);
    sem_close(sem_productor);
    sem_close(sem_consumidor);
    sem_unlink(NOMBRE_SEMAFORO_PRODUCTOR);
    sem_unlink(NOMBRE_SEMAFORO_CONSUMIDOR);
    shm_unlink(NOMBRE_MEMORIA);

    return 0;
}
