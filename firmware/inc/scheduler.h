#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 8

typedef void (*TaskFunc)(void);

typedef struct {
    TaskFunc func;
    uint32_t period;
    uint32_t elapsed;
    uint8_t ready;
} Task;

void scheduler_init(void);
void scheduler_add(uint8_t id, TaskFunc func, uint32_t period_ms);
void scheduler_tick(void);
void scheduler_dispatch(void);

#endif
