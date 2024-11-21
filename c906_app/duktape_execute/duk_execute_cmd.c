
#include <stdio.h>
#include <stdlib.h>

/* aos */
#include <cli.h>

/* utils */
#include <utils_getopt.h>
#include <utils_log.h>

/* Duktape */
#include "duktape.h"

extern duk_context *ctx;
extern int duktape_execute_file(duk_context *ctx, const char *filename);

static duk_ret_t native_print(duk_context *ctx) {
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    printf("%s\r\n", duk_safe_to_string(ctx, -1));
    return 0;
}

void cmd_duk_execute(char *buf, int len, int argc, char **argv) {

    const char *filename;
    getopt_env_t getopt_env;
    utils_getopt_init(&getopt_env, 0);

    if (!ctx) {
        printf("Failed to create a Duktape heap.\r\n");
        return;
    }

    // Register native print function call
    duk_push_c_function(ctx, native_print, DUK_VARARGS);
    duk_put_global_string(ctx, "print");

    for (; getopt_env.optind < argc; getopt_env.optind++) {
        //printf("extra arguments: %s\r\n", argv[getopt_env.optind]);
        filename = argv[getopt_env.optind];
        int result = duktape_execute_file(ctx, filename);
        if (result != 0) {
            printf("Error executing script: %s\r\n", filename);
        }
    }
}