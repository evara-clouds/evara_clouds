#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

long get_memory_usage_kb(pid_t pid) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");
    if (!file) {
        perror("Error opening status file");
        return -1;
    }

    char line[256];
    long mem = -1;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "VmRSS: %ld", &mem) == 1) {
            break;  // Found the resident set size (actual physical memory used)
        }
    }

    fclose(file);
    return mem;  // in KB
}

int main(void) {
    pid_t pid;
    printf("Enter PID of the running app: ");
    scanf("%d", &pid);

    long mem_kb = get_memory_usage_kb(pid);
    if (mem_kb > 0)
        printf("App (PID %d) is using %ld KB (%.2f MB) of RAM.\n",
               pid, mem_kb, mem_kb / 1024.0);
    else
        fprintf(stderr, "Could not read memory usage for PID %d.\n", pid);

    return 0;
}
