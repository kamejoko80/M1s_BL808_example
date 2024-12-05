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
    JERRY_CONST_ENTRY("LV_ALIGN_DEFAULT",                  LV_ALIGN_DEFAULT                 ),
    JERRY_CONST_ENTRY("LV_ALIGN_TOP_LEFT",                 LV_ALIGN_TOP_LEFT                ),
    JERRY_CONST_ENTRY("LV_ALIGN_TOP_MID",                  LV_ALIGN_TOP_MID                 ),
    JERRY_CONST_ENTRY("LV_ALIGN_TOP_RIGHT",                LV_ALIGN_TOP_RIGHT               ),
    JERRY_CONST_ENTRY("LV_ALIGN_BOTTOM_LEFT",              LV_ALIGN_BOTTOM_LEFT             ),
    JERRY_CONST_ENTRY("LV_ALIGN_BOTTOM_MID",               LV_ALIGN_BOTTOM_MID              ),
    JERRY_CONST_ENTRY("LV_ALIGN_BOTTOM_RIGHT",             LV_ALIGN_BOTTOM_RIGHT            ),
    JERRY_CONST_ENTRY("LV_ALIGN_LEFT_MID",                 LV_ALIGN_LEFT_MID                ),
    JERRY_CONST_ENTRY("LV_ALIGN_RIGHT_MID",                LV_ALIGN_RIGHT_MID               ),
    JERRY_CONST_ENTRY("LV_ALIGN_CENTER",                   LV_ALIGN_CENTER                  ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_TOP_LEFT",             LV_ALIGN_OUT_TOP_LEFT            ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_TOP_MID",              LV_ALIGN_OUT_TOP_MID             ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_TOP_RIGHT",            LV_ALIGN_OUT_TOP_RIGHT           ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_BOTTOM_LEFT",          LV_ALIGN_OUT_BOTTOM_LEFT         ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_BOTTOM_MID",           LV_ALIGN_OUT_BOTTOM_MID          ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_BOTTOM_RIGHT",         LV_ALIGN_OUT_BOTTOM_RIGHT        ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_LEFT_TOP",             LV_ALIGN_OUT_LEFT_TOP            ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_LEFT_MID",             LV_ALIGN_OUT_LEFT_MID            ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_LEFT_BOTTOM",          LV_ALIGN_OUT_LEFT_BOTTOM         ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_RIGHT_TOP",            LV_ALIGN_OUT_RIGHT_TOP           ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_RIGHT_MID",            LV_ALIGN_OUT_RIGHT_MID           ),
    JERRY_CONST_ENTRY("LV_ALIGN_OUT_RIGHT_BOTTOM",         LV_ALIGN_OUT_RIGHT_BOTTOM        ),
    JERRY_CONST_ENTRY("LV_ARC_MODE_NORMAL",                LV_ARC_MODE_NORMAL               ),
    JERRY_CONST_ENTRY("LV_ARC_MODE_SYMMETRICAL",           LV_ARC_MODE_SYMMETRICAL          ),
    JERRY_CONST_ENTRY("LV_ARC_MODE_REVERSE",               LV_ARC_MODE_REVERSE              ),
    JERRY_CONST_ENTRY("LV_ARC_DRAW_PART_BACKGROUND",       LV_ARC_DRAW_PART_BACKGROUND      ),
    JERRY_CONST_ENTRY("LV_ARC_DRAW_PART_FOREGROUND",       LV_ARC_DRAW_PART_FOREGROUND      ),
    JERRY_CONST_ENTRY("LV_ARC_DRAW_PART_KNOB",             LV_ARC_DRAW_PART_KNOB            ),
    JERRY_CONST_ENTRY("LV_BAR_MODE_NORMAL",                LV_BAR_MODE_NORMAL               ),
    JERRY_CONST_ENTRY("LV_BAR_MODE_SYMMETRICAL",           LV_BAR_MODE_SYMMETRICAL          ),
    JERRY_CONST_ENTRY("LV_BAR_MODE_RANGE",                 LV_BAR_MODE_RANGE                ),
    JERRY_CONST_ENTRY("LV_BAR_DRAW_PART_INDICATOR",        LV_BAR_DRAW_PART_INDICATOR       ),
    JERRY_CONST_ENTRY("LV_ANIM_OFF",                       LV_ANIM_OFF                      ),
    JERRY_CONST_ENTRY("LV_ANIM_ON",                        LV_ANIM_ON                       ),
    JERRY_CONST_ENTRY("LV_EVENT_ALL",                      LV_EVENT_ALL                     ),
    JERRY_CONST_ENTRY("LV_EVENT_PRESSED",                  LV_EVENT_PRESSED                 ),
    JERRY_CONST_ENTRY("LV_EVENT_PRESSING",                 LV_EVENT_PRESSING                ),
    JERRY_CONST_ENTRY("LV_EVENT_PRESS_LOST",               LV_EVENT_PRESS_LOST              ),
    JERRY_CONST_ENTRY("LV_EVENT_SHORT_CLICKED",            LV_EVENT_SHORT_CLICKED           ),
    JERRY_CONST_ENTRY("LV_EVENT_LONG_PRESSED",             LV_EVENT_LONG_PRESSED            ),
    JERRY_CONST_ENTRY("LV_EVENT_LONG_PRESSED_REPEAT",      LV_EVENT_LONG_PRESSED_REPEAT     ),
    JERRY_CONST_ENTRY("LV_EVENT_CLICKED",                  LV_EVENT_CLICKED                 ),
    JERRY_CONST_ENTRY("LV_EVENT_RELEASED",                 LV_EVENT_RELEASED                ),
    JERRY_CONST_ENTRY("LV_EVENT_SCROLL_BEGIN",             LV_EVENT_SCROLL_BEGIN            ),
    JERRY_CONST_ENTRY("LV_EVENT_SCROLL_END",               LV_EVENT_SCROLL_END              ),
    JERRY_CONST_ENTRY("LV_EVENT_SCROLL",                   LV_EVENT_SCROLL                  ),
    JERRY_CONST_ENTRY("LV_EVENT_GESTURE",                  LV_EVENT_GESTURE                 ),
    JERRY_CONST_ENTRY("LV_EVENT_KEY",                      LV_EVENT_KEY                     ),
    JERRY_CONST_ENTRY("LV_EVENT_FOCUSED",                  LV_EVENT_FOCUSED                 ),
    JERRY_CONST_ENTRY("LV_EVENT_DEFOCUSED",                LV_EVENT_DEFOCUSED               ),
    JERRY_CONST_ENTRY("LV_EVENT_LEAVE",                    LV_EVENT_LEAVE                   ),
    JERRY_CONST_ENTRY("LV_EVENT_HIT_TEST",                 LV_EVENT_HIT_TEST                ),
    JERRY_CONST_ENTRY("LV_EVENT_COVER_CHECK",              LV_EVENT_COVER_CHECK             ),
    JERRY_CONST_ENTRY("LV_EVENT_REFR_EXT_DRAW_SIZE",       LV_EVENT_REFR_EXT_DRAW_SIZE      ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_MAIN_BEGIN",          LV_EVENT_DRAW_MAIN_BEGIN         ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_MAIN",                LV_EVENT_DRAW_MAIN               ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_MAIN_END",            LV_EVENT_DRAW_MAIN_END           ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_POST_BEGIN",          LV_EVENT_DRAW_POST_BEGIN         ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_POST",                LV_EVENT_DRAW_POST               ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_POST_END",            LV_EVENT_DRAW_POST_END           ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_PART_BEGIN",          LV_EVENT_DRAW_PART_BEGIN         ),
    JERRY_CONST_ENTRY("LV_EVENT_DRAW_PART_END",            LV_EVENT_DRAW_PART_END           ),
    JERRY_CONST_ENTRY("LV_EVENT_VALUE_CHANGED",            LV_EVENT_VALUE_CHANGED           ),
    JERRY_CONST_ENTRY("LV_EVENT_INSERT",                   LV_EVENT_INSERT                  ),
    JERRY_CONST_ENTRY("LV_EVENT_REFRESH",                  LV_EVENT_REFRESH                 ),
    JERRY_CONST_ENTRY("LV_EVENT_READY",                    LV_EVENT_READY                   ),
    JERRY_CONST_ENTRY("LV_EVENT_CANCEL",                   LV_EVENT_CANCEL                  ),
    JERRY_CONST_ENTRY("LV_EVENT_DELETE",                   LV_EVENT_DELETE                  ),
    JERRY_CONST_ENTRY("LV_EVENT_CHILD_CHANGED",            LV_EVENT_CHILD_CHANGED           ),
    JERRY_CONST_ENTRY("LV_EVENT_CHILD_CREATED",            LV_EVENT_CHILD_CREATED           ),
    JERRY_CONST_ENTRY("LV_EVENT_CHILD_DELETED",            LV_EVENT_CHILD_DELETED           ),
    JERRY_CONST_ENTRY("LV_EVENT_SCREEN_UNLOAD_START",      LV_EVENT_SCREEN_UNLOAD_START     ),
    JERRY_CONST_ENTRY("LV_EVENT_SCREEN_LOAD_START",        LV_EVENT_SCREEN_LOAD_START       ),
    JERRY_CONST_ENTRY("LV_EVENT_SCREEN_LOADED",            LV_EVENT_SCREEN_LOADED           ),
    JERRY_CONST_ENTRY("LV_EVENT_SCREEN_UNLOADED",          LV_EVENT_SCREEN_UNLOADED         ),
    JERRY_CONST_ENTRY("LV_EVENT_SIZE_CHANGED",             LV_EVENT_SIZE_CHANGED            ),
    JERRY_CONST_ENTRY("LV_EVENT_STYLE_CHANGED",            LV_EVENT_STYLE_CHANGED           ),
    JERRY_CONST_ENTRY("LV_EVENT_LAYOUT_CHANGED",           LV_EVENT_LAYOUT_CHANGED          ),
    JERRY_CONST_ENTRY("LV_EVENT_GET_SELF_SIZE",            LV_EVENT_GET_SELF_SIZE           ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RAW",                     LV_IMG_CF_RAW                    ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RAW_ALPHA",               LV_IMG_CF_RAW_ALPHA              ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RAW_CHROMA_KEYED",        LV_IMG_CF_RAW_CHROMA_KEYED       ),
    JERRY_CONST_ENTRY("LV_IMG_CF_TRUE_COLOR",              LV_IMG_CF_TRUE_COLOR             ),
    JERRY_CONST_ENTRY("LV_IMG_CF_TRUE_COLOR_ALPHA",        LV_IMG_CF_TRUE_COLOR_ALPHA       ),
    JERRY_CONST_ENTRY("LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED", LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED),
    JERRY_CONST_ENTRY("LV_IMG_CF_INDEXED_1BIT",            LV_IMG_CF_INDEXED_1BIT           ),
    JERRY_CONST_ENTRY("LV_IMG_CF_INDEXED_2BIT",            LV_IMG_CF_INDEXED_2BIT           ),
    JERRY_CONST_ENTRY("LV_IMG_CF_INDEXED_4BIT",            LV_IMG_CF_INDEXED_4BIT           ),
    JERRY_CONST_ENTRY("LV_IMG_CF_INDEXED_8BIT",            LV_IMG_CF_INDEXED_8BIT           ),
    JERRY_CONST_ENTRY("LV_IMG_CF_ALPHA_1BIT",              LV_IMG_CF_ALPHA_1BIT             ),
    JERRY_CONST_ENTRY("LV_IMG_CF_ALPHA_2BIT",              LV_IMG_CF_ALPHA_2BIT             ),
    JERRY_CONST_ENTRY("LV_IMG_CF_ALPHA_4BIT",              LV_IMG_CF_ALPHA_4BIT             ),
    JERRY_CONST_ENTRY("LV_IMG_CF_ALPHA_8BIT",              LV_IMG_CF_ALPHA_8BIT             ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RGB888",                  LV_IMG_CF_RGB888                 ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RGBA8888",                LV_IMG_CF_RGBA8888               ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RGBX8888",                LV_IMG_CF_RGBX8888               ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RGB565",                  LV_IMG_CF_RGB565                 ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RGBA5658",                LV_IMG_CF_RGBA5658               ),
    JERRY_CONST_ENTRY("LV_IMG_CF_RGB565A8",                LV_IMG_CF_RGB565A8               ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_0",          LV_IMG_CF_USER_ENCODED_0         ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_1",          LV_IMG_CF_USER_ENCODED_1         ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_2",          LV_IMG_CF_USER_ENCODED_2         ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_3",          LV_IMG_CF_USER_ENCODED_3         ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_4",          LV_IMG_CF_USER_ENCODED_4         ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_5",          LV_IMG_CF_USER_ENCODED_5         ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_6",          LV_IMG_CF_USER_ENCODED_6         ),
    JERRY_CONST_ENTRY("LV_IMG_CF_USER_ENCODED_7",          LV_IMG_CF_USER_ENCODED_7         ),
    JERRY_CONST_LIST_END(),
};

void jr_gbl_const_init(void)
{
    jerry_value_t global_obj = jerry_current_realm();
    jr_register_global_constant_list(global_obj, jerry_constant_entry_list);
    jerry_value_free(global_obj);
}

