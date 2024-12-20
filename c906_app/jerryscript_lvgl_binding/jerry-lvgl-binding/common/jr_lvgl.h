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
    const char *name;
    const char *value;
} jerry_str_const_entry_t;

typedef struct {
    uint32_t map_len;
    char   **map;
} jerry_user_data_t;

typedef void (*jr_clear_user_data_cb_t)(lv_obj_t *obj);

#define JERRY_CFUNC_ENTRY(cfunc, handler)     {.cfunc_name = (const char *)cfunc, .cfunc_handler = handler}
#define JERRY_CFUNC_LIST_END()                {.cfunc_name = NULL, .cfunc_handler = 0}

#define JERRY_CONST_ENTRY(cname, cvalue)      {.name = (const char *)cname, .value = cvalue}
#define JERRY_CONST_LIST_END()                {.name = NULL, .value = 0}

#define JERRY_STR_CONST_ENTRY(cname, cvalue)  {.name = (const char *)cname, .value = (const char *)cvalue}
#define JERRY_STR_CONST_LIST_END()            {.name = NULL, .value = NULL}

#define JERRY_GET_NATIVE_PTR(native_type, native_prt, js_ptr, native_info_ptr) \
    native_type * native_prt = NULL;                                           \
    native_prt = (native_type *)jr_get_lvgl_object(js_ptr, native_info_ptr);   \

#define SET_FIELD_IF_PRESENT(js_field, c_field, type_conversion)\
    do {\
        jerry_value_t val = jerry_object_get(draw_dsc_obj, jerry_string_sz(js_field));\
        if (jerry_value_is_number(val)) {\
            draw_dsc.c_field = (type_conversion)jerry_value_as_number(val);\
        }\
        jerry_value_free(val);\
    } while (0)

#define GET_FIELD_FROM_OBJ(js_obj, field_name, target_var, type) \
    do { \
        jerry_value_t field_key = jerry_string_sz(field_name); \
        jerry_value_t field_val = jerry_object_get(js_obj, field_key); \
        jerry_value_free(field_key); \
        if (!jerry_value_is_number(field_val)) { \
            jerry_value_free(field_val); \
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected '" field_name "' field as number"); \
        } \
        target_var = (type)jerry_value_as_number(field_val); \
        jerry_value_free(field_val); \
    } while (0)

void jr_set_prop_list(jerry_value_t prop_obj, jerryx_property_entry *methods);
void jr_lvgl_obj_desctruct(lv_obj_t *obj, jr_clear_user_data_cb_t callback);
void jr_register_cfunc_list(const jerry_cfunc_entry_t *entries);
void jr_register_obj_method(const jerry_value_t obj, const jerry_cfunc_entry_t *entries);
void jr_register_global_constant_list(jerry_value_t global_obj, const jerry_const_entry_t *entries);
void jr_register_global_str_constant_list(jerry_value_t global_obj, const jerry_str_const_entry_t *entries);
void *jr_get_lvgl_object(const jerry_value_t js_obj, const jerry_object_native_info_t *native_info_p);
char *jr_get_string(jerry_value_t value);
lv_color_t jr_lvgl_color_to_color_t(uint32_t color);
uint32_t jr_lvgl_color_t_to_color(lv_color_t color);

#endif /* !JR_LVGL_H */