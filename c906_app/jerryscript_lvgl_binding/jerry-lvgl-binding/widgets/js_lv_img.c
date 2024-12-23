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
#define LV_OBJ_NAME           "Img"
#define LV_OBJ_CREATE(parent) lv_img_create(parent)

/************************************************************************
* Typedef
*************************************************************************/

typedef struct {
    void *img_src;
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

static jerry_value_t js_lv_img_set_src(const jerry_call_info_t *call_info_p,
                                       const jerry_value_t args[],
                                       const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_array(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (src as array)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    jerry_value_t src_array = args[0];

    // Extract width
    jerry_value_t width_val = jerry_object_get_index(src_array, 0);
    if (!jerry_value_is_number(width_val)) {
        jerry_value_free(width_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Width should be a number");
    }
    lv_coord_t width = (lv_coord_t)jerry_value_as_number(width_val);
    jerry_value_free(width_val);

    // Extract height
    jerry_value_t height_val = jerry_object_get_index(src_array, 1);
    if (!jerry_value_is_number(height_val)) {
        jerry_value_free(height_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Height should be a number");
    }
    lv_coord_t height = (lv_coord_t)jerry_value_as_number(height_val);
    jerry_value_free(height_val);

    // Extract data buffer
    jerry_value_t data_val = jerry_object_get_index(src_array, 2);
    if (!jerry_value_is_arraybuffer(data_val)) {
        jerry_value_free(data_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Data should be an ArrayBuffer");
    }

    jerry_length_t buffer_size = jerry_arraybuffer_size(data_val);
    size_t expected_size = width * height * sizeof(lv_color_t);
    if (buffer_size != expected_size) {
        jerry_value_free(data_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Buffer size does not match width * height * sizeof(lv_color_t)");
    }

    void *buffer = malloc(buffer_size);
    if (!buffer) {
        jerry_value_free(data_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    jerry_arraybuffer_read(data_val, 0, (uint8_t *)buffer, buffer_size);
    jerry_value_free(data_val);

    // Create image descriptor
    static lv_img_dsc_t img_dsc;
    memset(&img_dsc, 0, sizeof(img_dsc));
    img_dsc.header.always_zero = 0;
    img_dsc.header.w = width;
    img_dsc.header.h = height;
    img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;
    img_dsc.data_size = buffer_size;
    img_dsc.data = (const uint8_t *)buffer;

    /* store img_src pointer into user data */
    jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    if(user_data != NULL){
       /* this prevents from set src second time */
       if(user_data->img_src != NULL)
       {
           free(user_data->img_src);
       }
       user_data->img_src = buffer;
    }

    lv_img_set_src(obj, &img_dsc);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_set_offset_x(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);

    lv_img_set_offset_x(obj, x);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_set_offset_y(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (y)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[0]);

    lv_img_set_offset_y(obj, y);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_set_angle(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (angle)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    int16_t angle = (int16_t)jerry_value_as_number(args[0]);

    lv_img_set_angle(obj, angle);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_set_pivot(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);

    lv_img_set_pivot(obj, x, y);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_set_zoom(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (zoom)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    uint16_t zoom = (uint16_t)jerry_value_as_number(args[0]);

    lv_img_set_zoom(obj, zoom);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_set_antialias(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_boolean(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (antialias)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    bool antialias = jerry_value_is_true(args[0]);

    lv_img_set_antialias(obj, antialias);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_set_size_mode(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (mode)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_img_size_mode_t mode = (lv_img_size_mode_t)jerry_value_as_number(args[0]);

    lv_img_set_size_mode(obj, mode);

    return jerry_undefined();
}

static jerry_value_t js_lv_img_get_src(const jerry_call_info_t *call_info_p,
                                       const jerry_value_t args[],
                                       const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    const void *src = lv_img_get_src(obj);
    if (src == NULL) {
        return jerry_undefined();
    }

    const lv_img_dsc_t *img_dsc = (const lv_img_dsc_t *)src;
    if (img_dsc->data == NULL || img_dsc->data_size == 0) {
        return jerry_undefined();
    }

    size_t struct_size = sizeof(lv_img_dsc_t);
    size_t total_size = struct_size + img_dsc->data_size;

    void *buffer = malloc(total_size);
    if (!buffer) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    memcpy(buffer, img_dsc, struct_size);
    memcpy((uint8_t *)buffer + struct_size, img_dsc->data, img_dsc->data_size);

    jerry_value_t js_buffer = jerry_arraybuffer(total_size);
    jerry_arraybuffer_write(js_buffer, 0, (const uint8_t *)buffer, total_size);

    free(buffer);

    return js_buffer;
}

static jerry_value_t js_lv_img_get_offset_x(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_coord_t offset_x = lv_img_get_offset_x(obj);

    return jerry_number(offset_x);
}

static jerry_value_t js_lv_img_get_offset_y(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_coord_t offset_y = lv_img_get_offset_y(obj);

    return jerry_number(offset_y);
}

static jerry_value_t js_lv_img_get_angle(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    uint16_t angle = lv_img_get_angle(obj);

    return jerry_number(angle);
}

static jerry_value_t js_lv_img_get_pivot(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_point_t pivot;
    lv_img_get_pivot(obj, &pivot);

    jerry_value_t pivot_obj = jerry_object();
    jerry_object_set(pivot_obj, jerry_string_sz("x"), jerry_number(pivot.x));
    jerry_object_set(pivot_obj, jerry_string_sz("y"), jerry_number(pivot.y));

    return pivot_obj;
}

static jerry_value_t js_lv_img_get_zoom(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    uint16_t zoom = lv_img_get_zoom(obj);

    return jerry_number(zoom);
}

static jerry_value_t js_lv_img_get_antialias(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    bool antialias = lv_img_get_antialias(obj);

    return jerry_boolean(antialias);
}

static jerry_value_t js_lv_img_get_size_mode(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid image object");
    }

    lv_img_size_mode_t mode = lv_img_get_size_mode(obj);

    return jerry_number((int32_t)mode);
}

/************************************************************************
* Protperties, methodes entry list
*************************************************************************/

static const jerry_cfunc_entry_t methods[] = {
    JERRY_CFUNC_ENTRY("align",        js_lv_obj_align),
    JERRY_CFUNC_ENTRY("setSize",      js_lv_obj_set_size),
    JERRY_CFUNC_ENTRY("setStyle",  js_lv_obj_set_style),
    JERRY_CFUNC_ENTRY("onPress",      js_lv_obj_on_press),
    JERRY_CFUNC_ENTRY("setSrc",       js_lv_img_set_src),
    JERRY_CFUNC_ENTRY("setOffsetX",   js_lv_img_set_offset_x),
    JERRY_CFUNC_ENTRY("setOffsetY",   js_lv_img_set_offset_y),
    JERRY_CFUNC_ENTRY("setAngle",     js_lv_img_set_angle),
    JERRY_CFUNC_ENTRY("setPivot",     js_lv_img_set_pivot),
    JERRY_CFUNC_ENTRY("setZoom",      js_lv_img_set_zoom),
    JERRY_CFUNC_ENTRY("setAntialias", js_lv_img_set_antialias),
    JERRY_CFUNC_ENTRY("setSizeMode",  js_lv_img_set_size_mode),
    JERRY_CFUNC_ENTRY("getSrc",       js_lv_img_get_src),
    JERRY_CFUNC_ENTRY("getOffsetX",   js_lv_img_get_offset_x),
    JERRY_CFUNC_ENTRY("getOffsetY",   js_lv_img_get_offset_y),
    JERRY_CFUNC_ENTRY("getAngle",     js_lv_img_get_angle),
    JERRY_CFUNC_ENTRY("getPivot",     js_lv_img_get_pivot),
    JERRY_CFUNC_ENTRY("getZoom",      js_lv_img_get_zoom),
    JERRY_CFUNC_ENTRY("getAntialias", js_lv_img_get_antialias),
    JERRY_CFUNC_ENTRY("getSizeMode",  js_lv_img_get_size_mode),
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
        if (user_data->img_src != NULL) {
            free(user_data->img_src);
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

    /* user data setting example */
    jerry_lv_user_data_t *user_data = malloc(sizeof(jerry_lv_user_data_t));
    if(user_data == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to allocate userdata");
    }
    lv_obj_set_user_data(obj, user_data);

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

void jr_lv_img_init(void) {
    jerry_value_t global_obj = jerry_current_realm();
    jerry_value_t constructor = jerry_function_external(js_lv_obj_constructor);
    jerry_value_t constructor_name = jerry_string_sz(LV_OBJ_NAME);
    jerry_object_set(global_obj, constructor_name, constructor);
    jerry_value_free(constructor_name);
    jerry_value_free(constructor);
    jerry_value_free(global_obj);
}
