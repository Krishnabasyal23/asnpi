
#include "traffic.h"
#include "semaphore.h"
#include <rpi_gpio.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// pins for led sensors and buzzer
#define RED_LED 5
#define YELLOW_LED 16
#define GREEN_LED 18
#define BUZZER_PIN 24

extern semaphore_t sem; // Shared semaphore
extern volatile unsigned pir_state; // pir sensor state
extern volatile unsigned light_state; // light sensor state

// initial outputs
static int outputs_init(void) {
    if (rpi_gpio_setup(RED_LED, GPIO_OUT) != 0) return -1;
    if (rpi_gpio_setup(YELLOW_LED, GPIO_OUT) != 0) return -1;
    if (rpi_gpio_setup(GREEN_LED, GPIO_OUT) != 0) return -1;
    if (rpi_gpio_setup(BUZZER_PIN, GPIO_OUT) != 0) return -1;
    return 0;
}
// turn on red, others off
static void set_red(void) {
    rpi_gpio_output(RED_LED, GPIO_HIGH);
    rpi_gpio_output(YELLOW_LED, GPIO_LOW);
    rpi_gpio_output(GREEN_LED, GPIO_LOW);
}
 // yellow turns on, others off
static void set_yellow(void) {
    rpi_gpio_output(RED_LED, GPIO_LOW);
    rpi_gpio_output(YELLOW_LED, GPIO_HIGH);
    rpi_gpio_output(GREEN_LED, GPIO_LOW);
}
// turn on green, others off
static void set_green(void) {
    rpi_gpio_output(RED_LED, GPIO_LOW);
    rpi_gpio_output(YELLOW_LED, GPIO_LOW);
    rpi_gpio_output(GREEN_LED, GPIO_HIGH);
}

// Traffic control thread: responds to PIR and light sensor
// Logic: Green always ON by default. PIR triggers buzzer, yellow, red, then back to green
void* traffic_light_thread(void* arg) {
    (void)arg;

    if (outputs_init() != 0) {
        perror("[Traffic] Output init failed");
        return NULL;
    }

    printf("[Traffic] Thread started\n");

    // Default green ON
    set_green();
    rpi_gpio_output(BUZZER_PIN, GPIO_LOW);

    while (1) {
        sem_wait_custom(&sem);
        unsigned current_pir = pir_state;
        sem_post_custom(&sem);

        if (current_pir == GPIO_HIGH) {
            // Motion detected: buzz and cycle lights
            printf("[Traffic] PIR motion detected!\n");

               
            rpi_gpio_output(BUZZER_PIN, GPIO_HIGH);
            usleep(200000);
            rpi_gpio_output(BUZZER_PIN, GPIO_LOW);

            set_yellow();
            usleep(500000); // 0.5s yellow

            set_red();
            usleep(1000000); // 1s red

            set_green();
        }

        // Always ensure green is ON if no motion
        usleep(100000); // small delay to reduce CPU usage
    }

    return NULL;
}