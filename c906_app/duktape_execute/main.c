#include <stdio.h>

/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>

/* aos */
#include <cli.h>

/* fs */
#include <fatfs.h>
#include <vfs.h>

/* RISCV */
#include <csi_core.h>

/* Duktape */
#include "duktape.h"

/* Global */
duk_context *ctx;

/* Extern */
extern void cmd_duk_execute(char *buf, int len, int argc, char **argv);

const static struct cli_command cmds_user[] STATIC_CLI_CMD_ATTRIBUTE = {
    {"duk_execute", "Execute Duktaple JS file", cmd_duk_execute},
};

static duk_ret_t native_print(duk_context *ctx) {
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    printf("%s\r\n", duk_safe_to_string(ctx, -1));
    return 0;
}

void main()
{
    fatfs_register();

    ctx = duk_create_heap_default();
    if (!ctx) {
        printf("Failed to create a Duktape heap.\r\n");
        return;
    }

    // Register native print function call
    duk_push_c_function(ctx, native_print, DUK_VARARGS);
    duk_put_global_string(ctx, "print");

    // duk_destroy_heap(ctx);
}