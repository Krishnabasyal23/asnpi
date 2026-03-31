#include "sensor.h"
#include "semaphore.h"
#include <rpi_gpio.h>
#include <stdio.h>
#include <unistd.h>

#define SENSOR_PIN 6

extern semaphore_t sem;

void* sensor_thread(void* arg)
{
    (void)arg; // avoid unused parameter warning

    unsigned level;
    int ret;

    while (1)
    {
        ret = rpi_gpio_input(SENSOR_PIN, &level);
        if (ret < 0)
        {
            printf("[Sensor] Error reading pin %d\n", SENSOR_PIN);
            level = 0; // safe default
        }

        // Use level (0 = LOW, 1 = HIGH)
        if (level)
        {
            printf("[Sensor] Pin HIGH\n");
        }
        else
        {
            printf("[Sensor] Pin LOW\n");
        }

        usleep(100000); // 100ms delay
    }

    return NULL;
}