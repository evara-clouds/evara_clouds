/*
 * Simulated Sensor Data Logger with Dynamic Circular Buffer and File Logging
 * Author: Evara
 * 
 * Features:
 *  - User-defined buffer size (dynamic allocation with malloc)
 *  - 3 simulated sensors: temperature, pressure, flow
 *  - Circular buffer behavior (overwrite oldest when full)
 *  - File logging to data_log.csv
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // for sleep()

// ===============================
// --- Data Structures -----------
// ===============================
typedef struct {
    float temperature;
    float pressure;
    float flow;
    unsigned long timestamp;
} SensorData;

typedef struct {
    SensorData *buffer;    // dynamically allocated array
    int capacity;          // max number of records
    int head;              // next write position
    int tail;              // oldest record
    int count;             // current number of items
    unsigned long overwrite_count;
} CircularBuffer;

// ===============================
// --- Function Prototypes -------
// ===============================
int initBuffer(CircularBuffer *cb, int capacity);
void freeBuffer(CircularBuffer *cb);
void addReading(CircularBuffer *cb, SensorData data);
void printBuffer(const CircularBuffer *cb);
SensorData generateSensorData(void);
void logToFile(SensorData data);

// ===============================
// --- Function Implementations ---
// ===============================
int initBuffer(CircularBuffer *cb, int capacity) {
    cb->buffer = (SensorData *)malloc(capacity * sizeof(SensorData));
    if (!cb->buffer) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return 0;
    }
    cb->capacity = capacity;
    cb->head = 0;
    cb->tail = 0;
    cb->count = 0;
    cb->overwrite_count = 0;
    return 1;
}

void freeBuffer(CircularBuffer *cb) {
    if (cb->buffer) {
        free(cb->buffer);
        cb->buffer = NULL;
    }
}

void addReading(CircularBuffer *cb, SensorData data) {
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % cb->capacity;

    if (cb->count < cb->capacity) {
        cb->count++;
    } else {
        cb->tail = (cb->tail + 1) % cb->capacity;
        cb->overwrite_count++;
    }
}

void printBuffer(const CircularBuffer *cb) {
    printf("\n========================================\n");
    printf("BUFFER STATUS: %d/%d filled | Overwrites: %lu\n",
           cb->count, cb->capacity, cb->overwrite_count);
    printf("----------------------------------------\n");
    printf("Idx | Temp (°C) | Press (bar) | Flow (L/min) | Timestamp\n");
    printf("----------------------------------------\n");

    int index = cb->tail;
    for (int i = 0; i < cb->count; i++) {
        SensorData d = cb->buffer[index];
        printf("%3d | %9.2f | %10.2f | %12.2f | %lu\n",
               i + 1, d.temperature, d.pressure, d.flow, d.timestamp);
        index = (index + 1) % cb->capacity;
    }
    printf("========================================\n");
}

SensorData generateSensorData(void) {
    SensorData d;
    d.temperature = 20.0f + (rand() % 800) / 10.0f; // 20–100°C
    d.pressure = 1.0f + (rand() % 90) / 10.0f;      // 1–10 bar
    d.flow = (rand() % 1000) / 10.0f;               // 0–100 L/min
    d.timestamp = (unsigned long)time(NULL);
    return d;
}

void logToFile(SensorData data) {
    FILE *file = fopen("data_log.csv", "a");
    if (!file) {
        fprintf(stderr, "Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%.2f,%.2f,%.2f,%lu\n",
            data.temperature, data.pressure, data.flow, data.timestamp);
    fclose(file);
}

// ===============================
// --- Main Function -------------
// ===============================
int main(void) {
    srand((unsigned int)time(NULL));

    CircularBuffer cb;
    int buffer_size, total_readings;

    printf("Enter circular buffer size: ");
    scanf("%d", &buffer_size);

    printf("Enter number of readings to simulate: ");
    scanf("%d", &total_readings);

    if (!initBuffer(&cb, buffer_size)) {
        return EXIT_FAILURE;
    }

    // Create or overwrite log file header
    FILE *file = fopen("data_log.csv", "w");
    fprintf(file, "Temperature (°C),Pressure (bar),Flow (L/min),Timestamp\n");
    fclose(file);

    printf("\nStarting Sensor Data Simulation...\n");

    for (int i = 0; i < total_readings; i++) {
        SensorData data = generateSensorData();
        addReading(&cb, data);
        logToFile(data); // write to CSV
        printBuffer(&cb);
        sleep(1); // simulate 1-second sampling
    }

    printf("\nSimulation complete. Data logged to 'data_log.csv'.\n");
    freeBuffer(&cb);
    return 0;
}
/*
┌────────────────────────────┐
│ Virtual Sensors (simulated)│
└───────────────┬────────────┘
                │
                ▼
┌────────────────────────────┐
│ Dynamically Allocated      │
│ Circular Buffer (malloc)   │
│ - stores N readings        │
└───────────────┬────────────┘
                │
                ▼
┌────────────────────────────┐
│ File Logger (data_log.csv) │
│ - appends readings          │
└────────────────────────────┘
*/
