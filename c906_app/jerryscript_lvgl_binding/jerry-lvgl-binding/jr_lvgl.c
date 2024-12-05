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

