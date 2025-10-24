#include <stdio.h>
#include <stdlib.h>

long get_file_size(const char *filepath) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Move to end of file
    if (fseek(file, 0L, SEEK_END) != 0) {
        perror("Error seeking to end of file");
        fclose(file);
        return -1;
    }

    // Get position (this equals file size)
    long size = ftell(file);
    fclose(file);

    return size;
}

int main(void) {
    const char *filepath = "/home/evara-clouds/Documents/classes/Measuring the diameter of a hair using a laser.pdf";

    long filesize = get_file_size(filepath);
    if (filesize < 0) {
        fprintf(stderr, "Failed to get file size.\n");
        return 1;
    }

    printf("File: %s\n", filepath);
    printf("Size: %ld bytes (%.2f KB)\n", filesize, filesize / 1024.0);

    return 0;
}
