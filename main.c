#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "traffic.h"
#include "sensor.h"
#include "semaphore.h"
#include <rpi_gpio.h>

semaphore_t sem;
volatile unsigned sensor_state = GPIO_LOW;

void init_gpio() {
    rpi_gpio_setup(5, GPIO_OUT);    // RED
    rpi_gpio_setup(16, GPIO_OUT);   // YELLOW
    rpi_gpio_setup(18, GPIO_OUT);   // GREEN
    rpi_gpio_setup(22, GPIO_IN);    // SENSOR
}

int main() {
    pthread_t t1, t2;

    sem_init_custom(&sem, 1);
    init_gpio();

    pthread_create(&t1, NULL, sensor_thread, NULL);
    pthread_create(&t2, NULL, traffic_light_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}