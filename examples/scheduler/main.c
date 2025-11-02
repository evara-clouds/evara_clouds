#include "scheduler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* lightweight demo tasks */
void task1_sensor(void) {
    static int c = 0;
    printf("[T1] Sensor read %d\n", c++);
}

void task2_uart(void) {
    printf("[T2] UART send\n");
}

void task3_logger(void) {
    printf("[T3] Logging\n");
}

int main(int argc, char *argv[]) {
    scheduler_init();
    scheduler_add(task1_sensor, 5);   /* every 5 ms */
    scheduler_add(task2_uart,    10); /* every 10 ms */
    scheduler_add(task3_logger,  25); /* every 25 ms */

    if (argc > 1 && strcmp(argv[1], "--ticks") == 0 && argc > 2) {
        uint32_t dur = (uint32_t)atoi(argv[2]);
        scheduler_run_for(dur);
    } else {
        /* run until Ctrl+C */
        scheduler_run();
    }

    return 0;
}
