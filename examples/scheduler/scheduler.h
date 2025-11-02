#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stddef.h>

#define SCHED_MAX_TASKS 12

typedef void (*TaskFunc)(void);

/* Task states */
typedef enum {
    TASK_STATE_UNUSED = 0,
    TASK_STATE_READY,
    TASK_STATE_RUNNING,
    TASK_STATE_DISABLED
} TaskState;

typedef struct {
    TaskFunc    func;         /* function pointer */
    uint32_t    period_ms;    /* period in ms */
    uint32_t    time_left;    /* countdown to next run */
    TaskState   state;        /* ready / running / disabled */
    uint64_t    run_count;    /* how many times executed */
    uint64_t    runtime_ns;   /* cumulative runtime in nanoseconds */
} TaskControlBlock;

/* Scheduler API */
void scheduler_init(void);
int  scheduler_add(TaskFunc f, uint32_t period_ms); /* returns task id or -1 */
void scheduler_enable(size_t task_id);
void scheduler_disable(size_t task_id);
void scheduler_tick(void);      /* call from 1ms tick (internal) */
void scheduler_run_for(uint32_t duration_ms); /* run deterministic for duration_ms */
void scheduler_run(void);       /* run until SIGINT (Ctrl+C) */
float scheduler_cpu_load(void); /* last-run CPU load % (0.0..100.0) */
size_t scheduler_num_tasks(void);

#endif
