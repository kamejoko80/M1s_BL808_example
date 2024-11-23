#ifndef JR_LVGL_H
#define JR_LVGL_H

#include "lvgl.h"
#include "jerryscript.h"

typedef struct {
    const char *cfunc_name;
    jerry_external_handler_t cfunc_handler;
} jerry_cfunc_entry_t;

#define JERRY_CFUNC_ENTRY(cfunc, handler) {.cfunc_name = (const char *)cfunc, .cfunc_handler = handler}

#define JERRY_GET_NATIVE_PTR(native_type, native_prt, js_ptr, native_info_ptr) \
    native_type * native_prt = NULL;                                           \
    native_prt = (native_type *)jr_get_lvgl_object(js_ptr, native_info_ptr);   \

void jr_register_cfunc_list(const jerry_cfunc_entry_t *entries, jerry_size_t size);
void *jr_get_lvgl_object(const jerry_value_t js_obj, const jerry_object_native_info_t *native_info_p);

#endif /* !JR_LVGL_H */