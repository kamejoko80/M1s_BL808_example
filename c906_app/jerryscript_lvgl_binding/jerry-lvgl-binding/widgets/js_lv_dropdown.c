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
#include "js_lv_btn.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

/* Change the object appropriately */
#define LV_OBJ_NAME           "DropDown"
#define LV_OBJ_CREATE(parent) lv_dropdown_create(parent)

/************************************************************************
* Typedef
*************************************************************************/

typedef struct {
    char *text_static;
    char *options_static;
    void *symbol;
} jerry_lv_user_data_t;

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
        if (user_data->text_static != NULL) {
            free(user_data->text_static);
        }
        if (user_data->options_static != NULL) {
            free(user_data->options_static);
        }
        if (user_data->symbol != NULL) {
            free(user_data->symbol);
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

static jerry_object_native_info_t jerry_obj_native_info = {
    .free_cb = js_lv_obj_destructor_cb,
};

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
    //    user_data->value = 1;
    //    user_data->name = strdup("Some text");
    // }

    jerry_object_set_native_ptr(call_info_p->this_value, /* jerry_value_t object */
                                &jerry_obj_native_info,  /* const jerry_object_native_info_t *native_info_p */
                                obj                      /* void *native_pointer_p */
                                );
    return jerry_undefined();
}

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

    // Register the event callback
    lv_obj_add_event_cb(obj, js_lv_obj_event_cb, LV_EVENT_VALUE_CHANGED, (void *)(uintptr_t)args[0]);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_set_text(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    jerry_size_t text_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    if(user_data != NULL){
        user_data->text_static = text;
    }

    lv_dropdown_set_text(obj, text);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_set_options(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (options)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    jerry_size_t options_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *options = malloc(options_size + 1);
    if (!options) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)options, options_size);
    options[options_size] = '\0';

    lv_dropdown_set_options(obj, options);

    free(options);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_set_options_static(const jerry_call_info_t *call_info_p,
                                                       const jerry_value_t args[],
                                                       const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (options)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    jerry_size_t options_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *options = malloc(options_size + 1);
    if (!options) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)options, options_size);
    options[options_size] = '\0';

    jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    if(user_data != NULL){
        user_data->options_static = options;
    }

    lv_dropdown_set_options_static(obj, options);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_add_option(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_string(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (option, pos)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    jerry_size_t option_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *option = malloc(option_size + 1);
    if (!option) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)option, option_size);
    option[option_size] = '\0';

    uint32_t pos = (uint32_t)jerry_value_as_number(args[1]);

    lv_dropdown_add_option(obj, option, pos);

    free(option);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_clear_options(const jerry_call_info_t *call_info_p,
                                                  const jerry_value_t args[],
                                                  const jerry_length_t args_count) {
    if (args_count != 0) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected no arguments");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    lv_dropdown_clear_options(obj);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_set_selected(const jerry_call_info_t *call_info_p,
                                                 const jerry_value_t args[],
                                                 const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (sel_opt)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    uint16_t sel_opt = (uint16_t)jerry_value_as_number(args[0]);
    lv_dropdown_set_selected(obj, sel_opt);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_set_dir(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (dir)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    lv_dir_t dir = (lv_dir_t)jerry_value_as_number(args[0]);
    lv_dropdown_set_dir(obj, dir);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_set_symbol(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_arraybuffer(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (symbol as ArrayBuffer)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    jerry_length_t symbol_size = jerry_arraybuffer_size(args[0]);
    if (symbol_size == 0) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Symbol size is zero");
    }

    void *symbol_buffer = malloc(symbol_size);
    if (!symbol_buffer) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    jerry_arraybuffer_read(args[0], 0, (uint8_t *)symbol_buffer, symbol_size);

    jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    if(user_data != NULL){
        user_data->symbol = symbol_buffer;
    }

    lv_dropdown_set_symbol(obj, symbol_buffer);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_set_selected_highlight(const jerry_call_info_t *call_info_p,
                                                           const jerry_value_t args[],
                                                           const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_boolean(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (en)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    bool en = jerry_value_is_true(args[0]);
    lv_dropdown_set_selected_highlight(obj, en);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_get_list(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    lv_obj_t *list = lv_dropdown_get_list(obj);
    if (list == NULL) {
        return jerry_undefined();
    }

    jerry_value_t js_list = jerry_object();
    jerry_object_set_native_ptr(js_list, &jerry_obj_native_info, list);
    return js_list;
}

static jerry_value_t js_lv_dropdown_get_text(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    const char *text = lv_dropdown_get_text(obj);
    if (text == NULL) {
        return jerry_undefined();
    }

    return jerry_string_sz(text);
}

static jerry_value_t js_lv_dropdown_get_options(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    const char *options = lv_dropdown_get_options(obj);
    if (options == NULL) {
        return jerry_undefined();
    }

    return jerry_string_sz(options);
}

static jerry_value_t js_lv_dropdown_get_selected(const jerry_call_info_t *call_info_p,
                                                 const jerry_value_t args[],
                                                 const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    uint16_t selected = lv_dropdown_get_selected(obj);
    return jerry_number(selected);
}

static jerry_value_t js_lv_dropdown_get_option_cnt(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    uint16_t option_cnt = lv_dropdown_get_option_cnt(obj);
    return jerry_number(option_cnt);
}

static jerry_value_t js_lv_dropdown_get_selected_str(const jerry_call_info_t *call_info_p,
                                                     const jerry_value_t args[],
                                                     const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (buf_size)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    uint32_t buf_size = (uint32_t)jerry_value_as_number(args[0]);
    char *buf = malloc(buf_size);
    if (!buf) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    lv_dropdown_get_selected_str(obj, buf, buf_size);

    jerry_value_t js_buf = jerry_string_sz(buf);
    free(buf);

    return js_buf;
}

static jerry_value_t js_lv_dropdown_get_option_index(const jerry_call_info_t *call_info_p,
                                                     const jerry_value_t args[],
                                                     const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (option)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    jerry_size_t option_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *option = malloc(option_size + 1);
    if (!option) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)option, option_size);
    option[option_size] = '\0';

    int32_t index = lv_dropdown_get_option_index(obj, option);
    printf("index = %d\n", index);
    free(option);

    return jerry_number(index);
}

static jerry_value_t js_lv_dropdown_get_symbol(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    const char *symbol = lv_dropdown_get_symbol(obj);
    if (!symbol) {
        return jerry_undefined();
    }

    return jerry_string_sz(symbol);
}

static jerry_value_t js_lv_dropdown_get_selected_highlight(const jerry_call_info_t *call_info_p,
                                                           const jerry_value_t args[],
                                                           const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    bool highlight = lv_dropdown_get_selected_highlight(obj);
    return jerry_boolean(highlight);
}

static jerry_value_t js_lv_dropdown_get_dir(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    lv_dir_t dir = lv_dropdown_get_dir(obj);
    return jerry_number((int32_t)dir);
}

static jerry_value_t js_lv_dropdown_open(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    lv_dropdown_open(obj);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_close(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    lv_dropdown_close(obj);

    return jerry_undefined();
}

static jerry_value_t js_lv_dropdown_is_open(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid dropdown object");
    }

    bool is_open = lv_dropdown_is_open(obj);
    return jerry_boolean(is_open);
}


/************************************************************************
* Class register functions
*************************************************************************/

static void jr_lv_obj_class_register(jerry_external_handler_t constructor_handler) {

    jerryx_property_entry methods[] =
    {
        JERRYX_PROPERTY_FUNCTION ("align",                js_lv_obj_align),
        JERRYX_PROPERTY_FUNCTION ("setSize",              js_lv_obj_set_size),
        JERRYX_PROPERTY_FUNCTION ("onChanged",            js_lv_obj_on_changed),
        JERRYX_PROPERTY_FUNCTION ("setText",              js_lv_dropdown_set_text),
        JERRYX_PROPERTY_FUNCTION ("setOptions",           js_lv_dropdown_set_options),
        JERRYX_PROPERTY_FUNCTION ("setOptionsStatic",     js_lv_dropdown_set_options_static),
        JERRYX_PROPERTY_FUNCTION ("addOption",            js_lv_dropdown_add_option),
        JERRYX_PROPERTY_FUNCTION ("clearOptions",         js_lv_dropdown_clear_options),
        JERRYX_PROPERTY_FUNCTION ("setSelected",          js_lv_dropdown_set_selected),
        JERRYX_PROPERTY_FUNCTION ("setDir",               js_lv_dropdown_set_dir),
        JERRYX_PROPERTY_FUNCTION ("setSymbol",            js_lv_dropdown_set_symbol),
        JERRYX_PROPERTY_FUNCTION ("setSelectedHighLight", js_lv_dropdown_set_selected_highlight),
        JERRYX_PROPERTY_FUNCTION ("getList",              js_lv_dropdown_get_list),
        JERRYX_PROPERTY_FUNCTION ("getText",              js_lv_dropdown_get_text),
        JERRYX_PROPERTY_FUNCTION ("getOptions",           js_lv_dropdown_get_options),
        JERRYX_PROPERTY_FUNCTION ("getSelected",          js_lv_dropdown_get_selected),
        JERRYX_PROPERTY_FUNCTION ("getOptionCnt",         js_lv_dropdown_get_option_cnt),
        JERRYX_PROPERTY_FUNCTION ("getSelectedStr",       js_lv_dropdown_get_selected_str),
        JERRYX_PROPERTY_FUNCTION ("getOptionIndex",       js_lv_dropdown_get_option_index),
        JERRYX_PROPERTY_FUNCTION ("getSymbol",            js_lv_dropdown_get_symbol),
        JERRYX_PROPERTY_FUNCTION ("getSelectedHighLight", js_lv_dropdown_get_selected_highlight),
        JERRYX_PROPERTY_FUNCTION ("getDir",               js_lv_dropdown_get_dir),
        JERRYX_PROPERTY_FUNCTION ("open",                 js_lv_dropdown_open),
        JERRYX_PROPERTY_FUNCTION ("close",                js_lv_dropdown_close),
        JERRYX_PROPERTY_FUNCTION ("isOpen",               js_lv_dropdown_is_open),
        JERRYX_PROPERTY_LIST_END(),
    };

    jerry_value_t constructor = jerry_function_external(constructor_handler);
    jerry_value_t prop_obj = jerry_object();
    jr_set_prop_list(prop_obj, methods);
    jerry_value_t prototype_property = jerry_string_sz("prototype");
    jerry_object_set(constructor, prototype_property, prop_obj);
    jerry_value_free(prototype_property);
    jerry_value_free(prop_obj);

    jerry_value_t global_obj = jerry_current_realm();
    jerry_value_t constructor_name = jerry_string_sz(LV_OBJ_NAME);
    jerry_object_set(global_obj, constructor_name, constructor);
    jerry_value_free(constructor_name);
    jerry_value_free(constructor);
    jerry_value_free(global_obj);
}

void jr_lv_dropdown_init(void) {
    jr_lv_obj_class_register(js_lv_obj_constructor);
}
