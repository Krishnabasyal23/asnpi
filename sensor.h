
#ifndef SENSOR_H
#define SENSOR_H
#include <pthread.h>

void* pir_sensor_thread(void* arg);
void* light_sensor_thread(void* arg);

#endif