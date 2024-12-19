#include <stdio.h>
#include "jr_lvgl.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

/* Workaround: fixed compile error */
typedef struct _lv_event_dsc_t {
    lv_event_cb_t cb;
    void * user_data;
    lv_event_code_t filter : 8;
} lv_event_dsc_t;

lv_color_t jr_lvgl_color_to_color_t(uint32_t color) {
    lv_color_t lv_color;
#if LV_COLOR_DEPTH == 32
    // Assuming color is 0xAARRGGBB, extract ARGB8888 components
    lv_color.ch.red = (color >> 16) & 0xFF;
    lv_color.ch.green = (color >> 8) & 0xFF;
    lv_color.ch.blue = color & 0xFF;
    lv_color.ch.alpha = (color >> 24) & 0xFF;
#elif LV_COLOR_DEPTH == 16
#if LV_COLOR_16_SWAP == 0
    // Assuming color is 0xRRGGBB, convert to RGB565
    uint8_t red = (color >> 16) & 0xFF;
    uint8_t green = (color >> 8) & 0xFF;
    uint8_t blue = color & 0xFF;
    // Normal RGB565 (5-6-5 bit arrangement)
    lv_color.ch.red = red >> 3;      // 5 bits for red
    lv_color.ch.green = green >> 2;  // 6 bits for green
    lv_color.ch.blue = color & 0x1f; // 5 bits for blue
#else
    // Assuming color format is GH B R GL (3-5-5-3)
    lv_color.ch.red = (color >> 3) & 0x1F ;    // 5 bits for red
    lv_color.ch.green_l = color & 0x7;         // Lower 3 bits of green
    lv_color.ch.green_h = (color >> 13) & 0x7; // Upper 3 bits of green
    lv_color.ch.blue = (color >> 8) & 0x1F ;   // 5 bits for blue
#endif
#elif LV_COLOR_DEPTH == 8
    // Assuming color is grayscale or indexed
    uint8_t red = (color >> 16) & 0xFF;
    uint8_t green = (color >> 8) & 0xFF;
    uint8_t blue = color & 0xFF;
    lv_color.full = (uint8_t)(0.3 * red + 0.59 * green + 0.11 * blue);
#else
#error "Unsupported LV_COLOR_DEPTH"
#endif
    // printf("color =     %x\n", color);
    // printf("color r:    %x\n", lv_color.ch.red);
    // printf("color gh:   %x\n", lv_color.ch.green_h);
    // printf("color gl:   %x\n", lv_color.ch.green_l);
    // printf("color b:    %x\n", lv_color.ch.blue);
    // printf("color full: %x\n", lv_color.full);
    return lv_color;
}

uint32_t jr_lvgl_color_t_to_color(lv_color_t color) {
    return color.full;
}

/* This function allocates memory, please free the text pointer after using it */
char *jr_get_string(jerry_value_t value) {
    jerry_size_t size = jerry_string_size(value, JERRY_ENCODING_UTF8);
    char *text = malloc(size + 1);
    if (!text) {
        printf("jr_get_string: memory allocation failed\n");
        return NULL;
    }
    jerry_string_to_buffer(value, JERRY_ENCODING_UTF8, (jerry_char_t *)text, size);
    text[size] = '\0';
    return text;
}

void jr_lvgl_detach_children(lv_obj_t *parent) {
    uint32_t len = lv_obj_get_child_cnt(parent);
    lv_obj_t *grand_parent = lv_obj_get_parent(parent);

    while (len) {
        lv_obj_t *child = lv_obj_get_child(parent, len - 1);
        lv_obj_set_parent(child, grand_parent);
        len--;
    }
}

void jr_lvgl_obj_desctruct(lv_obj_t *obj, jr_clear_user_data_cb_t callback) {
    if(obj) {
        jr_lvgl_detach_children(obj);
        if(callback) {
            callback(obj);
        }
        lv_obj_del(obj);
    }
}

void jr_register_cfunc_list(const jerry_cfunc_entry_t *entries) {

    jerry_value_t global_object = jerry_current_realm();

    for (uint32_t idx = 0; (entries[idx].cfunc_name != NULL); idx++)
    {
        jerry_value_t property_name  = jerry_string_sz(entries[idx].cfunc_name);
        jerry_value_t property_value = jerry_function_external(entries[idx].cfunc_handler);
        jerry_value_t set_result     = jerry_object_set(global_object, property_name, property_value);

        if(jerry_value_is_exception(set_result)) {
            printf("Failed register %s function\n", entries[idx].cfunc_name);
        }

        jerry_value_free(set_result);
        jerry_value_free(property_value);
        jerry_value_free(property_name);
    }

    jerry_value_free(global_object);
}

void jr_register_global_constant_list(jerry_value_t global_obj, const jerry_const_entry_t *entries)
{
    for (uint32_t idx = 0; (entries[idx].name != NULL); idx++)
    {
        jerry_value_t name = jerry_string_sz(entries[idx].name);
        jerry_value_t value = jerry_number(entries[idx].value);
        jerry_value_t set_result = jerry_object_set(global_obj, name, value);
        if(jerry_value_is_exception(set_result)) {
            printf("Failed register global %s constant\n", entries[idx].name);
        }
        jerry_value_free(set_result);
        jerry_value_free(value);
        jerry_value_free(name);
    }
}

void jr_register_global_str_constant_list(jerry_value_t global_obj, const jerry_str_const_entry_t *entries)
{
    for (uint32_t idx = 0; (entries[idx].name != NULL); idx++)
    {
        jerry_value_t name = jerry_string_sz(entries[idx].name);
        jerry_value_t value = jerry_string_sz(entries[idx].value);
        jerry_value_t set_result = jerry_object_set(global_obj, name, value);
        if(jerry_value_is_exception(set_result)) {
            printf("Failed register global %s constant\n", entries[idx].name);
        }
        jerry_value_free(set_result);
        jerry_value_free(value);
        jerry_value_free(name);
    }
}

/* Refer to: 10.EXT-REFERENCE-HANDLER.md  */
void jr_set_prop_list(jerry_value_t prop_obj, jerryx_property_entry *methods) {

    jerryx_register_result reg = jerryx_set_properties(prop_obj, methods);
    /* if `reg.result` is undefined all methods are registered */
    if (jerry_value_is_exception (reg.result))
    {
        printf ("Only registered %d properties\r\n", reg.registered);
        /* clean up not registered property values */
        jerryx_release_property_entry(methods, reg);

        /* clean up the error */
        jerry_value_free(reg.result);
    }
}

void *jr_get_lvgl_object(const jerry_value_t js_obj, const jerry_object_native_info_t *native_info_p) {
    void *obj = NULL;
    obj = jerry_object_get_native_ptr(js_obj, native_info_p);
    if (obj == NULL) {
        jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid LVGL object");
    }
    return obj;
}

