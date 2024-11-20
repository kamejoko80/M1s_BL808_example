#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>
/* aos */
#include <cli.h>
/* fs */
#include <bl_romfs.h>
/* RISCV */
#include <csi_core.h>
/* lvgl */
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"
/* Duktape */
#include "duktape.h"

static void lvgl_task(void *param)
{
    while (1) {
        lv_task_handler();
        vTaskDelay(1);
    }
    vTaskDelete(NULL);
}

extern void register_lv_arc_bindings(duk_context *ctx);

const char *script =
    "var arc = lv_arc_create();\n"
    "lv_obj_center(arc);"
    "lv_arc_set_value(arc, 45);\n"
    "var value = lv_arc_get_value(arc);\n"
    "print('Arc value: ' + value);\n";

// Binding for lv_obj_center()
static duk_ret_t lv_obj_center_js(duk_context *ctx) {
    lv_obj_t *obj = duk_get_pointer(ctx, 0);
    lv_obj_center(obj);
    return 0; // No return values
}

void main()
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    // Set up Duktape
    duk_context *ctx = duk_create_heap_default();
    if (!ctx) {
        printf("Failed to create a Duktape heap.\r\n");
        return;
    }

    // Register LVGL bindings
    register_lv_arc_bindings(ctx);
    // Register lv_obj_center()
    duk_push_c_function(ctx, lv_obj_center_js, 1);
    duk_put_global_string(ctx, "lv_obj_center");

    if (duk_peval_string(ctx, script) != 0) {
        printf("Error: %s\r\n", duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx);

    lv_task_handler();
    xTaskCreate(lvgl_task, (char *)"lvgl task", 512, NULL, 15, NULL);

    // Destroy the Duktape heap
    //duk_destroy_heap(ctx);
}