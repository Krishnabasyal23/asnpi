#include "traffic.h"
#include "semaphore.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <rpi_gpio.h>

// Define GPIO pins
#define SENSOR_PIN 17
#define RED_LED 22
#define GREEN_LED 27
#define YELLOW_LED 5

// Initialize traffic LEDs
static int leds_init(void)
{
    if (rpi_gpio_setup(RED_LED, GPIO_OUT) != 0)
        return -1;
    if (rpi_gpio_setup(GREEN_LED, GPIO_OUT) != 0)
        return -1;
    if (rpi_gpio_setup(YELLOW_LED, GPIO_OUT) != 0)
        return -1;
    return 0;
}

// Helper functions to set lights
static void set_red(void)
{
    rpi_gpio_output(RED_LED, GPIO_HIGH);
    rpi_gpio_output(GREEN_LED, GPIO_LOW);
    rpi_gpio_output(YELLOW_LED, GPIO_LOW);
}

static void set_green(void)
{
    rpi_gpio_output(RED_LED, GPIO_LOW);
    rpi_gpio_output(GREEN_LED, GPIO_HIGH);
    rpi_gpio_output(YELLOW_LED, GPIO_LOW);
}

static void set_yellow(void)
{
    rpi_gpio_output(RED_LED, GPIO_LOW);
    rpi_gpio_output(GREEN_LED, GPIO_LOW);
    rpi_gpio_output(YELLOW_LED, GPIO_HIGH);
}

// Traffic light thread
void* traffic_light_thread(void* arg)
{
    (void)arg; // suppress unused warning

    if (leds_init() != 0)
    {
        perror("[Traffic] LED init failed");
        return NULL;
    }

    printf("[Traffic] Task started\n");

    while (1)
    {
        unsigned sensor_level;
        int ret;

        // Read the sensor pin
        ret = rpi_gpio_input(SENSOR_PIN, &sensor_level);
        if (ret < 0)
        {
            printf("[Traffic] Error reading sensor pin %d\n", SENSOR_PIN);
            sensor_level = 0; // safe default
        }

        // Decide which LED to turn on based on sensor
        if (sensor_level)
        {
            set_green();
            printf("[Traffic] Sensor HIGH: Green light\n");
        }
        else
        {
            set_yellow();
            printf("[Traffic] Sensor LOW: Yellow light\n");
        }

        // Wait before next cycle
        sleep(2);
    }

    return NULL;
}