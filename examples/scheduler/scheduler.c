#define _POSIX_C_SOURCE 200809L

#include "scheduler.h"
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>

static TaskControlBlock tasks[SCHED_MAX_TASKS];
static size_t task_count = 0;

/* runtime accounting */
static uint64_t total_ticks = 0;     /* number of 1ms ticks seen */
static uint64_t active_ticks = 0;    /* ticks where >=1 task ran */
static volatile sig_atomic_t keep_running = 1;

/* helper: monotonic now in ns */
static inline uint64_t now_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + (uint64_t)ts.tv_nsec;
}

void scheduler_init(void) {
    memset(tasks, 0, sizeof(tasks));
    task_count = 0;
    total_ticks = 0;
    active_ticks = 0;
}

/* add task; returns id or -1 */
int scheduler_add(TaskFunc f, uint32_t period_ms) {
    if (!f || task_count >= SCHED_MAX_TASKS) return -1;
    tasks[task_count].func = f;
    tasks[task_count].period_ms = period_ms;
    tasks[task_count].time_left = period_ms;
    tasks[task_count].state = TASK_STATE_READY;
    tasks[task_count].run_count = 0;
    tasks[task_count].runtime_ns = 0;
    return (int)task_count++;
}

void scheduler_enable(size_t task_id) {
    if (task_id < task_count) tasks[task_id].state = TASK_STATE_READY;
}

void scheduler_disable(size_t task_id) {
    if (task_id < task_count) tasks[task_id].state = TASK_STATE_DISABLED;
}

/* decrement time-left for active tasks (1 ms tick) */
void scheduler_tick(void) {
    for (size_t i = 0; i < task_count; ++i) {
        if (tasks[i].state == TASK_STATE_READY && tasks[i].time_left > 0) {
            tasks[i].time_left--;
        }
    }
}

/* internal single step: run ready tasks whose time_left == 0,
   measure per-task execution time and update counters */
static void scheduler_step_once(void) {
    int ran_any = 0;

    for (size_t i = 0; i < task_count; ++i) {
        if (tasks[i].state == TASK_STATE_READY && tasks[i].time_left == 0) {
            tasks[i].state = TASK_STATE_RUNNING;

            uint64_t t0 = now_ns();
            tasks[i].func();
            uint64_t t1 = now_ns();

            tasks[i].run_count++;
            tasks[i].runtime_ns += (t1 - t0);

            tasks[i].time_left = tasks[i].period_ms;
            tasks[i].state = TASK_STATE_READY;
            ran_any = 1;
        }
    }

    if (ran_any) active_ticks++;
}

/* compute CPU load (percentage of ticks where something ran) */
float scheduler_cpu_load(void) {
    if (total_ticks == 0) return 0.0f;
    return (float)active_ticks * 100.0f / (float)total_ticks;
}

size_t scheduler_num_tasks(void) {
    return task_count;
}

/* run deterministic for duration_ms milliseconds */
void scheduler_run_for(uint32_t duration_ms) {
    struct timespec ts = {0, 1000000}; /* 1ms */

    uint32_t ticks_to_run = duration_ms;
    total_ticks = 0;
    active_ticks = 0;

    printf("Scheduler: running for %u ms | tasks: %zu\n", duration_ms, task_count);

    for (uint32_t tick = 0; tick < ticks_to_run; ++tick) {
        scheduler_step_once();
        scheduler_tick();
        nanosleep(&ts, NULL);
        total_ticks++;
    }

    printf("Simulation complete. Ticks: %lu | Active ticks: %lu | CPU load: %.2f%%\n",
           (unsigned long)total_ticks, (unsigned long)active_ticks, scheduler_cpu_load());
    /* print per-task stats */
    for (size_t i = 0; i < task_count; ++i) {
        printf("Task %zu: runs=%" PRIu64 " total_runtime_ms=%.3f\n",
               i, tasks[i].run_count, tasks[i].runtime_ns / 1e6);
    }
}

/* SIGINT handler to stop scheduler_run() */
static void handle_sigint(int sig) {
    (void)sig;
    keep_running = 0;
}

void scheduler_run(void) {
    struct timespec ts = {0, 1000000}; /* 1ms */
    total_ticks = 0;
    active_ticks = 0;
    keep_running = 1;

    signal(SIGINT, handle_sigint);

    printf("Scheduler: running until SIGINT (Ctrl+C) | tasks: %zu\n", task_count);

    while (keep_running) {
        scheduler_step_once();
        scheduler_tick();
        nanosleep(&ts, NULL);
        total_ticks++;
    }

    printf("\nScheduler stopped. Ticks: %lu | Active ticks: %lu | CPU load: %.2f%%\n",
           (unsigned long)total_ticks, (unsigned long)active_ticks, scheduler_cpu_load());
    for (size_t i = 0; i < task_count; ++i) {
        printf("Task %zu: runs=%" PRIu64 " total_runtime_ms=%.3f\n",
               i, tasks[i].run_count, tasks[i].runtime_ns / 1e6);
    }
}
