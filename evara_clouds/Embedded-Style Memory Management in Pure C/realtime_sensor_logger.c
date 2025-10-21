/*
>Author: Evara
>Title: Real-Time Sensor Data Logger with Embedded-Style Memory Management in C
>Description: This program simulates real-time sensor data logging with embedded-style memory management in C.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // for sleep()

#define MEMORY_WARNING_THRESHOLD 0.8  // 80%
#define BUFFER_CAPACITY 10

typedef struct {
    float temperature;
    float pressure;
    float flow;
} SensorData;

typedef struct {
    SensorData *buffer;
    int capacity;
    int head;
    int count;
} CircularBuffer;

void initBuffer(CircularBuffer *cb, int capacity) {
    cb->buffer = malloc(sizeof(SensorData) * capacity);
    if (!cb->buffer) {
        printf("[ERROR] Memory allocation failed.\n");
        exit(1);
    }
    cb->capacity = capacity;
    cb->head = 0;
    cb->count = 0;
}

void freeBuffer(CircularBuffer *cb) {
    free(cb->buffer);
    cb->buffer = NULL;
}

SensorData simulateSensor() {
    SensorData d;
    d.temperature = 60.0 + (rand() % 50);
    d.pressure = 5.0 + (rand() % 10);
    d.flow = 20.0 + (rand() % 30);
    return d;
}

void addReading(CircularBuffer *cb, SensorData d) {
    if (cb->count < cb->capacity) {
        cb->buffer[(cb->head + cb->count) % cb->capacity] = d;
        cb->count++;
    } else {
        cb->buffer[cb->head] = d;
        cb->head = (cb->head + 1) % cb->capacity;
    }

    float usage = (float)cb->count / cb->capacity;
    if (usage >= MEMORY_WARNING_THRESHOLD)
        printf("[ALERT] Memory usage high: %.0f%%\n", usage * 100);

    if (d.temperature > 80.0)
        printf("[TEMP ALARM] %.2f°C exceeds limit!\n", d.temperature);
    if (d.pressure > 9.0)
        printf("[PRESSURE ALARM] %.2f bar exceeds limit!\n", d.pressure);
    if (d.flow > 45.0)
        printf("[FLOW ALARM] %.2f L/min exceeds limit!\n", d.flow);
}

void displayLastReading(const SensorData *d, int index) {
    printf("[%02d] T=%.1f°C  P=%.1f bar  F=%.1f L/min\n",
           index, d->temperature, d->pressure, d->flow);
}

int main() {
    srand(time(NULL));

    CircularBuffer cb;
    initBuffer(&cb, BUFFER_CAPACITY);

    printf("Starting real-time sensor simulation... (Ctrl+C to stop)\n");

    int readingCount = 0;
    while (1) {
        SensorData d = simulateSensor();
        addReading(&cb, d);
        readingCount++;

        printf("\n--- Reading #%d ---\n", readingCount);
        displayLastReading(&d, readingCount);
        printf("Buffer usage: %d/%d\n", cb.count, cb.capacity);

        sleep(1); // wait 1 second between readings
    }

    freeBuffer(&cb);
    return 0;
}
