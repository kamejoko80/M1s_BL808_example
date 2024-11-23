#include <stdio.h>

#include "jr_lvgl.h"

void jr_register_cfunc_list(const jerry_cfunc_entry_t *entries, jerry_size_t size) {

    jerry_value_t global_object = jerry_current_realm();

    for(jerry_size_t i = 0; i < size; i++) {
        jerry_value_t property_name  = jerry_string_sz(entries[i].cfunc_name);
        jerry_value_t property_value = jerry_function_external(entries[i].cfunc_handler);
        jerry_value_t set_result     = jerry_object_set(global_object, property_name, property_value);

        if(jerry_value_is_exception(set_result)) {
            printf("Failed register %s function\n", entries[i].cfunc_name);
        }

        jerry_value_free(set_result);
        jerry_value_free(property_value);
        jerry_value_free(property_name);
    }

    jerry_value_free(global_object);
}

void *jr_get_lvgl_object(const jerry_value_t js_obj, const jerry_object_native_info_t *native_info_p) {
    void *obj = NULL;
    obj = jerry_object_get_native_ptr(js_obj, native_info_p);
    if (obj == NULL) {
        jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid LVGL object");
    }
    return obj;
}

