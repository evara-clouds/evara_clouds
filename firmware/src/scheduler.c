#include "scheduler.h"
#include <stdint.h>

static volatile uint32_t tick_ms = 0;
static Task tasks[MAX_TASKS];

void scheduler_init(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].func = 0;
        tasks[i].period = 0;
        tasks[i].elapsed = 0;
        tasks[i].ready = 0;
    }
}

void scheduler_add(uint8_t id, TaskFunc func, uint32_t period_ms) {
    if (id < MAX_TASKS) {
        tasks[id].func = func;
        tasks[id].period = period_ms;
        tasks[id].elapsed = 0;
        tasks[id].ready = 0;
    }
}

void scheduler_tick(void) {
    tick_ms++;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].func) {
            tasks[i].elapsed++;
            if (tasks[i].elapsed >= tasks[i].period) {
                tasks[i].elapsed = 0;
                tasks[i].ready = 1;
            }
        }
    }
}

void scheduler_dispatch(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].ready && tasks[i].func) {
            tasks[i].ready = 0;
            tasks[i].func();
        }
    }
}
