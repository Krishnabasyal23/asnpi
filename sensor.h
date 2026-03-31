#ifndef SENSOR_H
#define SENSOR_H

#include <pthread.h>

void* sensor_thread(void* arg);
int grove_read_sensor(char *buf, int maxlen);

#endif