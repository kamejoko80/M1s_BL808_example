#ifndef JS_LV_OBJ_STYLE_H
#define JS_LV_OBJ_STYLE_H

#include "lvgl.h"
#include "lv_event.h"
#include "jerryscript.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

typedef struct {
    const char *name;
    const char *arg_type;
    void (*lv_style_func_v)(lv_obj_t * obj, uint32_t     value, lv_style_selector_t selector);
    void (*lv_style_func_c)(lv_obj_t * obj, lv_color_t   value, lv_style_selector_t selector);    
    void (*lv_style_func_p)(lv_obj_t * obj, const void * value, lv_style_selector_t selector);
} js_lv_style_property_map_t;

jerry_value_t js_lv_set_style(lv_obj_t *obj, const jerry_value_t args[], const jerry_length_t args_count);

#endif /* !JS_LV_OBJ_STYLE_H */
