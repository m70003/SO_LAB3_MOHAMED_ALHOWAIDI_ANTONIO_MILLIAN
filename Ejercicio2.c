#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5 // tamaño del buffer
#define TOTAL_NUMBERS 20 // total de números a producir/consumir

int buffer[BUFFER_SIZE]; // buffer circular
int in = 0, out = 0, count = 0; // índices y contador del buffer

pthread_mutex_t mutex; // mutex para sincronizar acceso
pthread_cond_t not_empty, not_full; // condiciones para vacío/lleno

void *produce_numbers(void *arg) {
    for (int i = 0; i < TOTAL_NUMBERS; i++) {
        pthread_mutex_lock(&mutex); // bloquear acceso al buffer

        while (count == BUFFER_SIZE) // esperar si buffer lleno
            pthread_cond_wait(&not_full, &mutex);

        buffer[in] = random() % 100; // producir número aleatorio
        printf("Productor: producido %d en posición %d\n", buffer[in], in);
        in = (in + 1) % BUFFER_SIZE; // actualizar índice
        count++; // incrementar contador

        pthread_cond_signal(&not_empty); // notificar que no está vacío
        pthread_mutex_unlock(&mutex); // liberar mutex
    }

    pthread_exit(NULL); // finalizar hilo productor
}

void *consume_numbers(void *arg) {
    int total_even = 0, total_odd = 0; // contadores

    for (int i = 0; i < TOTAL_NUMBERS; i++) {
        pthread_mutex_lock(&mutex); // bloquear acceso al buffer

        while (count == 0) // esperar si buffer vacío
            pthread_cond_wait(&not_empty, &mutex);

        int number = buffer[out]; // consumir número
        printf("Consumidor: consumido %d de posición %d\n", number, out);
        out = (out + 1) % BUFFER_SIZE; // actualizar índice
        count--; // decrementar contador

        if (number % 2 == 0) // verificar par/impar
            total_even++;
        else
            total_odd++;

        pthread_cond_signal(&not_full); // notificar que no está lleno
        pthread_mutex_unlock(&mutex); // liberar mutex
    }

    printf("Totales: pares=%d, impares=%d\n", total_even, total_odd);
    pthread_exit(NULL); // finalizar hilo consumidor
}

int main() {
    pthread_t producer, consumer; // hilos

    pthread_mutex_init(&mutex, NULL); // inicializar mutex
    pthread_cond_init(&not_empty, NULL); // inicializar condición vacío
    pthread_cond_init(&not_full, NULL); // inicializar condición lleno

    pthread_create(&producer, NULL, produce_numbers, NULL); // crear productor
    pthread_create(&consumer, NULL, consume_numbers, NULL); // crear consumidor

    pthread_join(producer, NULL); // esperar al productor
    pthread_join(consumer, NULL); // esperar al consumidor

    pthread_cond_destroy(&not_empty); // destruir condición vacío
    pthread_cond_destroy(&not_full); // destruir condición lleno
    pthread_mutex_destroy(&mutex); // destruir mutex

    return 0;
}
