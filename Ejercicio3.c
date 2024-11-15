#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex_A, mutex_B; // mutex para bases de datos

void *thread_H1(void *arg) {
    pthread_mutex_lock(&mutex_A); // acceso exclusivo a A
    printf("Hilo H1 (ID %lu) accediendo a BDD A\n", pthread_self());
    sleep(rand() % 3); // simulación de uso
    printf("Hilo H1 (ID %lu) libera BDD A\n", pthread_self());
    pthread_mutex_unlock(&mutex_A); // liberar A
    pthread_exit(NULL);
}

void *thread_H2(void *arg) {
    pthread_mutex_lock(&mutex_B); // acceso exclusivo a B
    printf("Hilo H2 (ID %lu) accediendo a BDD B\n", pthread_self());
    sleep(rand() % 3); // simulación de uso
    printf("Hilo H2 (ID %lu) libera BDD B\n", pthread_self());
    pthread_mutex_unlock(&mutex_B); // liberar B
    pthread_exit(NULL);
}

void *thread_H3_H4(void *arg) {
    pthread_mutex_lock(&mutex_A); // acceso exclusivo a A y B
    pthread_mutex_lock(&mutex_B);
    printf("Hilo H3/H4 (ID %lu) accediendo a BDD A y B\n", pthread_self());
    sleep(rand() % 3); // simulación de uso
    printf("Hilo H3/H4 (ID %lu) libera BDD A y B\n", pthread_self());
    pthread_mutex_unlock(&mutex_B); // liberar B
    pthread_mutex_unlock(&mutex_A); // liberar A
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[40]; // hilos
    pthread_mutex_init(&mutex_A, NULL); // inicializar mutex A
    pthread_mutex_init(&mutex_B, NULL); // inicializar mutex B

    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, thread_H1, NULL); // H1
        pthread_create(&threads[i + 10], NULL, thread_H2, NULL); // H2
        pthread_create(&threads[i + 20], NULL, thread_H3_H4, NULL); // H3
        pthread_create(&threads[i + 30], NULL, thread_H3_H4, NULL); // H4
    }

    for (int i = 0; i < 40; i++)
        pthread_join(threads[i], NULL); // esperar a todos los hilos

    pthread_mutex_destroy(&mutex_A); // destruir mutex A
    pthread_mutex_destroy(&mutex_B); // destruir mutex B

    return 0;
}
