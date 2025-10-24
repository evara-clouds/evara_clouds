/*
 * Simulated Sensor Data Logger with Circular Buffer
 * Author: Evara
 * 
 * Concept:
 *  - Simulates 3 sensors: Temperature, Pressure, and Flow.
 *  - Stores readings in a circular buffer of fixed size.
 *  - Demonstrates memory management without dynamic allocation.
 *
 * Compile with: gcc sensor_logger.c -o sensor_logger
 * Run: ./sensor_logger
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 

// ===============================
// --- Configuration Section ----
// ===============================
#define BUFFER_SIZE 10  // Circular buffer capacity
#define NUM_READINGS 25 // Total readings to simulate

// ===============================
// --- Data Structures -----------
// ===============================

// One sensor record
typedef struct {
    float temperature;    // in °C
    float pressure;       // in bar
    float flow;           // in L/min
    unsigned long timestamp; // UNIX time (seconds)
} SensorData;

// Circular buffer to store sensor readings
typedef struct {
    SensorData buffer[BUFFER_SIZE];
    int head;   // index of next write
    int tail;   // index of oldest data
    int count;  // how many items currently in buffer
    unsigned long overwrite_count;
} CircularBuffer;

// ===============================
// --- Function Prototypes -------
// ===============================
void initBuffer(CircularBuffer *cb);
void addReading(CircularBuffer *cb, SensorData data);
void printBuffer(const CircularBuffer *cb);
SensorData generateSensorData(void);

// ===============================
// --- Function Implementations ---
// ===============================

// Initialize circular buffer
void initBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->overwrite_count = 0;
}

// Add new sensor reading to buffer
void addReading(CircularBuffer *cb, SensorData data) {
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % BUFFER_SIZE;

    if (cb->count < BUFFER_SIZE) {
        cb->count++;
    } else {
        // Buffer full — overwrite oldest data
        cb->tail = (cb->tail + 1) % BUFFER_SIZE;
        cb->overwrite_count++;
    }
}

// Print all stored readings
void printBuffer(const CircularBuffer *cb) {
    printf("\n========================================\n");
    printf("BUFFER STATUS: %d/%d filled\n", cb->count, BUFFER_SIZE);
    printf("Overwrites: %lu\n", cb->overwrite_count);
    printf("----------------------------------------\n");
    printf("Index | Temperature | Pressure | Flow | Timestamp\n");
    printf("----------------------------------------\n");

    int index = cb->tail;
    for (int i = 0; i < cb->count; i++) {
        SensorData d = cb->buffer[index];
        printf("%5d | %10.2f°C | %8.2f bar | %6.2f L/min | %lu\n",
               i + 1, d.temperature, d.pressure, d.flow, d.timestamp);
        index = (index + 1) % BUFFER_SIZE;
    }

    printf("========================================\n");
}

// Generate simulated sensor data
SensorData generateSensorData(void) {
    SensorData d;
    d.temperature = 20.0f + (rand() % 800) / 10.0f; // 20–100°C
    d.pressure = 1.0f + (rand() % 90) / 10.0f;      // 1–10 bar
    d.flow = (rand() % 1000) / 10.0f;               // 0–100 L/min
    d.timestamp = (unsigned long)time(NULL);
    return d;
}

// ===============================
// --- Main Function -------------
// ===============================
int main(void) {
    srand((unsigned int)time(NULL));

    CircularBuffer cb;
    initBuffer(&cb);

    printf("Starting Simulated Sensor Data Logger...\n");

    for (int i = 0; i < NUM_READINGS; i++) {
        SensorData data = generateSensorData();
        addReading(&cb, data);
        printBuffer(&cb);
        sleep(1); // Wait 1 second between readings
    }

    printf("\nSimulation complete.\n");
    return 0;
}
/*
+----------------------------+
| Virtual Sensors            |
|  - Temperature()           |
|  - Pressure()              |
|  - Flow()                  |
+-------------+--------------+
              |
              v
+----------------------------+
| Circular Buffer            |
|  (stores last N readings)  |
+-------------+--------------+
              |
              v
+----------------------------+
| Display/Logger             |
|  - Print buffer contents   |
|  - Show memory usage       |
+----------------------------+

*/