#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

long get_directory_size(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat info;
    char fullpath[4096];
    long total = 0;

    dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (stat(fullpath, &info) == 0) {
            if (S_ISREG(info.st_mode))
                total += info.st_size; // Add file size
            else if (S_ISDIR(info.st_mode))
                total += get_directory_size(fullpath); // Recurse into subdirectory
        }
    }

    closedir(dir);
    return total;
}

int main(void) {
    const char *dirpath = "/home/evara-clouds/evara_clouds";//the directory path to calculate size 

    long total_size = get_directory_size(dirpath);
    if (total_size >= 0)
        printf("Total size of '%s': %ld bytes (%.2f MB)\n",
               dirpath, total_size, total_size / (1024.0 * 1024.0));
    else
        fprintf(stderr, "Failed to calculate directory size.\n");

    return 0;
}
