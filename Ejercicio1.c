#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int start, end, num_threads; // variables globales para los rangos y nº de hilos
pthread_mutex_t mutex; // mutex para proteger acceso a datos compartidos
pthread_cond_t *conditions; // array de variables de condición
int current_thread = 0; // id del hilo actual que puede imprimir

void *print_numbers(void *arg) {
    int thread_id = *(int *)arg; // id del hilo
    free(arg); // liberar memoria asignada para el id

    for (int i = start + thread_id; i <= end; i += num_threads) {
        pthread_mutex_lock(&mutex); // bloquear acceso compartido

        while (current_thread != thread_id) // esperar su turno
            pthread_cond_wait(&conditions[thread_id], &mutex);

        printf("Hilo %d imprime %d\n", thread_id, i); // imprimir número

        current_thread = (current_thread + 1) % num_threads; // pasar turno
        pthread_cond_signal(&conditions[current_thread]); // notificar al siguiente hilo

        pthread_mutex_unlock(&mutex); // liberar mutex
    }

    pthread_exit(NULL); // finalizar hilo
}

int main(int argc, char *argv[]) {
    if (argc != 4) { // validar argumentos
        printf("Uso: %s <inicio> <fin> <nº_hilos>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    start = atoi(argv[1]);
    end = atoi(argv[2]);
    num_threads = atoi(argv[3]);

    if (end - start + 1 < num_threads) { // validar rango vs nº hilos
        printf("Error: rango insuficiente para el nº de hilos.\n");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[num_threads]; // array para los hilos
    conditions = malloc(num_threads * sizeof(pthread_cond_t)); // inicializar condiciones

    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < num_threads; i++)
        pthread_cond_init(&conditions[i], NULL);

    for (int i = 0; i < num_threads; i++) {
        int *id = malloc(sizeof(int)); // reservar memoria para id del hilo
        *id = i;
        pthread_create(&threads[i], NULL, print_numbers, id); // crear hilo
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL); // esperar a que terminen los hilos

    for (int i = 0; i < num_threads; i++)
        pthread_cond_destroy(&conditions[i]); // destruir condiciones
    pthread_mutex_destroy(&mutex); // destruir mutex
    free(conditions); // liberar memoria

    printf("Hilo principal: todos los hilos finalizaron.\n");
    return 0;
}
