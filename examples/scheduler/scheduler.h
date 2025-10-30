#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define MAX_TASKS 8

typedef void (*TaskFunc)(void);

typedef struct {
    TaskFunc task;
    uint32_t period_ms;
    uint32_t time_left;
    uint8_t active;
    uint32_t run_count;     // <-- new: number of times executed
    uint32_t last_run_time; // <-- new: last execution timestamp (ms)
} Task;


void scheduler_init(void);
int  scheduler_add(TaskFunc func, uint32_t period_ms);
void scheduler_tick(void);
void scheduler_run(void);

#endif
