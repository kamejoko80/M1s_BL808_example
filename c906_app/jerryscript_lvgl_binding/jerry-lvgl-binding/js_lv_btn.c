/*
 * LVGL Button JerryScript Bindings
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

static jerry_value_t js_lv_obj_set_size(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_cnt) {
    if (args_cnt < 3 || !jerry_value_is_object(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected (obj, width, height)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, args[0], NULL);
    lv_coord_t width = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t height = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_obj_set_size(obj, width, height);
    return jerry_undefined();
}

static jerry_value_t js_lv_obj_align(const jerry_call_info_t *call_info_p,
                                     const jerry_value_t args[],
                                     const jerry_length_t args_cnt) {
    if (args_cnt < 4 || !jerry_value_is_object(args[0]) ||
        !jerry_value_is_number(args[1]) ||
        !jerry_value_is_number(args[2]) ||
        !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected (obj, align, x_ofs, y_ofs)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, args[0], NULL);
    lv_align_t align = (lv_align_t)jerry_value_as_number(args[1]);
    lv_coord_t x_ofs = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t y_ofs = (lv_coord_t)jerry_value_as_number(args[3]);
    lv_obj_align(obj, align, x_ofs, y_ofs);
    return jerry_undefined();
}

static jerry_value_t js_lv_btn_create(const jerry_call_info_t *call_info_p,
                                      const jerry_value_t args[],
                                      const jerry_length_t args_cnt) {
    if (args_cnt < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Parent object is required.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, parent, args[0], NULL);
    lv_obj_t *btn = lv_btn_create(parent);
    if (btn == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to create button.");
    }
    jerry_value_t js_btn = jerry_object();
    jerry_object_set_native_ptr(js_btn, NULL, btn);
    return js_btn;
}

static const jerry_cfunc_entry_t jerry_cfunc_entry_list[] = {
    JERRY_CFUNC_ENTRY("lv_obj_set_size", js_lv_obj_set_size),
    JERRY_CFUNC_ENTRY("lv_obj_align",    js_lv_obj_align),
    JERRY_CFUNC_ENTRY("lv_btn_create",   js_lv_btn_create),
};

void jr_lv_btn_init(void) {
    jr_register_cfunc_list(jerry_cfunc_entry_list, sizeof(jerry_cfunc_entry_list)/sizeof(jerry_cfunc_entry_t));
}
