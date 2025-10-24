/*
┌─────────────────────────────┐
│ Virtual Sensors             │
│  - Temperature, Pressure... │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ Circular Buffer (dynamic)   │
│ Stores readings             │
└───────────────┬─────────────┘
                │
                ▼
┌─────────────────────────────┐
│ Alarm System                │
│  - Compares against limits  │
│  - Prints warnings/logs     │
└─────────────────────────────┘
*/
/*
 * Simulated Sensor Data Logger with Alarms
 * Author: Evara
 *
 * Features:
 *  - Dynamic circular buffer (user-defined size)
 *  - File logging (data_log.csv)
 *  - Real-time alarm checks for over/under thresholds
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// ===============================
// --- Configuration Section -----
// ===============================
#define TEMP_HIGH_LIMIT   80.0f   // °C
#define PRESS_HIGH_LIMIT   9.0f   // bar
#define FLOW_LOW_LIMIT    10.0f   // L/min

// ANSI color codes for terminal highlighting
#define RED     "\x1b[31m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"

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
    SensorData *buffer;
    int capacity;
    int head;
    int tail;
    int count;
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
void checkAlarms(SensorData data); // <--- NEW

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
    cb->head = cb->tail = cb->count = 0;
    cb->overwrite_count = 0;
    return 1;
}

void freeBuffer(CircularBuffer *cb) {
    if (cb->buffer) free(cb->buffer);
}

void addReading(CircularBuffer *cb, SensorData data) {
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) % cb->capacity;

    if (cb->count < cb->capacity)
        cb->count++;
    else {
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
    d.pressure    = 1.0f + (rand() % 90) / 10.0f;   // 1–10 bar
    d.flow        = (rand() % 1000) / 10.0f;        // 0–100 L/min
    d.timestamp   = (unsigned long)time(NULL);
    return d;
}

void logToFile(SensorData data) {
    FILE *file = fopen("data_log.csv", "a");
    if (!file) return;
    fprintf(file, "%.2f,%.2f,%.2f,%lu\n",
            data.temperature, data.pressure, data.flow, data.timestamp);
    fclose(file);
}

/*
 * checkAlarms:
 *   Checks if any process variable exceeds a threshold.
 *   Prints warning and logs to 'alarms_log.txt' if triggered.
 */
void checkAlarms(SensorData data) {
    FILE *file = fopen("alarms_log.txt", "a");
    if (!file) return;

    if (data.temperature > TEMP_HIGH_LIMIT) {
        printf(RED "⚠️  HIGH TEMP ALARM: %.2f °C\n" RESET, data.temperature);
        fprintf(file, "High Temp Alarm: %.2f °C at %lu\n", data.temperature, data.timestamp);
    }

    if (data.pressure > PRESS_HIGH_LIMIT) {
        printf(RED "⚠️  HIGH PRESSURE ALARM: %.2f bar\n" RESET, data.pressure);
        fprintf(file, "High Pressure Alarm: %.2f bar at %lu\n", data.pressure, data.timestamp);
    }

    if (data.flow < FLOW_LOW_LIMIT) {
        printf(YELLOW "⚠️  LOW FLOW WARNING: %.2f L/min\n" RESET, data.flow);
        fprintf(file, "Low Flow Warning: %.2f L/min at %lu\n", data.flow, data.timestamp);
    }

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

    if (!initBuffer(&cb, buffer_size))
        return EXIT_FAILURE;

    // Prepare log files
    FILE *datafile = fopen("data_log.csv", "w");
    fprintf(datafile, "Temperature (°C),Pressure (bar),Flow (L/min),Timestamp\n");
    fclose(datafile);

    FILE *alarmfile = fopen("alarms_log.txt", "w");
    fprintf(alarmfile, "=== Alarm Log ===\n");
    fclose(alarmfile);

    printf("\nStarting Sensor Data Simulation...\n");

    for (int i = 0; i < total_readings; i++) {
        SensorData data = generateSensorData();
        addReading(&cb, data);
        logToFile(data);
        checkAlarms(data);     // <--- NEW ALARM CHECK
        printBuffer(&cb);
        sleep(1);
    }

    printf("\nSimulation complete. Logs saved to 'data_log.csv' and 'alarms_log.txt'.\n");
    freeBuffer(&cb);
    return 0;
}

