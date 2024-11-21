
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

void cmd_duk_execute(char *buf, int len, int argc, char **argv) {

    const char *filename;
    getopt_env_t getopt_env;
    utils_getopt_init(&getopt_env, 0);

    for (; getopt_env.optind < argc; getopt_env.optind++) {
        //printf("extra arguments: %s\r\n", argv[getopt_env.optind]);
        if(ctx != NULL) {
            filename = argv[getopt_env.optind];
            int result = duktape_execute_file(ctx, filename);
            if (result != 0) {
                printf("Error executing script: %s\r\n", filename);
            }
        }
    }
}