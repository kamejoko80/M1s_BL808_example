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

// A simple toggle flag for button color
static bool is_color_toggled = false;

// Callback for button click
static void button_event_handler(lv_event_t *event) {
    lv_obj_t *btn = lv_event_get_target(event);
    if (is_color_toggled) {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0000), LV_PART_MAIN); // Red
    } else {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_PART_MAIN); // Green
    }
    is_color_toggled = !is_color_toggled;
}

// Binding for lv_scr_act()
static duk_ret_t js_lv_scr_act(duk_context *ctx) {
    lv_obj_t *screen = lv_scr_act();
    duk_push_pointer(ctx, screen);
    return 1; // Number of return values
}

// Binding for lv_btn_create()
static duk_ret_t js_lv_btn_create(duk_context *ctx) {
    lv_obj_t *parent = duk_get_pointer(ctx, 0);
    lv_obj_t *btn = lv_btn_create(parent);
    duk_push_pointer(ctx, btn);
    return 1; // Number of return values
}

// Binding for lv_obj_center()
static duk_ret_t js_lv_obj_center(duk_context *ctx) {
    lv_obj_t *obj = duk_get_pointer(ctx, 0);
    lv_obj_center(obj);
    return 0; // No return values
}

// Binding for lv_label_create()
static duk_ret_t js_lv_label_create(duk_context *ctx) {
    lv_obj_t *parent = duk_get_pointer(ctx, 0);
    lv_obj_t *label = lv_label_create(parent);
    duk_push_pointer(ctx, label);
    return 1; // Number of return values
}

// Binding for lv_label_set_text()
static duk_ret_t js_lv_label_set_text(duk_context *ctx) {
    lv_obj_t *label = duk_get_pointer(ctx, 0);
    const char *text = duk_get_string(ctx, 1);
    lv_label_set_text(label, text);
    return 0; // No return values
}

// Binding for lv_obj_add_event_cb()
static duk_ret_t js_lv_obj_add_event_cb(duk_context *ctx) {
    lv_obj_t *obj = duk_get_pointer(ctx, 0);
    const char *event_name = duk_get_string(ctx, 1);

    // Currently only support "click" event
    if (strcmp(event_name, "click") == 0) {
        lv_obj_add_event_cb(obj, button_event_handler, LV_EVENT_CLICKED, NULL);
    } else {
        printf("Unsupported event type: %s", event_name);
    }

    return 0;
}

// Register LVGL functions to Duktape
void register_lvgl_bindings(duk_context *ctx) {
    // Register lv_scr_act()
    duk_push_c_function(ctx, js_lv_scr_act, 0);
    duk_put_global_string(ctx, "lv_scr_act");

    // Register lv_btn_create()
    duk_push_c_function(ctx, js_lv_btn_create, 1);
    duk_put_global_string(ctx, "lv_btn_create");

    // Register lv_obj_center()
    duk_push_c_function(ctx, js_lv_obj_center, 1);
    duk_put_global_string(ctx, "lv_obj_center");

    // Register lv_label_create()
    duk_push_c_function(ctx, js_lv_label_create, 1);
    duk_put_global_string(ctx, "lv_label_create");

    // Register lv_label_set_text()
    duk_push_c_function(ctx, js_lv_label_set_text, 2);
    duk_put_global_string(ctx, "lv_label_set_text");

    // Add lv_obj_add_event_cb binding
    duk_push_c_function(ctx, js_lv_obj_add_event_cb, 2);
    duk_put_global_string(ctx, "lv_obj_add_event_cb");
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
    register_lvgl_bindings(ctx);

    // Run JavaScript code
    const char *script =
        "var screen = lv_scr_act();"
        "var btn = lv_btn_create(screen);"
        "lv_obj_center(btn);"
        "var label = lv_label_create(btn);"
        "lv_label_set_text(label, 'Click Me');"
        "lv_obj_add_event_cb(btn, 'click');";

    if (duk_peval_string(ctx, script) != 0) {
        printf("Error: %s\r\n", duk_safe_to_string(ctx, -1));
    }
    duk_pop(ctx);

    lv_task_handler();
    xTaskCreate(lvgl_task, (char *)"lvgl task", 512, NULL, 15, NULL);

    // Destroy the Duktape heap
    //duk_destroy_heap(ctx);
}