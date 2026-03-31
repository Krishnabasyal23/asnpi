#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "traffic.h"
#include "sensor.h"
#include "semaphore.h"
#include <rpi_gpio.h>

semaphore_t sem;

void init_gpio() {
    // LEDs
    rpi_gpio_setup(5, GPIO_OUT);   // RED
    rpi_gpio_setup(16, GPIO_OUT);  // YELLOW
    rpi_gpio_setup(18, GPIO_OUT);  // GREEN

    // Sensor + buzzer
    rpi_gpio_setup(6, GPIO_IN);    // SENSOR
    rpi_gpio_setup(22, GPIO_OUT);  // BUZZER
}

int main() {
    pthread_t t1, t2;

    sem_init_custom(&sem, 1);

    init_gpio();

    pthread_create(&t1, NULL, traffic_light_thread, NULL);
    pthread_create(&t2, NULL, sensor_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}