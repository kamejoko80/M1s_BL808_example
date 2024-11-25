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

static const jerry_const_entry_t jerry_constant_entry_list[] = {
    JERRY_CONST_ENTRY("LV_ALIGN_DEFAULT",          LV_ALIGN_DEFAULT),
    JERRY_CONST_ENTRY("LV_ALIGN_TOP_LEFT",         LV_ALIGN_TOP_LEFT),
    JERRY_CONST_ENTRY("LV_ALIGN_TOP_MID",          LV_ALIGN_TOP_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_TOP_RIGHT",        LV_ALIGN_TOP_RIGHT),
    JERRY_CONST_ENTRY("LV_ALIGN_BOTTOM_LEFT",      LV_ALIGN_BOTTOM_LEFT),
    JERRY_CONST_ENTRY("LV_ALIGN_BOTTOM_MID",       LV_ALIGN_BOTTOM_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_BOTTOM_RIGHT",     LV_ALIGN_BOTTOM_RIGHT),
    JERRY_CONST_ENTRY("LV_ALIGN_LEFT_MID",         LV_ALIGN_LEFT_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_RIGHT_MID",        LV_ALIGN_RIGHT_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_CENTER",           LV_ALIGN_CENTER),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_TOP_LEFT",     LV_ALIGN_OUT_TOP_LEFT),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_TOP_MID",      LV_ALIGN_OUT_TOP_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_TOP_RIGHT",    LV_ALIGN_OUT_TOP_RIGHT),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_BOTTOM_LEFT",  LV_ALIGN_OUT_BOTTOM_LEFT),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_BOTTOM_MID",   LV_ALIGN_OUT_BOTTOM_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_BOTTOM_RIGHT", LV_ALIGN_OUT_BOTTOM_RIGHT),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_LEFT_TOP",     LV_ALIGN_OUT_LEFT_TOP),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_LEFT_MID",     LV_ALIGN_OUT_LEFT_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_LEFT_BOTTOM",  LV_ALIGN_OUT_LEFT_BOTTOM),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_RIGHT_TOP",    LV_ALIGN_OUT_RIGHT_TOP),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_RIGHT_MID",    LV_ALIGN_OUT_RIGHT_MID),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_RIGHT_BOTTOM", LV_ALIGN_OUT_RIGHT_BOTTOM),
    JERRY_CONST_LIST_END(),
};

void jr_gbl_const_init(void)
{
    jerry_value_t global_obj = jerry_current_realm();
    jr_register_global_constant_list(global_obj, jerry_constant_entry_list);
    jerry_value_free(global_obj);
}

