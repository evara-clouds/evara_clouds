#include <stdio.h>

union SensorData {
    float temperature;        // 4 bytes
    unsigned char bytes[4];   // 4 bytes
    char status;              // 1 byte (shares same memory)
};

int main() {
    union SensorData data;

    // Simulate sensor sending 4 bytes in hex
    data.bytes[0] = 0xDB;
    data.bytes[1] = 0x0F;
    data.bytes[2] = 0x49;
    data.bytes[3] = 0x40;

    printf("Temperature: %.2f°C\n", data.temperature);

    // Now assign to 'status' — overwrites the same memory
    data.status = 'A';   // ASCII 65

    printf("Status (as char): %c\n", data.status);
    printf("Reinterpreted temperature: %.2f°C\n", data.temperature);

    return 0;
}
