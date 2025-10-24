#include "scheduler.h"
#include <stdio.h>
#include <unistd.h>

void task_led(void)   { printf("[Task] LED Blink\n"); }
void task_sensor(void){ printf("[Task] Read Sensor\n"); }
void task_log(void)   { printf("[Task] Log Data\n"); }

int main(void) {
    scheduler_init();
    scheduler_add(0, task_led, 100);
    scheduler_add(1, task_sensor, 200);
    scheduler_add(2, task_log, 500);

    while (1) {
        scheduler_tick();
        scheduler_dispatch();
        usleep(1000); // 1 ms tick simulation
    }
}
