/*Improved sensor logger with dynamic memory management and alarms
>Author: Evara
> Dynamic allocation (so buffer size is flexible)
>Memory usage monitoring
>Alarm thresholds (both sensor and memory(warn if memory is near capacity))
>Overflow protection
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MEMORY_WARNING_THRESHOLD 0.8  // 80% usage warning

// -------- Sensor simulation structure ----------
typedef struct {
    float temperature;
    float pressure;
    float flow;
} SensorData;

// -------- Circular buffer structure ----------
typedef struct {
    SensorData *buffer; // dynamically allocated
    int capacity;
    int head;
    int count;
} CircularBuffer;

// -------- Function prototypes ----------
void initBuffer(CircularBuffer *cb, int capacity);
void freeBuffer(CircularBuffer *cb);
void addReading(CircularBuffer *cb, SensorData data);
void displayBuffer(const CircularBuffer *cb);
SensorData simulateSensor(void);

// -------- Sensor simulation ----------
SensorData simulateSensor(void) {
    SensorData d;
    d.temperature = 60.0 + (rand() % 50); // 60–110°C
    d.pressure = 5.0 + (rand() % 10);     // 5–15 bar
    d.flow = 20.0 + (rand() % 30);        // 20–50 L/min
    return d;
}

// -------- Buffer initialization ----------
void initBuffer(CircularBuffer *cb, int capacity) {
    cb->buffer = (SensorData *)malloc(sizeof(SensorData) * capacity);
    if (!cb->buffer) {
        printf("[ERROR] Memory allocation failed.\n");
        exit(1);
    }
    cb->capacity = capacity;
    cb->head = 0;
    cb->count = 0;
}

// -------- Free memory ----------
void freeBuffer(CircularBuffer *cb) {
    free(cb->buffer);
    cb->buffer = NULL;
    cb->capacity = 0;
    cb->count = 0;
    cb->head = 0;
}

// -------- Add data to buffer ----------
void addReading(CircularBuffer *cb, SensorData data) {
    // Handle overflow — overwrite oldest
    if (cb->count < cb->capacity) {
        cb->buffer[(cb->head + cb->count) % cb->capacity] = data;
        cb->count++;
    } else {
        printf("[WARN] Buffer full, overwriting oldest data.\n");
        cb->buffer[cb->head] = data;
        cb->head = (cb->head + 1) % cb->capacity;
    }

    // Check memory usage
    float usage = (float)cb->count / cb->capacity;
    if (usage >= MEMORY_WARNING_THRESHOLD) {
        printf("[ALERT] Memory usage high: %.0f%% used.\n", usage * 100);
    }

    // Sensor alarms
    if (data.temperature > 80.0)
        printf("[TEMP ALARM] Temperature %.2f°C exceeds limit!\n", data.temperature);
    if (data.pressure > 9.0)
        printf("[PRESSURE ALARM] Pressure %.2f bar exceeds limit!\n", data.pressure);
    if (data.flow > 45.0)
        printf("[FLOW ALARM] Flow %.2f L/min exceeds limit!\n", data.flow);
}

// -------- Display current buffer contents ----------
void displayBuffer(const CircularBuffer *cb) {
    printf("\n---- Buffer Contents (%d/%d) ----\n", cb->count, cb->capacity);
    for (int i = 0; i < cb->count; i++) {
        int index = (cb->head + i) % cb->capacity;
        SensorData d = cb->buffer[index];
        printf("[%02d] T=%.1f°C  P=%.1f bar  F=%.1f L/min\n", i + 1, d.temperature, d.pressure, d.flow);
    }
    printf("-------------------------------\n");
}

// -------- Main program ----------
int main() {
    srand(time(NULL));

    CircularBuffer cb;
    int numReadings;

    printf("Enter number of readings to simulate: ");
    scanf("%d", &numReadings);

    initBuffer(&cb, 10); // 10-sample buffer

    for (int i = 0; i < numReadings; i++) {
        SensorData d = simulateSensor();
        addReading(&cb, d);
    }

    displayBuffer(&cb);
    freeBuffer(&cb);

    return 0;
}
