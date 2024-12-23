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
#define LV_OBJ_NAME           "AnalogClock"
#define LV_OBJ_CREATE(parent) lv_analogclock_create(parent)

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

static jerry_value_t js_lv_obj_on_changed(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_function(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected a callback function");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if(obj == NULL) {
       return jerry_undefined();
    }

    lv_obj_add_event_cb(obj, js_lv_obj_event_cb, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)args[0]);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_add_scale(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid analog clock object");
    }

    lv_analogclock_scale_t *scale = lv_analogclock_add_scale(obj);
    if (scale == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to add scale");
    }

    return jerry_number((uintptr_t)scale);
}

static jerry_value_t js_lv_analogclock_set_ticks(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (width, len, color)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid analog clock object");
    }

    uint16_t width = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t len = (uint16_t)jerry_value_as_number(args[1]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[2]);
    lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);

    lv_analogclock_set_ticks(obj, width, len, color);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_major_ticks(const jerry_call_info_t *call_info_p,
                                                      const jerry_value_t args[],
                                                      const jerry_length_t args_count) {
    if (args_count < 4 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (width, len, color, label_gap)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid analog clock object");
    }

    uint16_t width = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t len = (uint16_t)jerry_value_as_number(args[1]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[2]);
    lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);
    int16_t label_gap = (int16_t)jerry_value_as_number(args[3]);

    lv_analogclock_set_major_ticks(obj, width, len, color, label_gap);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_scale_range(const jerry_call_info_t *call_info_p,
                                                      const jerry_value_t args[],
                                                      const jerry_length_t args_count) {
    if (args_count < 5 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3]) || !jerry_value_is_number(args[4])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (scale, min, max, angle_range, rotation)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid analog clock object");
    }

    lv_analogclock_scale_t *scale = (lv_analogclock_scale_t *)(uintptr_t)jerry_value_as_number(args[0]);
    int32_t min = (int32_t)jerry_value_as_number(args[1]);
    int32_t max = (int32_t)jerry_value_as_number(args[2]);
    uint32_t angle_range = (uint32_t)jerry_value_as_number(args[3]);
    uint32_t rotation = (uint32_t)jerry_value_as_number(args[4]);

    lv_analogclock_set_scale_range(obj, scale, min, max, angle_range, rotation);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_hide_digits(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_boolean(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (hide_digits)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid analog clock object");
    }

    bool hide_digits = jerry_value_to_boolean(args[0]);
    lv_analogclock_hide_digits(obj, hide_digits);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_add_needle_line(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    if (args_count < 4 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2]) ||
        !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (scale, width, color, r_mod)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid analog clock object");
    }

    JERRY_GET_NATIVE_PTR(lv_analogclock_scale_t, scale, args[0], &jerry_obj_native_info);
    if (!scale) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid scale object");
    }

    uint16_t width = (uint16_t)jerry_value_as_number(args[1]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[2]);
    lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);
    int16_t r_mod = (int16_t)jerry_value_as_number(args[3]);

    lv_analogclock_indicator_t *indicator = lv_analogclock_add_needle_line(obj, scale, width, color, r_mod);

    if (!indicator) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to add needle line");
    }

    jerry_value_t js_indicator = jerry_object();
    jerry_object_set_native_ptr(js_indicator, &jerry_obj_native_info, indicator);

    return js_indicator;
}

static jerry_value_t js_lv_analogclock_set_hour_needle_line(const jerry_call_info_t *call_info_p,
                                                            const jerry_value_t args[],
                                                            const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) ||
        !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (width, color, r_mod)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid analog clock object");
    }

    uint16_t width = (uint16_t)jerry_value_as_number(args[0]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[1]);
    lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);
    int16_t r_mod = (int16_t)jerry_value_as_number(args[2]);

    lv_analogclock_set_hour_needle_line(obj, width, color, r_mod);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_min_needle_line(const jerry_call_info_t *call_info_p,
                                                           const jerry_value_t args[],
                                                           const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (width, color, r_mod)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    uint16_t width = (uint16_t)jerry_value_as_number(args[0]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[1]);
    lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);
    int16_t r_mod = (int16_t)jerry_value_as_number(args[2]);

    lv_analogclock_set_min_needle_line(obj, width, color, r_mod);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_sec_needle_line(const jerry_call_info_t *call_info_p,
                                                           const jerry_value_t args[],
                                                           const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (width, color, r_mod)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    uint16_t width = (uint16_t)jerry_value_as_number(args[0]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[1]);
    lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);
    int16_t r_mod = (int16_t)jerry_value_as_number(args[2]);

    lv_analogclock_set_sec_needle_line(obj, width, color, r_mod);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_add_needle_img(const jerry_call_info_t *call_info_p,
                                                      const jerry_value_t args[],
                                                      const jerry_length_t args_count) {
    if (args_count < 4 || !jerry_value_is_object(args[0]) || !jerry_value_is_string(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (scale, src, pivot_x, pivot_y)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    JERRY_GET_NATIVE_PTR(lv_analogclock_scale_t, scale, args[0], &jerry_obj_native_info);
    if (!scale) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid scale");
    }

    jerry_size_t src_size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
    char *src = malloc(src_size + 1);
    if (!src) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)src, src_size);
    src[src_size] = '\0';

    lv_coord_t pivot_x = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t pivot_y = (lv_coord_t)jerry_value_as_number(args[3]);

    lv_analogclock_indicator_t *needle = lv_analogclock_add_needle_img(obj, scale, src, pivot_x, pivot_y);

    free(src);

    if (!needle) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to add needle image");
    }

    jerry_value_t js_needle = jerry_object();
    jerry_object_set_native_ptr(js_needle, &jerry_obj_native_info, needle);

    return js_needle;
}

static jerry_value_t js_lv_analogclock_set_hour_needle_img(const jerry_call_info_t *call_info_p,
                                                           const jerry_value_t args[],
                                                           const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_object(args[0]) || !jerry_value_is_string(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (src, pivot_x, pivot_y)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    jerry_size_t src_size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
    char *src = malloc(src_size + 1);
    if (!src) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)src, src_size);
    src[src_size] = '\0';

    lv_coord_t pivot_x = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t pivot_y = (lv_coord_t)jerry_value_as_number(args[3]);

    lv_analogclock_set_hour_needle_img(obj, src, pivot_x, pivot_y);

    free(src);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_min_needle_img(const jerry_call_info_t *call_info_p,
                                                          const jerry_value_t args[],
                                                          const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_object(args[0]) || !jerry_value_is_string(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (src, pivot_x, pivot_y)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    jerry_size_t src_size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
    char *src = malloc(src_size + 1);
    if (!src) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)src, src_size);
    src[src_size] = '\0';

    lv_coord_t pivot_x = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t pivot_y = (lv_coord_t)jerry_value_as_number(args[3]);

    lv_analogclock_set_min_needle_img(obj, src, pivot_x, pivot_y);

    free(src);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_sec_needle_img(const jerry_call_info_t *call_info_p,
                                                          const jerry_value_t args[],
                                                          const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_object(args[0]) || !jerry_value_is_string(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (src, pivot_x, pivot_y)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    jerry_size_t src_size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
    char *src = malloc(src_size + 1);
    if (!src) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)src, src_size);
    src[src_size] = '\0';

    lv_coord_t pivot_x = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t pivot_y = (lv_coord_t)jerry_value_as_number(args[3]);

    lv_analogclock_set_sec_needle_img(obj, src, pivot_x, pivot_y);

    free(src);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_add_arc(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (width, color, r_mod)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    uint16_t width = (uint16_t)jerry_value_as_number(args[1]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[2]);
    lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);
    int16_t r_mod = (int16_t)jerry_value_as_number(args[3]);

    lv_analogclock_indicator_t *indicator = lv_analogclock_add_arc(obj, width, color, r_mod);

    if (!indicator) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to add arc");
    }

    jerry_value_t js_indicator = jerry_object();
    jerry_object_set_native_ptr(js_indicator, &jerry_obj_native_info, indicator);

    return js_indicator;
}

static jerry_value_t js_lv_analogclock_add_scale_lines(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    if (args_count < 5 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_boolean(args[3]) ||
        !jerry_value_is_number(args[4])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (color_start, color_end, local, width_mod)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    uint32_t color_start_value = (uint32_t)jerry_value_as_number(args[1]);
    lv_color_t color_start = lv_color_make((color_start_value >> 16) & 0xFF, (color_start_value >> 8) & 0xFF,
                                           color_start_value & 0xFF);

    uint32_t color_end_value = (uint32_t)jerry_value_as_number(args[2]);
    lv_color_t color_end = lv_color_make((color_end_value >> 16) & 0xFF, (color_end_value >> 8) & 0xFF, color_end_value & 0xFF);

    bool local = jerry_value_is_true(args[3]);
    int16_t width_mod = (int16_t)jerry_value_as_number(args[4]);

    lv_analogclock_indicator_t *indicator = lv_analogclock_add_scale_lines(obj, color_start, color_end, local, width_mod);

    if (!indicator) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to add scale lines");
    }

    jerry_value_t js_indicator = jerry_object();
    jerry_object_set_native_ptr(js_indicator, &jerry_obj_native_info, indicator);

    return js_indicator;
}

static jerry_value_t js_lv_analogclock_set_indicator_value(const jerry_call_info_t *call_info_p,
                                                          const jerry_value_t args[],
                                                          const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (indic, value)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    JERRY_GET_NATIVE_PTR(lv_analogclock_indicator_t, indic, args[0], &jerry_obj_native_info);
    if (!indic) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid indicator");
    }

    int32_t value = (int32_t)jerry_value_as_number(args[1]);
    lv_analogclock_set_indicator_value(obj, indic, value);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_indicator_start_value(const jerry_call_info_t *call_info_p,
                                                                 const jerry_value_t args[],
                                                                 const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (indic, start_value)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    JERRY_GET_NATIVE_PTR(lv_analogclock_indicator_t, indic, args[0], &jerry_obj_native_info);
    if (!indic) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid indicator");
    }

    int32_t start_value = (int32_t)jerry_value_as_number(args[1]);
    lv_analogclock_set_indicator_start_value(obj, indic, start_value);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_indicator_end_value(const jerry_call_info_t *call_info_p,
                                                               const jerry_value_t args[],
                                                               const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (indic, end_value)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    JERRY_GET_NATIVE_PTR(lv_analogclock_indicator_t, indic, args[0], &jerry_obj_native_info);
    if (!indic) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid indicator");
    }

    int32_t end_value = (int32_t)jerry_value_as_number(args[1]);
    lv_analogclock_set_indicator_end_value(obj, indic, end_value);

    return jerry_undefined();
}

static jerry_value_t js_lv_analogclock_set_time(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (hour, min, sec)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (!obj) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid object");
    }

    int32_t hour = (int32_t)jerry_value_as_number(args[0]);
    int32_t min = (int32_t)jerry_value_as_number(args[1]);
    int32_t sec = (int32_t)jerry_value_as_number(args[2]);

    lv_analogclock_set_time(obj, hour, min, sec);

    return jerry_undefined();
}

/************************************************************************
* Protperties, methodes entry list
*************************************************************************/

static const jerry_cfunc_entry_t methods[] = {
    JERRY_CFUNC_ENTRY("align",                  js_lv_obj_align),
    JERRY_CFUNC_ENTRY("setSize",                js_lv_obj_set_size),
    JERRY_CFUNC_ENTRY("setStyle",               js_lv_obj_set_style),
    JERRY_CFUNC_ENTRY("onChanged",              js_lv_obj_on_changed),
    JERRY_CFUNC_ENTRY("addScale",               js_lv_analogclock_add_scale),
    JERRY_CFUNC_ENTRY("setTicks",               js_lv_analogclock_set_ticks),
    JERRY_CFUNC_ENTRY("setMajorTicks",          js_lv_analogclock_set_major_ticks),
    JERRY_CFUNC_ENTRY("setScaleRange",          js_lv_analogclock_set_scale_range),
    JERRY_CFUNC_ENTRY("hideDigits",             js_lv_analogclock_hide_digits),
    JERRY_CFUNC_ENTRY("addNeedleLine",          js_lv_analogclock_add_needle_line),
    JERRY_CFUNC_ENTRY("setHourNeedleLine",      js_lv_analogclock_set_hour_needle_line),
    JERRY_CFUNC_ENTRY("setMinNeedleLine",       js_lv_analogclock_set_min_needle_line),
    JERRY_CFUNC_ENTRY("setSecNeedleLine",       js_lv_analogclock_set_sec_needle_line),
    JERRY_CFUNC_ENTRY("addNeedleImg",           js_lv_analogclock_add_needle_img),
    JERRY_CFUNC_ENTRY("setHourNeedleImg",       js_lv_analogclock_set_hour_needle_img),
    JERRY_CFUNC_ENTRY("setMinNeedleImg",        js_lv_analogclock_set_min_needle_img),
    JERRY_CFUNC_ENTRY("setSecNeedleImg",        js_lv_analogclock_set_sec_needle_img),
    JERRY_CFUNC_ENTRY("addArc",                 js_lv_analogclock_add_arc),
    JERRY_CFUNC_ENTRY("addScaleLines",          js_lv_analogclock_add_scale_lines),
    JERRY_CFUNC_ENTRY("setIndicatorValue",      js_lv_analogclock_set_indicator_value),
    JERRY_CFUNC_ENTRY("setIndicatorStartValue", js_lv_analogclock_set_indicator_start_value),
    JERRY_CFUNC_ENTRY("setIndicatorEndValue",   js_lv_analogclock_set_indicator_end_value),
    JERRY_CFUNC_ENTRY("setTime",                js_lv_analogclock_set_time),
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

    /* user data setting example */
    jerry_lv_user_data_t *user_data = malloc(sizeof(jerry_lv_user_data_t));
    if(user_data == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to allocate userdata");
    }
    lv_obj_set_user_data(obj, user_data);

    /* later we can set user data as bellow */
    // jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    // if(user_data != NULL){
    //    /* this avoid memory leakage */
    //    if (user_data->name != NULL) {
    //        free(user_data->name);
    //    }
    //    user_data->value = 1;
    //    user_data->name = strdup("Some text");
    // }

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

void jr_lv_analogclock_init(void) {
    jerry_value_t global_obj = jerry_current_realm();
    jerry_value_t constructor = jerry_function_external(js_lv_obj_constructor);
    jerry_value_t constructor_name = jerry_string_sz(LV_OBJ_NAME);
    jerry_object_set(global_obj, constructor_name, constructor);
    jerry_value_free(constructor_name);
    jerry_value_free(constructor);
    jerry_value_free(global_obj);
}
