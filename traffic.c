#include "traffic.h"
#include "semaphore.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <rpi_gpio.h>

#define RED_LED 5
#define YELLOW_LED 16
#define GREEN_LED 18
#define BUZZER_PIN 24

extern semaphore_t sem;
extern volatile unsigned sensor_state;

static int outputs_init(void)
{
    if (rpi_gpio_setup(RED_LED, GPIO_OUT) != 0) return -1;
    if (rpi_gpio_setup(YELLOW_LED, GPIO_OUT) != 0) return -1;
    if (rpi_gpio_setup(GREEN_LED, GPIO_OUT) != 0) return -1;
    if (rpi_gpio_setup(BUZZER_PIN, GPIO_OUT) != 0) return -1;
    return 0;
}

static void set_yellow(void)
{
    rpi_gpio_output(RED_LED, GPIO_LOW);
    rpi_gpio_output(YELLOW_LED, GPIO_HIGH);
    rpi_gpio_output(GREEN_LED, GPIO_LOW);
}

static void set_green(void)
{
    rpi_gpio_output(RED_LED, GPIO_LOW);
    rpi_gpio_output(YELLOW_LED, GPIO_LOW);
    rpi_gpio_output(GREEN_LED, GPIO_HIGH);
}

void* traffic_light_thread(void* arg)
{
    (void)arg;

    if (outputs_init() != 0)
    {
        perror("[Traffic] Output init failed");
        return NULL;
    }

    printf("[Traffic] Thread started\n");

    unsigned current_state = GPIO_LOW;
    unsigned last_state = (unsigned)-1;

    set_yellow();
    rpi_gpio_output(BUZZER_PIN, GPIO_LOW);
    printf("[Traffic] No motion -> YELLOW\n");

    while (1)
    {
        sem_wait_custom(&sem);
        current_state = sensor_state;
        sem_post_custom(&sem);

        if (current_state != last_state)
        {
            if (current_state == GPIO_HIGH)
            {
                set_green();
                printf("[Traffic] Motion -> GREEN\n");

                rpi_gpio_output(BUZZER_PIN, GPIO_HIGH);
                usleep(200000);   // short beep
                rpi_gpio_output(BUZZER_PIN, GPIO_LOW);
            }
            else
            {
                set_yellow();
                printf("[Traffic] No motion -> YELLOW\n");
            }

            last_state = current_state;
        }

        usleep(100000);
    }

    return NULL;
}