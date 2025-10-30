#define _POSIX_C_SOURCE 200809L

#include "scheduler.h"
#include <stdio.h>
#include <time.h>    // for nanosleep()
#include <stdint.h>
#include <signal.h>   // for signal handling
#include <stdatomic.h> // for atomic flag (optional, cleaner)

static volatile sig_atomic_t keep_running = 1;

void handle_sigint(int sig) {
    (void)sig;
    keep_running = 0;
}

static Task tasks[MAX_TASKS];
static uint32_t num_tasks = 0;

/**
 * Initialize the scheduler by clearing all tasks.
 */
void scheduler_init(void) {
    for (uint32_t i = 0; i < MAX_TASKS; ++i) {
        tasks[i].active = 0;
    }
    num_tasks = 0;
}

/**
 * Add a task to the scheduler with a given execution period (in ms).
 */
int scheduler_add(TaskFunc func, uint32_t period_ms) {
    if (num_tasks >= MAX_TASKS) return -1;

    tasks[num_tasks].task = func;
    tasks[num_tasks].period_ms = period_ms;
    tasks[num_tasks].time_left = period_ms;
    tasks[num_tasks].active = 1;

    num_tasks++;
    return 0;
}

/**
 * Decrement the countdown timers of all active tasks.
 */
void scheduler_tick(void) {
    for (uint32_t i = 0; i < num_tasks; ++i) {
        if (tasks[i].active && tasks[i].time_left > 0)
            tasks[i].time_left--;
    }
}
void scheduler_enable_task(uint32_t id) {
    if (id < num_tasks) tasks[id].active = 1;
}

void scheduler_disable_task(uint32_t id) {
    if (id < num_tasks) tasks[id].active = 0;
}

/**
 * Cooperative scheduler main loop.
 * Each tick = 1 ms (simulated using nanosleep).
 */
void scheduler_run(void) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1000000;  // 1 ms

    printf("=== Cooperative RTOS Simulation ===\n");
    printf("Tick: 1 ms | Tasks: %u\n\n", num_tasks);

    while (1) {
        for (uint32_t i = 0; i < num_tasks; ++i) {
            if (tasks[i].active && tasks[i].time_left == 0) {
                tasks[i].task();
                tasks[i].time_left = tasks[i].period_ms;
            }
        }

        scheduler_tick();
        nanosleep(&ts, NULL);  // 1 ms simulated delay
uint64_t total_ticks = 0;
uint64_t active_ticks = 0;

while (keep_running) {
    total_ticks++;
    uint8_t any_run = 0;

    for (uint32_t i = 0; i < num_tasks; ++i) {
        if (tasks[i].active && tasks[i].time_left == 0) {
            tasks[i].task();
            tasks[i].time_left = tasks[i].period_ms;
            tasks[i].run_count++;
            any_run = 1;
        }
    }

    if (any_run) active_ticks++;

    scheduler_tick();
    nanosleep(&ts, NULL);
}

float cpu_load = (float)active_ticks / total_ticks * 100.0f;
printf("Scheduler stopped. CPU load: %.2f%%\n", cpu_load);
    }
}

