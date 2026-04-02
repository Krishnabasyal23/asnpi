
#include <pthread.h>
#include "sensor.h"
#include "traffic.h"
#include "semaphore.h"
#include <rpi_gpio.h>
#include <stdio.h>

semaphore_t sem; // global; safely share sensor states between threads
// global sensor states
volatile unsigned pir_state = GPIO_LOW;
volatile unsigned light_state = GPIO_LOW;

int main() {
    pthread_t t1, t2, t3;

    sem_init_custom(&sem, 1);

    // Setup GPIO pins, led and buzzer as output
    rpi_gpio_setup(5, GPIO_OUT);    // RED
    rpi_gpio_setup(16, GPIO_OUT);   // YELLOW
    rpi_gpio_setup(18, GPIO_OUT);   // GREEN
    rpi_gpio_setup(24, GPIO_OUT);   // BUZZER
    rpi_gpio_setup(22, GPIO_IN);    // PIR
    rpi_gpio_setup(23, GPIO_IN);    // LIGHT

    // Start threads
    pthread_create(&t1, NULL, pir_sensor_thread, NULL);
    pthread_create(&t2, NULL, light_sensor_thread, NULL);
    pthread_create(&t3, NULL, traffic_light_thread, NULL);
// Wait for threads to finish (they won't, but this keeps main alive)
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    return 0;
}