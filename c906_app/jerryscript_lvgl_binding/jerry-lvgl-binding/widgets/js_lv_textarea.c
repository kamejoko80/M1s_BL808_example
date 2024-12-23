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
#define LV_OBJ_NAME           "TextArea"
#define LV_OBJ_CREATE(parent) lv_textarea_create(parent)

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

static jerry_value_t js_lv_textarea_add_char(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (char)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    uint32_t c = (uint32_t)jerry_value_as_number(args[0]);
    lv_textarea_add_char(obj, c);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_add_text(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    jerry_size_t text_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    lv_textarea_add_text(obj, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_del_char(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_textarea_del_char(obj);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_del_char_forward(const jerry_call_info_t *call_info_p,
                                                     const jerry_value_t args[],
                                                     const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_textarea_del_char_forward(obj);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_text(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    jerry_size_t text_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    lv_textarea_set_text(obj, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_placeholder_text(const jerry_call_info_t *call_info_p,
                                                         const jerry_value_t args[],
                                                         const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (placeholder text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    jerry_size_t text_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    lv_textarea_set_placeholder_text(obj, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_cursor_pos(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (cursor position)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    int32_t pos = (int32_t)jerry_value_as_number(args[0]);
    lv_textarea_set_cursor_pos(obj, pos);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_cursor_click_pos(const jerry_call_info_t *call_info_p,
                                                         const jerry_value_t args[],
                                                         const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_boolean(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (enable)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool enable = jerry_value_is_true(args[0]);
    lv_textarea_set_cursor_click_pos(obj, enable);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_password_mode(const jerry_call_info_t *call_info_p,
                                                      const jerry_value_t args[],
                                                      const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_boolean(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (enable)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool enable = jerry_value_is_true(args[0]);
    lv_textarea_set_password_mode(obj, enable);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_password_bullet(const jerry_call_info_t *call_info_p,
                                                        const jerry_value_t args[],
                                                        const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (bullet character)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    jerry_size_t bullet_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *bullet = malloc(bullet_size + 1);
    if (!bullet) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)bullet, bullet_size);
    bullet[bullet_size] = '\0';

    lv_textarea_set_password_bullet(obj, bullet);
    free(bullet);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_one_line(const jerry_call_info_t *call_info_p,
                                                 const jerry_value_t args[],
                                                 const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_boolean(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (boolean)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool en = jerry_value_is_true(args[0]);
    lv_textarea_set_one_line(obj, en);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_accepted_chars(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    jerry_size_t text_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    lv_textarea_set_accepted_chars(obj, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_max_length(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (number)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    uint32_t max_length = (uint32_t)jerry_value_as_number(args[0]);
    lv_textarea_set_max_length(obj, max_length);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_insert_replace(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    jerry_size_t text_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    lv_textarea_set_insert_replace(obj, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_text_selection(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_boolean(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (enabled)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool en = jerry_value_is_true(args[0]);
    lv_textarea_set_text_selection(obj, en);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_password_show_time(const jerry_call_info_t *call_info_p,
                                                           const jerry_value_t args[],
                                                           const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (time)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    uint16_t time = (uint16_t)jerry_value_as_number(args[0]);
    lv_textarea_set_password_show_time(obj, time);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_set_align(const jerry_call_info_t *call_info_p,
                                              const jerry_value_t args[],
                                              const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (align)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_text_align_t align = (lv_text_align_t)jerry_value_as_number(args[0]);
    lv_textarea_set_align(obj, align);

    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_get_text(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    const char *text = lv_textarea_get_text(obj);
    if (!text) {
        return jerry_undefined();
    }
    return jerry_string_sz(text);
}

static jerry_value_t js_lv_textarea_get_placeholder_text(const jerry_call_info_t *call_info_p,
                                                         const jerry_value_t args[],
                                                         const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    const char *placeholder = lv_textarea_get_placeholder_text(obj);
    if (!placeholder) {
        return jerry_undefined();
    }
    return jerry_string_sz(placeholder);
}

static jerry_value_t js_lv_textarea_get_label(const jerry_call_info_t *call_info_p,
                                              const jerry_value_t args[],
                                              const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_obj_t *label = lv_textarea_get_label(obj);
    if (label == NULL) {
        return jerry_undefined();
    }

    jerry_value_t js_label = jerry_object();
    jerry_object_set_native_ptr(js_label, &jerry_obj_native_info, label);
    return js_label;
}

static jerry_value_t js_lv_textarea_get_cursor_pos(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    uint32_t pos = lv_textarea_get_cursor_pos(obj);
    return jerry_number(pos);
}

static jerry_value_t js_lv_textarea_get_cursor_click_pos(const jerry_call_info_t *call_info_p,
                                                         const jerry_value_t args[],
                                                         const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool en = lv_textarea_get_cursor_click_pos(obj);
    return jerry_boolean(en);
}

static jerry_value_t js_lv_textarea_get_password_mode(const jerry_call_info_t *call_info_p,
                                                      const jerry_value_t args[],
                                                      const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool en = lv_textarea_get_password_mode(obj);
    return jerry_boolean(en);
}

static jerry_value_t js_lv_textarea_get_password_bullet(const jerry_call_info_t *call_info_p,
                                                        const jerry_value_t args[],
                                                        const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    const char *bullet = lv_textarea_get_password_bullet(obj);
    if (bullet == NULL) {
        return jerry_undefined();
    }

    return jerry_string_sz(bullet);
}

static jerry_value_t js_lv_textarea_get_one_line(const jerry_call_info_t *call_info_p,
                                                 const jerry_value_t args[],
                                                 const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool one_line = lv_textarea_get_one_line(obj);
    return jerry_boolean(one_line);
}

static jerry_value_t js_lv_textarea_get_accepted_chars(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    const char *accepted_chars = lv_textarea_get_accepted_chars(obj);
    if (accepted_chars == NULL) {
        return jerry_undefined();
    }

    return jerry_string_sz(accepted_chars);
}

static jerry_value_t js_lv_textarea_get_max_length(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    uint32_t max_length = lv_textarea_get_max_length(obj);
    return jerry_number(max_length);
}

static jerry_value_t js_lv_textarea_text_is_selected(const jerry_call_info_t *call_info_p,
                                                     const jerry_value_t args[],
                                                     const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool is_selected = lv_textarea_text_is_selected(obj);
    return jerry_boolean(is_selected);
}

static jerry_value_t js_lv_textarea_get_text_selection(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    bool text_selection = lv_textarea_get_text_selection(obj);
    return jerry_boolean(text_selection);
}

static jerry_value_t js_lv_textarea_get_password_show_time(const jerry_call_info_t *call_info_p,
                                                           const jerry_value_t args[],
                                                           const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    uint16_t show_time = lv_textarea_get_password_show_time(obj);
    return jerry_number(show_time);
}

static jerry_value_t js_lv_textarea_clear_selection(const jerry_call_info_t *call_info_p,
                                                    const jerry_value_t args[],
                                                    const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_textarea_clear_selection(obj);
    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_cursor_right(const jerry_call_info_t *call_info_p,
                                                 const jerry_value_t args[],
                                                 const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_textarea_cursor_right(obj);
    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_cursor_left(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_textarea_cursor_left(obj);
    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_cursor_down(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_textarea_cursor_down(obj);
    return jerry_undefined();
}

static jerry_value_t js_lv_textarea_cursor_up(const jerry_call_info_t *call_info_p,
                                              const jerry_value_t args[],
                                              const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid TextArea object");
    }

    lv_textarea_cursor_up(obj);
    return jerry_undefined();
}

/************************************************************************
* Protperties, methodes entry list
*************************************************************************/

static const jerry_cfunc_entry_t methods[] = {
    JERRY_CFUNC_ENTRY("align",               js_lv_obj_align),
    JERRY_CFUNC_ENTRY("setSize",             js_lv_obj_set_size),
    JERRY_CFUNC_ENTRY("setStyle",            js_lv_obj_set_style),
    JERRY_CFUNC_ENTRY("onChanged",           js_lv_obj_on_changed),
    JERRY_CFUNC_ENTRY("addChar",             js_lv_textarea_add_char),
    JERRY_CFUNC_ENTRY("addText",             js_lv_textarea_add_text),
    JERRY_CFUNC_ENTRY("delChar",             js_lv_textarea_del_char),
    JERRY_CFUNC_ENTRY("delCharForward",      js_lv_textarea_del_char_forward),
    JERRY_CFUNC_ENTRY("setText",             js_lv_textarea_set_text),
    JERRY_CFUNC_ENTRY("setPlaceHolderText",  js_lv_textarea_set_placeholder_text),
    JERRY_CFUNC_ENTRY("setCursorPos",        js_lv_textarea_set_cursor_pos),
    JERRY_CFUNC_ENTRY("setCursorClickPos",   js_lv_textarea_set_cursor_click_pos),
    JERRY_CFUNC_ENTRY("setPasswordMode",     js_lv_textarea_set_password_mode),
    JERRY_CFUNC_ENTRY("setPasswordBullet",   js_lv_textarea_set_password_bullet),
    JERRY_CFUNC_ENTRY("setOneLine",          js_lv_textarea_set_one_line),
    JERRY_CFUNC_ENTRY("setAcceptedChars",    js_lv_textarea_set_accepted_chars),
    JERRY_CFUNC_ENTRY("setMaxLength",        js_lv_textarea_set_max_length),
    JERRY_CFUNC_ENTRY("setInsertReplace",    js_lv_textarea_set_insert_replace),
    JERRY_CFUNC_ENTRY("setTextSelection",    js_lv_textarea_set_text_selection),
    JERRY_CFUNC_ENTRY("setPasswordShowTime", js_lv_textarea_set_password_show_time),
    JERRY_CFUNC_ENTRY("setAlign",            js_lv_textarea_set_align),
    JERRY_CFUNC_ENTRY("getText",             js_lv_textarea_get_text),
    JERRY_CFUNC_ENTRY("getPlaceHolderText",  js_lv_textarea_get_placeholder_text),
    JERRY_CFUNC_ENTRY("getLabel",            js_lv_textarea_get_label),
    JERRY_CFUNC_ENTRY("getCursorPos",        js_lv_textarea_get_cursor_pos),
    JERRY_CFUNC_ENTRY("getCursorClickPos",   js_lv_textarea_get_cursor_click_pos),
    JERRY_CFUNC_ENTRY("getPasswordMode",     js_lv_textarea_get_password_mode),
    JERRY_CFUNC_ENTRY("getPasswordBullet",   js_lv_textarea_get_password_bullet),
    JERRY_CFUNC_ENTRY("getOneLine",          js_lv_textarea_get_one_line),
    JERRY_CFUNC_ENTRY("getAcceptedChars",    js_lv_textarea_get_accepted_chars),
    JERRY_CFUNC_ENTRY("getMaxLength",        js_lv_textarea_get_max_length),
    JERRY_CFUNC_ENTRY("isSelected",          js_lv_textarea_text_is_selected),
    JERRY_CFUNC_ENTRY("getTextSelection",    js_lv_textarea_get_text_selection),
    JERRY_CFUNC_ENTRY("getPasswordShowTime", js_lv_textarea_get_password_show_time),
    JERRY_CFUNC_ENTRY("clearSelection",      js_lv_textarea_clear_selection),
    JERRY_CFUNC_ENTRY("cursorRight",         js_lv_textarea_cursor_right),
    JERRY_CFUNC_ENTRY("cursorLeft",          js_lv_textarea_cursor_left),
    JERRY_CFUNC_ENTRY("cursorDown",          js_lv_textarea_cursor_down),
    JERRY_CFUNC_ENTRY("cursorUp",            js_lv_textarea_cursor_up),
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

void jr_lv_textarea_init(void) {
    jerry_value_t global_obj = jerry_current_realm();
    jerry_value_t constructor = jerry_function_external(js_lv_obj_constructor);
    jerry_value_t constructor_name = jerry_string_sz(LV_OBJ_NAME);
    jerry_object_set(global_obj, constructor_name, constructor);
    jerry_value_free(constructor_name);
    jerry_value_free(constructor);
    jerry_value_free(global_obj);
}
