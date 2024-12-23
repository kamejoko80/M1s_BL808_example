/*
 * LVGL Label JerryScript Bindings
 * Author: Henry Dang
 *
 * MIT License
 *
 * Copyright (c) 2024 Henry Dang
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include "jr_lvgl.h"
#include "js_lv_obj_style.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

/* Change the object appropriately */
#define LV_OBJ_NAME           "Bar"
#define LV_OBJ_CREATE(parent) lv_bar_create(parent)

/************************************************************************
* Typedef
*************************************************************************/

typedef struct {
    uint32_t value;
    char     *name;
} jerry_lv_user_data_t;

/************************************************************************
* Function prototypes
*************************************************************************/

static void js_lv_obj_destructor_cb(void *native_p, jerry_object_native_info_t *call_info_p);
static void js_lv_obj_event_cb(lv_event_t *e);

/************************************************************************
* Global variable definition
*************************************************************************/

static jerry_object_native_info_t jerry_obj_native_info = {
    .free_cb = js_lv_obj_destructor_cb,
};

/************************************************************************
* Protperties, methodes definition
*************************************************************************/

static jerry_value_t js_lv_obj_align(const jerry_call_info_t *call_info_p,
                                     const jerry_value_t args[],
                                     const jerry_length_t args_count) {
    // Ensure at least 3 arguments: align, x_ofs, y_ofs
    if (args_count < 3) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Insufficient arguments");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if(obj == NULL) {
       return jerry_undefined();
    }

    uint32_t align = (uint32_t)jerry_value_as_number(args[0]);
    lv_coord_t x_ofs = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t y_ofs = (lv_coord_t)jerry_value_as_number(args[2]);

    // Call LVGL function
    lv_obj_align(obj, align, x_ofs, y_ofs);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_size(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_cnt) {
    if (args_cnt < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected (width, height)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if(obj == NULL) {
       return jerry_undefined();
    }

    lv_coord_t width = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t height = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_obj_set_size(obj, width, height);
    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_style(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (property, value, selector)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    js_lv_set_style(obj, args, args_count);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_value(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (value, anim)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int32_t value = (int32_t)jerry_value_as_number(args[1]);
    lv_anim_enable_t anim = (lv_anim_enable_t)jerry_value_as_number(args[2]);

    lv_bar_set_value(obj, value, anim);
    return jerry_undefined();
}

static jerry_value_t js_lv_bar_set_start_value(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (start_value, anim)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int32_t start_value = (int32_t)jerry_value_as_number(args[1]);
    lv_anim_enable_t anim = (lv_anim_enable_t)jerry_value_as_number(args[2]);

    lv_bar_set_start_value(obj, start_value, anim);
    return jerry_undefined();
}

static jerry_value_t js_lv_bar_set_range(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (min, max)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int32_t min = (int32_t)jerry_value_as_number(args[1]);
    int32_t max = (int32_t)jerry_value_as_number(args[2]);

    lv_bar_set_range(obj, min, max);
    return jerry_undefined();
}

static jerry_value_t js_lv_bar_set_mode(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (mode)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    lv_bar_mode_t mode = (lv_bar_mode_t)jerry_value_as_number(args[1]);

    lv_bar_set_mode(obj, mode);
    return jerry_undefined();
}

static jerry_value_t js_lv_bar_get_value(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int32_t value = lv_bar_get_value(obj);
    return jerry_number(value);
}

static jerry_value_t js_lv_bar_get_start_value(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int32_t start_value = lv_bar_get_start_value(obj);
    return jerry_number(start_value);
}

static jerry_value_t js_lv_bar_get_min_value(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int32_t min_value = lv_bar_get_min_value(obj);
    return jerry_number(min_value);
}

static jerry_value_t js_lv_bar_get_max_value(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int32_t max_value = lv_bar_get_max_value(obj);
    return jerry_number(max_value);
}

static jerry_value_t js_lv_bar_get_mode(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    lv_bar_mode_t mode = lv_bar_get_mode(obj);
    return jerry_number(mode);
}

static jerry_value_t js_lv_obj_on_press(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_function(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected a callback function");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if(obj == NULL) {
       return jerry_undefined();
    }

    // Register the event callback
    lv_obj_add_event_cb(obj, js_lv_obj_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)args[0]);

    return jerry_undefined();
}

/************************************************************************
* Protperties, methodes entry list
*************************************************************************/

static const jerry_cfunc_entry_t methods[] = {
    JERRY_CFUNC_ENTRY("align",         js_lv_obj_align),
    JERRY_CFUNC_ENTRY("setSize",       js_lv_obj_set_size),
    JERRY_CFUNC_ENTRY("setStyle",      js_lv_obj_set_style),
    JERRY_CFUNC_ENTRY("setValue",      js_lv_obj_set_value),
    JERRY_CFUNC_ENTRY("setStartValue", js_lv_bar_set_start_value),
    JERRY_CFUNC_ENTRY("setRange",      js_lv_bar_set_range),
    JERRY_CFUNC_ENTRY("setMode",       js_lv_bar_set_mode),
    JERRY_CFUNC_ENTRY("getValue",      js_lv_bar_get_value),
    JERRY_CFUNC_ENTRY("getStartValue", js_lv_bar_get_start_value),
    JERRY_CFUNC_ENTRY("getMinValue",   js_lv_bar_get_min_value),
    JERRY_CFUNC_ENTRY("getMaxValue",   js_lv_bar_get_max_value),
    JERRY_CFUNC_ENTRY("getMode",       js_lv_bar_get_mode),
    JERRY_CFUNC_ENTRY("onPress",       js_lv_obj_on_press),
    JERRY_CFUNC_LIST_END(),
};

/************************************************************************
* Native event handler for LVGL
*************************************************************************/

static void js_lv_obj_event_cb(lv_event_t *e) {

    /* Get the JavaScript callback from the user data */
    jerry_value_t js_callback = (jerry_value_t)(uintptr_t)lv_event_get_user_data(e);

    /* Call the JS callback with the event code as an argument */
    jerry_value_t args[1];
    args[0] = jerry_number(lv_event_get_code(e)); // Pass event code to JS
    jerry_value_t result = jerry_call(js_callback, jerry_undefined(), args, 1);
    jerry_value_free(result);
    jerry_value_free(args[0]);
}

/************************************************************************
* Constructor & Desctructor
*************************************************************************/

static void js_lv_clear_user_data_cb(lv_obj_t *obj) {

    jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    if (user_data != NULL) {
        if (user_data->name != NULL) {
            free(user_data->name);
        }
        free(user_data);
        lv_obj_set_user_data(obj, NULL);
    }
}

static void js_lv_obj_destructor_cb(void *native_p, jerry_object_native_info_t *call_info_p) {
    printf("%s %s\n", LV_OBJ_NAME, __FUNCTION__);
    lv_obj_t *obj = (lv_obj_t *) native_p;
    jr_lvgl_obj_desctruct(obj, &js_lv_clear_user_data_cb);
}

static jerry_value_t js_lv_obj_constructor(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    printf("%s %s\n", LV_OBJ_NAME, __FUNCTION__);
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object .");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, parent, args[0], NULL);
    lv_obj_t *obj = LV_OBJ_CREATE(parent);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to create button");
    }

    jerry_object_set_native_ptr(call_info_p->this_value, /* jerry_value_t object */
                                &jerry_obj_native_info,  /* const jerry_object_native_info_t *native_info_p */
                                obj                      /* void *native_pointer_p */
                                );

    /* Register the methods dynamically when create a new object */
    jr_register_obj_method(call_info_p->this_value, methods);

    return jerry_undefined();
}

/************************************************************************
* Class register functions
*************************************************************************/

void jr_lv_bar_init(void) {
    jerry_value_t global_obj = jerry_current_realm();
    jerry_value_t constructor = jerry_function_external(js_lv_obj_constructor);
    jerry_value_t constructor_name = jerry_string_sz(LV_OBJ_NAME);
    jerry_object_set(global_obj, constructor_name, constructor);
    jerry_value_free(constructor_name);
    jerry_value_free(constructor);
    jerry_value_free(global_obj);
}
