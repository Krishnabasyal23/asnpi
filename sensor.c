#include "sensor.h"
#include "semaphore.h"
#include <rpi_gpio.h>
#include <stdio.h>
#include <unistd.h>

#define SENSOR_PIN 22

extern semaphore_t sem;
extern volatile unsigned sensor_state;

void* sensor_thread(void* arg)
{
    (void)arg;

    unsigned level = GPIO_LOW;
    unsigned last_level = (unsigned)-1;
    int ret;

    printf("[Sensor] Thread started on pin %d\n", SENSOR_PIN);
    printf("[Sensor] Waiting for PIR to stabilize...\n");
    sleep(5);

    while (1)
    {
        ret = rpi_gpio_input(SENSOR_PIN, &level);
        if (ret < 0)
        {
            printf("[Sensor] Error reading pin %d\n", SENSOR_PIN);
            level = GPIO_LOW;
        }

        sem_wait_custom(&sem);
        sensor_state = level;
        sem_post_custom(&sem);

        if (level != last_level)
        {
            if (level == GPIO_HIGH)
                printf("[Sensor] Motion detected\n");
            else
                printf("[Sensor] No motion\n");

            printf("[Sensor] level=%u\n", level);
            last_level = level;
        }

        usleep(100000);
    }

    return NULL;
}