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
    JERRY_CONST_ENTRY("LV_DIR_NONE",                       LV_DIR_NONE                      ),
    JERRY_CONST_ENTRY("LV_DIR_LEFT",                       LV_DIR_LEFT                      ),
    JERRY_CONST_ENTRY("LV_DIR_RIGHT",                      LV_DIR_RIGHT                     ),
    JERRY_CONST_ENTRY("LV_DIR_TOP",                        LV_DIR_TOP                       ),
    JERRY_CONST_ENTRY("LV_DIR_BOTTOM",                     LV_DIR_BOTTOM                    ),
    JERRY_CONST_ENTRY("LV_DIR_HOR",                        LV_DIR_HOR                       ),
    JERRY_CONST_ENTRY("LV_DIR_VER",                        LV_DIR_VER                       ),
    JERRY_CONST_ENTRY("LV_DIR_ALL",                        LV_DIR_ALL                       ),
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
    JERRY_CONST_ENTRY("LV_OPA_TRANSP",                     LV_OPA_TRANSP                    ),
    JERRY_CONST_ENTRY("LV_OPA_0",                          LV_OPA_0                         ),
    JERRY_CONST_ENTRY("LV_OPA_10",                         LV_OPA_10                        ),
    JERRY_CONST_ENTRY("LV_OPA_20",                         LV_OPA_20                        ),
    JERRY_CONST_ENTRY("LV_OPA_30",                         LV_OPA_30                        ),
    JERRY_CONST_ENTRY("LV_OPA_40",                         LV_OPA_40                        ),
    JERRY_CONST_ENTRY("LV_OPA_50",                         LV_OPA_50                        ),
    JERRY_CONST_ENTRY("LV_OPA_60",                         LV_OPA_60                        ),
    JERRY_CONST_ENTRY("LV_OPA_70",                         LV_OPA_70                        ),
    JERRY_CONST_ENTRY("LV_OPA_80",                         LV_OPA_80                        ),
    JERRY_CONST_ENTRY("LV_OPA_90",                         LV_OPA_90                        ),
    JERRY_CONST_ENTRY("LV_OPA_100",                        LV_OPA_100                       ),
    JERRY_CONST_ENTRY("LV_OPA_COVER",                      LV_OPA_COVER                     ),
    JERRY_CONST_ENTRY("LV_TEXT_FLAG_NONE",                 LV_TEXT_FLAG_NONE                ),
    JERRY_CONST_ENTRY("LV_TEXT_FLAG_RECOLOR",              LV_TEXT_FLAG_RECOLOR             ),
    JERRY_CONST_ENTRY("LV_TEXT_FLAG_EXPAND",               LV_TEXT_FLAG_EXPAND              ),
    JERRY_CONST_ENTRY("LV_TEXT_FLAG_FIT",                  LV_TEXT_FLAG_FIT                 ),
    JERRY_CONST_ENTRY("LV_TEXT_CMD_STATE_WAIT",            LV_TEXT_CMD_STATE_WAIT           ),
    JERRY_CONST_ENTRY("LV_TEXT_CMD_STATE_PAR",             LV_TEXT_CMD_STATE_PAR            ),
    JERRY_CONST_ENTRY("LV_TEXT_CMD_STATE_IN",              LV_TEXT_CMD_STATE_IN             ),
    JERRY_CONST_ENTRY("LV_TEXT_ALIGN_AUTO",                LV_TEXT_ALIGN_AUTO               ),
    JERRY_CONST_ENTRY("LV_TEXT_ALIGN_LEFT",                LV_TEXT_ALIGN_LEFT               ),
    JERRY_CONST_ENTRY("LV_TEXT_ALIGN_CENTER",              LV_TEXT_ALIGN_CENTER             ),
    JERRY_CONST_ENTRY("LV_TEXT_ALIGN_RIGHT",               LV_TEXT_ALIGN_RIGHT              ),
    JERRY_CONST_ENTRY("LV_CHECKBOX_DRAW_PART_BOX",         LV_CHECKBOX_DRAW_PART_BOX        ),
    JERRY_CONST_ENTRY("LV_IMG_SIZE_MODE_VIRTUAL",          LV_IMG_SIZE_MODE_VIRTUAL         ),
    JERRY_CONST_ENTRY("LV_IMG_SIZE_MODE_REAL",             LV_IMG_SIZE_MODE_REAL            ),
    JERRY_CONST_ENTRY("LV_ROLLER_MODE_NORMAL",             LV_ROLLER_MODE_NORMAL            ),
    JERRY_CONST_ENTRY("LV_ROLLER_MODE_INFINITE",           LV_ROLLER_MODE_INFINITE          ),
    JERRY_CONST_ENTRY("LV_SLIDER_MODE_NORMAL",             LV_SLIDER_MODE_NORMAL            ),
    JERRY_CONST_ENTRY("LV_SLIDER_MODE_SYMMETRICAL",        LV_SLIDER_MODE_SYMMETRICAL       ),
    JERRY_CONST_ENTRY("LV_SLIDER_MODE_RANGE",              LV_SLIDER_MODE_RANGE             ),
    JERRY_CONST_ENTRY("LV_SLIDER_DRAW_PART_KNOB",          LV_SLIDER_DRAW_PART_KNOB         ),
    JERRY_CONST_ENTRY("LV_SLIDER_DRAW_PART_KNOB_LEFT",     LV_SLIDER_DRAW_PART_KNOB_LEFT    ),
    JERRY_CONST_ENTRY("LV_TABLE_CELL_CTRL_MERGE_RIGHT",    LV_TABLE_CELL_CTRL_MERGE_RIGHT   ),
    JERRY_CONST_ENTRY("LV_TABLE_CELL_CTRL_TEXT_CROP",      LV_TABLE_CELL_CTRL_TEXT_CROP     ),
    JERRY_CONST_ENTRY("LV_TABLE_CELL_CTRL_CUSTOM_1",       LV_TABLE_CELL_CTRL_CUSTOM_1      ),
    JERRY_CONST_ENTRY("LV_TABLE_CELL_CTRL_CUSTOM_2",       LV_TABLE_CELL_CTRL_CUSTOM_2      ),
    JERRY_CONST_ENTRY("LV_TABLE_CELL_CTRL_CUSTOM_3",       LV_TABLE_CELL_CTRL_CUSTOM_3      ),
    JERRY_CONST_ENTRY("LV_TABLE_CELL_CTRL_CUSTOM_4",       LV_TABLE_CELL_CTRL_CUSTOM_4      ),   
    JERRY_CONST_ENTRY("LV_PART_MAIN",                      LV_PART_MAIN                     ),
    JERRY_CONST_ENTRY("LV_PART_SCROLLBAR",                 LV_PART_SCROLLBAR                ),
    JERRY_CONST_ENTRY("LV_PART_INDICATOR",                 LV_PART_INDICATOR                ),
    JERRY_CONST_ENTRY("LV_PART_KNOB",                      LV_PART_KNOB                     ),
    JERRY_CONST_ENTRY("LV_PART_SELECTED",                  LV_PART_SELECTED                 ),
    JERRY_CONST_ENTRY("LV_PART_ITEMS",                     LV_PART_ITEMS                    ),
    JERRY_CONST_ENTRY("LV_PART_TICKS",                     LV_PART_TICKS                    ),
    JERRY_CONST_ENTRY("LV_PART_CURSOR",                    LV_PART_CURSOR                   ),
    JERRY_CONST_ENTRY("LV_PART_CUSTOM_FIRST",              LV_PART_CUSTOM_FIRST             ),
    JERRY_CONST_ENTRY("LV_PART_ANY",                       LV_PART_ANY                      ),
    JERRY_CONST_LIST_END(),
};

static const jerry_str_const_entry_t jerry_str_constant_entry_list[] = {
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BULLET",              LV_SYMBOL_BULLET                 ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_AUDIO",               LV_SYMBOL_AUDIO                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_VIDEO",               LV_SYMBOL_VIDEO                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_LIST",                LV_SYMBOL_LIST                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_OK",                  LV_SYMBOL_OK                     ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_CLOSE",               LV_SYMBOL_CLOSE                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_POWER",               LV_SYMBOL_POWER                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_SETTINGS",            LV_SYMBOL_SETTINGS               ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_HOME",                LV_SYMBOL_HOME                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_DOWNLOAD",            LV_SYMBOL_DOWNLOAD               ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_DRIVE",               LV_SYMBOL_DRIVE                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_REFRESH",             LV_SYMBOL_REFRESH                ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_MUTE",                LV_SYMBOL_MUTE                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_VOLUME_MID",          LV_SYMBOL_VOLUME_MID             ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_VOLUME_MAX",          LV_SYMBOL_VOLUME_MAX             ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_IMAGE",               LV_SYMBOL_IMAGE                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_TINT",                LV_SYMBOL_TINT                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_PREV",                LV_SYMBOL_PREV                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_PLAY",                LV_SYMBOL_PLAY                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_PAUSE",               LV_SYMBOL_PAUSE                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_STOP",                LV_SYMBOL_STOP                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_NEXT",                LV_SYMBOL_NEXT                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_EJECT",               LV_SYMBOL_EJECT                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_LEFT",                LV_SYMBOL_LEFT                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_RIGHT",               LV_SYMBOL_RIGHT                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_PLUS",                LV_SYMBOL_PLUS                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_MINUS",               LV_SYMBOL_MINUS                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_EYE_OPEN",            LV_SYMBOL_EYE_OPEN               ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_EYE_CLOSE",           LV_SYMBOL_EYE_CLOSE              ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_WARNING",             LV_SYMBOL_WARNING                ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_SHUFFLE",             LV_SYMBOL_SHUFFLE                ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_UP",                  LV_SYMBOL_UP                     ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_DOWN",                LV_SYMBOL_DOWN                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_LOOP",                LV_SYMBOL_LOOP                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_DIRECTORY",           LV_SYMBOL_DIRECTORY              ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_UPLOAD",              LV_SYMBOL_UPLOAD                 ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_CALL",                LV_SYMBOL_CALL                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_CUT",                 LV_SYMBOL_CUT                    ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_COPY",                LV_SYMBOL_COPY                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_SAVE",                LV_SYMBOL_SAVE                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BARS",                LV_SYMBOL_BARS                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_ENVELOPE",            LV_SYMBOL_ENVELOPE               ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_CHARGE",              LV_SYMBOL_CHARGE                 ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_PASTE",               LV_SYMBOL_PASTE                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BELL",                LV_SYMBOL_BELL                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_KEYBOARD",            LV_SYMBOL_KEYBOARD               ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_GPS",                 LV_SYMBOL_GPS                    ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_FILE",                LV_SYMBOL_FILE                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_WIFI",                LV_SYMBOL_WIFI                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BATTERY_FULL",        LV_SYMBOL_BATTERY_FULL           ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BATTERY_3",           LV_SYMBOL_BATTERY_3              ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BATTERY_2",           LV_SYMBOL_BATTERY_2              ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BATTERY_1",           LV_SYMBOL_BATTERY_1              ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BATTERY_EMPTY",       LV_SYMBOL_BATTERY_EMPTY          ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_USB",                 LV_SYMBOL_USB                    ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BLUETOOTH",           LV_SYMBOL_BLUETOOTH              ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_TRASH",               LV_SYMBOL_TRASH                  ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_EDIT",                LV_SYMBOL_EDIT                   ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_BACKSPACE",           LV_SYMBOL_BACKSPACE              ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_SD_CARD",             LV_SYMBOL_SD_CARD                ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_NEW_LINE",            LV_SYMBOL_NEW_LINE               ),
    JERRY_STR_CONST_ENTRY("LV_SYMBOL_DUMMY",               LV_SYMBOL_DUMMY                  ),
    JERRY_STR_CONST_LIST_END(),
};

void jr_gbl_const_init(void)
{
    jerry_value_t global_obj = jerry_current_realm();
    jr_register_global_constant_list(global_obj, jerry_constant_entry_list);
    jr_register_global_str_constant_list(global_obj, jerry_str_constant_entry_list);
    jerry_value_free(global_obj);
}

