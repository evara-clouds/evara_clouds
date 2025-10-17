#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/types.h>

long get_process_memory_kb(pid_t pid) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    FILE *file = fopen(path, "r");
    if (!file) return 0;

    char line[256];
    long mem = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "VmRSS: %ld", &mem) == 1)
            break;
    }
    fclose(file);
    return mem;  // in KB
}

int main(void) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("Error opening /proc");
        return 1;
    }

    struct dirent *entry;
    long total_mem_kb = 0;
    char path[256], cmdline[512];

    while ((entry = readdir(dir)) != NULL) {
        if (!isdigit(entry->d_name[0]))
            continue;  // skip non-numeric dirs

        snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);
        FILE *file = fopen(path, "r");
        if (!file)
            continue;

        if (fgets(cmdline, sizeof(cmdline), file)) {
            if (strstr(cmdline, "code") != NULL) {
                pid_t pid = atoi(entry->d_name);
                long mem_kb = get_process_memory_kb(pid);
                total_mem_kb += mem_kb;
            }
        }
        fclose(file);
    }

    closedir(dir);

    if (total_mem_kb > 0)
        printf("VS Code total memory usage: %ld KB (%.2f MB)\n",
               total_mem_kb, total_mem_kb / 1024.0);
    else
        printf("No VS Code processes found.\n");

    return 0;
}
