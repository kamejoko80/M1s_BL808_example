#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <fatfs.h>
#include <aos/kernel.h>
#include <vfs.h>
#include "duktape.h"

int duktape_execute_file(duk_context *ctx, const char *filename) {
    int fd = -1;
    struct stat stat;
    struct stat *st = &stat;
    size_t br;
    char *buffer = NULL;
    size_t file_size = 0;
    int retval = -1;

    // Open the JavaScript file
    if ((fd = aos_open(filename, O_RDWR)) < 0) {
        printf("%s not found!\r\n", filename);
        return -1;
    }

    // Get file size
    if (0 == aos_stat(filename, st)) {
        file_size = st->st_size;
        printf("JavaScript file size = %ld\r\n", file_size);
    } else {
        printf("Could not get file stat\r\n");
        file_size = 0;
        aos_close(fd);
        return -1;
    }

    // Allocate buffer to read the file content
    buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        aos_close(fd);
        return -1;
    }

    // Read the file content
    br = aos_read(fd, buffer, file_size);
    if (br == 0) {
        printf("Failed to read file: %s\r\n", filename);
        free(buffer);
        aos_close(fd);
        return -1;
    }

    // Null-terminate the buffer for Duktape
    buffer[file_size] = '\0';

    // Execute the script in Duktape
    if (duk_peval_string(ctx, buffer) != 0) {
        printf("Duktape execution failed: %s\r\n", duk_safe_to_string(ctx, -1));
    } else {
        printf("Script executed successfully\r\n");
        retval = 0; // Success
    }

    // Clean up
    duk_pop(ctx); // Pop result or error
    free(buffer);
    aos_close(fd);

    return retval;
}
