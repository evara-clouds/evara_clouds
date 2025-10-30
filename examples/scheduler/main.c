#include "scheduler.h"
#include <stdio.h>

void led_task(void)   { static int s=0; printf("[LED] toggle %d\n", s++); }
void temp_task(void)  { printf("[TEMP] read sensor\n"); }

int main(void) {
    scheduler_init();
    scheduler_add(led_task, 100);   // every 100 ms
    scheduler_add(temp_task, 250);  // every 250 ms
    scheduler_run();
    return 0;
}
