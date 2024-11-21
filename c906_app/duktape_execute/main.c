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
duk_context *ctx = NULL;

/* Extern */
extern void cmd_duk_execute(char *buf, int len, int argc, char **argv);

const static struct cli_command cmds_user[] STATIC_CLI_CMD_ATTRIBUTE = {
    {"duk_execute", "Execute Duktaple JS file", cmd_duk_execute},
};

void main()
{
    ctx = duk_create_heap_default();
    //duk_destroy_heap(ctx);
}