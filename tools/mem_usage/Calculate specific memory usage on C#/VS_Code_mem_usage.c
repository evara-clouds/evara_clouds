#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>


long get_vscode_memory_kb() {
    DIR *dir = opendir("/proc");
    if (!dir) {
        perror("Error opening /proc");
        return -1;
    }

    struct dirent *entry;
    char path[256], cmdline[256];
    FILE *file;
    long mem_kb = -1;

    while ((entry = readdir(dir)) != NULL) {
        // Skip non-numeric directories (only numeric names are PIDs)
        if (!isdigit(entry->d_name[0]))
            continue;

        snprintf(path, sizeof(path), "/proc/%s/cmdline", entry->d_name);
        file = fopen(path, "r");
        if (!file)
            continue;

        if (fgets(cmdline, sizeof(cmdline), file)) {
            if (strstr(cmdline, "code") != NULL) { // Found VS Code process
                fclose(file);
                snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
                file = fopen(path, "r");
                if (!file)
                    continue;

                char line[256];
                while (fgets(line, sizeof(line), file)) {
                    if (sscanf(line, "VmRSS: %ld", &mem_kb) == 1) {
                        fclose(file);
                        closedir(dir);
                        return mem_kb;
                    }
                }
                fclose(file);
            }
        }
        fclose(file);
    }

    closedir(dir);
    return -1;
}

int main(void) {
    long mem_kb = get_vscode_memory_kb();

    if (mem_kb > 0)
        printf("VS Code is using approximately %ld KB (%.2f MB) of RAM.\n",
               mem_kb, mem_kb / 1024.0);
    else
        printf("Could not find VS Code process or failed to read memory.\n");

    return 0;
}
