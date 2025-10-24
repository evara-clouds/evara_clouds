#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

long get_process_memory_kb(pid_t pid) {
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");
    if (!file)
        return -1;

    char line[256];
    long mem = -1;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "VmRSS: %ld", &mem) == 1)
            break;
    }

    fclose(file);
    return mem;
}

int main(void) {
    char target[128];
    printf("Enter the name of the app (e.g., code, brave, firefox): ");
    scanf("%127s", target);

    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("Error opening /proc");
        return 1;
    }

    struct dirent *entry;
    long total_mem_kb = 0;
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!isdigit(entry->d_name[0]))
            continue;

        char cmdline_path[256];
        snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%s/cmdline", entry->d_name);

        FILE *cmdfile = fopen(cmdline_path, "r");
        if (!cmdfile)
            continue;

        char cmdline[512];
        if (fgets(cmdline, sizeof(cmdline), cmdfile)) {
            if (strstr(cmdline, target)) {
                pid_t pid = atoi(entry->d_name);
                long mem_kb = get_process_memory_kb(pid);
                if (mem_kb > 0) {
                    total_mem_kb += mem_kb;
                    count++;
                }
            }
        }

        fclose(cmdfile);
    }

    closedir(dir);

    if (count > 0)
        printf("Total memory used by '%s' (%d processes): %ld KB (%.2f MB)\n",
               target, count, total_mem_kb, total_mem_kb / 1024.0);
    else
        printf("No processes found matching '%s'.\n", target);

    return 0;
}
