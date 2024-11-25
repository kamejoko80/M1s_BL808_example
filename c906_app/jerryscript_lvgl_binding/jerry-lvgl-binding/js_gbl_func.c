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
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

static jerry_value_t js_lv_scr_act(const jerry_call_info_t *call_info_p,
                                   const jerry_value_t args[],
                                   const jerry_length_t args_count) {
    lv_obj_t *screen = lv_scr_act();
    jerry_value_t js_screen = jerry_object();
    jerry_object_set_native_ptr(js_screen, NULL, screen);

    return js_screen;
}

static const jerry_cfunc_entry_t jerry_cfunc_entry_list[] = {
    JERRY_CFUNC_ENTRY("lv_scr_act", js_lv_scr_act),
    JERRY_CFUNC_LIST_END() ,
};

void jr_gbl_func_init(void) {
    jr_register_cfunc_list(jerry_cfunc_entry_list);
}    