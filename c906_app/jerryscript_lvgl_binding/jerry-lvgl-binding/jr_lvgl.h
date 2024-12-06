#ifndef JR_LVGL_H
#define JR_LVGL_H

#include "lvgl.h"
#include "lv_event.h"
#include "jerryscript.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

typedef struct {
    const char *cfunc_name;
    jerry_external_handler_t cfunc_handler;
} jerry_cfunc_entry_t;

typedef struct {
    const char *name;
    uint32_t value;
} jerry_const_entry_t;

typedef struct {
    uint32_t map_len;
    char   **map;
} jerry_user_data_t;

typedef void (*jr_clear_user_data_cb_t)(lv_obj_t *obj);

#define JERRY_CFUNC_ENTRY(cfunc, handler) {.cfunc_name = (const char *)cfunc, .cfunc_handler = handler}
#define JERRY_CFUNC_LIST_END()            {.cfunc_name = NULL, .cfunc_handler = 0}

#define JERRY_CONST_ENTRY(cname, cvalue)  {.name = (const char *)cname, .value = cvalue}
#define JERRY_CONST_LIST_END()            {.name = NULL, .value = 0}

#define JERRY_GET_NATIVE_PTR(native_type, native_prt, js_ptr, native_info_ptr) \
    native_type * native_prt = NULL;                                           \
    native_prt = (native_type *)jr_get_lvgl_object(js_ptr, native_info_ptr);   \

void jr_set_prop_list(jerry_value_t prop_obj, jerryx_property_entry *methods);
void jr_lvgl_obj_desctruct(lv_obj_t *obj, jr_clear_user_data_cb_t callback);
void jr_register_cfunc_list(const jerry_cfunc_entry_t *entries);
void jr_register_global_constant_list(jerry_value_t global_obj, const jerry_const_entry_t *entries);
void *jr_get_lvgl_object(const jerry_value_t js_obj, const jerry_object_native_info_t *native_info_p);
char *jr_get_string(jerry_value_t value);
lv_color_t jr_lvgl_convert_color_t(uint32_t color);

#endif /* !JR_LVGL_H */