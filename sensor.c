
#include "sensor.h"
#include "semaphore.h"
#include <rpi_gpio.h>
#include <stdio.h>
#include <unistd.h>

// gipo pins for sensors
#define PIR_PIN 22
#define LIGHT_PIN 23

extern semaphore_t sem; //Shared semaphore
extern volatile unsigned pir_state;// PIR sensor state
extern volatile unsigned light_state;

// PIR motion sensor thread
void* pir_sensor_thread(void* arg) {
    (void)arg;
    unsigned level = GPIO_LOW, last_level = (unsigned)-1;
    int ret;

    printf("[PIR] Thread started on pin %d\n", PIR_PIN);
    sleep(5); // stabilize

    while (1) {
        ret = rpi_gpio_input(PIR_PIN, &level);
        if (ret < 0) level = GPIO_LOW;

        sem_wait_custom(&sem);
        pir_state = level;
        sem_post_custom(&sem);
// on state chhange
        if (level != last_level) {
            printf("[PIR] %s\n", level == GPIO_HIGH ? "Motion detected" : "No motion");
            last_level = level;
        }
        usleep(100000);
    }
    return NULL;
}

// Light sensor thread
void* light_sensor_thread(void* arg) {
    (void)arg;
    unsigned level = GPIO_LOW, last_level = (unsigned)-1;
    int ret;

    printf("[Light] Thread started on pin %d\n", LIGHT_PIN);

    while (1) {
        ret = rpi_gpio_input(LIGHT_PIN, &level);
        if (ret < 0) level = GPIO_LOW;

        sem_wait_custom(&sem);
        light_state = level;
        sem_post_custom(&sem);
        // log on state change
        if (level != last_level) {
            printf("[Light] %s\n", level == GPIO_HIGH ? "Light detected" : "Dark");
            last_level = level;
        }
        usleep(100000);
    }
    return NULL;
}