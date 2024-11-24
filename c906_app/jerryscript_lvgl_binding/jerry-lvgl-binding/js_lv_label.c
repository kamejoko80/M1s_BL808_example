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
#include "js_lv_label.h"
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

static jerry_value_t js_lv_obj_align(const jerry_call_info_t *call_info_p,
                                     const jerry_value_t args[],
                                     const jerry_length_t args_count) {
    if (args_count != 4) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid argument count. Expected 4 arguments.");
    }

    if (!jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected an object as the argument");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, args[0], NULL);

    // Get lv_align_t (int value)
    if (!jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Argument 2 must be a number (lv_align_t).");
    }
    lv_align_t align = (lv_align_t)jerry_value_as_number(args[1]);

    // Get x_ofs (int value)
    if (!jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Argument 3 must be a number (x_ofs).");
    }
    lv_coord_t x_ofs = (lv_coord_t)jerry_value_as_number(args[2]);

    // Get y_ofs (int value)
    if (!jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Argument 4 must be a number (y_ofs).");
    }
    lv_coord_t y_ofs = (lv_coord_t)jerry_value_as_number(args[3]);

    // Call the lv_obj_align function
    lv_obj_align(obj, align, x_ofs, y_ofs);

    // Return undefined (void function)
    return jerry_undefined();
}

static jerry_value_t js_lv_label_create(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    // Check argument count
    if (args_count != 1) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected exactly 1 argument");
    }

    // Check if the argument is an object
    if (!jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected an object as the argument");
    }

    // Retrieve the native pointer from the first argument
    JERRY_GET_NATIVE_PTR(lv_obj_t, parent, args[0], NULL);

    // Call the LVGL function
    lv_obj_t *label = lv_label_create(parent);
    if (label == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to create label");
    }

    // Wrap the result in a JerryScript object and set the native pointer
    jerry_value_t js_label = jerry_object();
    jerry_object_set_native_ptr(js_label, NULL, label);

    return js_label;
}

static jerry_value_t js_lv_label_set_text(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    printf("js_lv_label_set_text\n");
    if (args_count != 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_string(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);

    jerry_size_t size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
    char *text = malloc(size + 1);
    if (text == NULL)
    {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Could not allocate text");
    }
    jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)text, size);
    text[size] = '\0';

    lv_label_set_text(label, text);
    free(text);

    return jerry_undefined();
}

static void call_lv_label_set_text_fmt(lv_obj_t *label, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    lv_label_set_text_fmt(label, fmt, args);
    va_end(args);
}

static jerry_value_t js_lv_label_set_text_fmt(const jerry_call_info_t *call_info_p,
                                              const jerry_value_t args[],
                                              const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_string(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and format string.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);

    jerry_size_t fmt_size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
    char *fmt = malloc(fmt_size + 1);
    if (!fmt) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)fmt, fmt_size);
    fmt[fmt_size] = '\0';

    // For simplicity, assume only one additional argument
    if (args_count == 2 || !jerry_value_is_number(args[2])) {
        free(fmt);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected a number for additional arguments.");
    }
    int arg = (int)jerry_value_as_number(args[2]);

    // Call the helper function
    call_lv_label_set_text_fmt(label, fmt, arg);

    // Clean up
    free(fmt);

    return jerry_undefined();
}

static jerry_value_t js_lv_label_set_text_static(const jerry_call_info_t *call_info_p,
                                                 const jerry_value_t args[],
                                                 const jerry_length_t args_count) {
    if (args_count != 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_string(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and static text.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);

    jerry_size_t text_size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    // Call the LVGL function
    lv_label_set_text_static(label, text);

    // Note: `lv_label_set_text_static` does not copy the text; it expects it to persist
    // If you are using a dynamically allocated string, ensure it persists as needed.

    // Free the temporary string buffer (but ensure the original text persists)
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_label_set_long_mode(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count != 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and long_mode (number).");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    lv_label_long_mode_t long_mode = (lv_label_long_mode_t)jerry_value_as_number(args[1]);
    lv_label_set_long_mode(label, long_mode);
    return jerry_undefined();
}

static jerry_value_t js_lv_label_set_recolor(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count != 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_boolean(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and boolean (en).");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    bool enable = jerry_value_is_true(args[1]);
    lv_label_set_recolor(label, enable);
    return jerry_undefined();
}

static jerry_value_t js_lv_label_set_text_sel_start(const jerry_call_info_t *call_info_p,
                                                    const jerry_value_t args[],
                                                    const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and number.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    uint32_t index = (uint32_t)jerry_value_as_number(args[1]);
    lv_label_set_text_sel_start(label, index);
    return jerry_undefined();
}

static jerry_value_t js_lv_label_get_text(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    const char *text = lv_label_get_text(label);
    return jerry_string_sz(text);
}

static jerry_value_t js_lv_label_get_long_mode(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    lv_label_long_mode_t long_mode = lv_label_get_long_mode(label);
    return jerry_number(long_mode);
}

static jerry_value_t js_lv_label_get_recolor(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    bool recolor = lv_label_get_recolor(label);
    return jerry_boolean(recolor);
}

static jerry_value_t js_lv_label_get_letter_pos(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_object(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object, number, and point object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    uint32_t char_id = (uint32_t)jerry_value_as_number(args[1]);
    JERRY_GET_NATIVE_PTR(lv_point_t, pos, args[2], NULL);
    lv_label_get_letter_pos(label, char_id, pos);
    return jerry_undefined();
}

static jerry_value_t js_lv_label_get_letter_on(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_object(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and point object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    JERRY_GET_NATIVE_PTR(lv_point_t, pos_in, args[1], NULL);
    uint32_t char_id = lv_label_get_letter_on(label, pos_in);
    return jerry_number(char_id);
}

static jerry_value_t js_lv_label_is_char_under_pos(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_object(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and point object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    JERRY_GET_NATIVE_PTR(lv_point_t, pos, args[1], NULL);
    bool is_under = lv_label_is_char_under_pos(label, pos);
    return jerry_boolean(is_under);
}

static jerry_value_t js_lv_label_get_text_selection_start(const jerry_call_info_t *call_info_p,
                                                          const jerry_value_t args[],
                                                          const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    uint32_t start_pos = lv_label_get_text_selection_start(label);
    return jerry_number(start_pos);
}

static jerry_value_t js_lv_label_get_text_selection_end(const jerry_call_info_t *call_info_p,
                                                        const jerry_value_t args[],
                                                        const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    uint32_t end_pos = lv_label_get_text_selection_end(label);
    return jerry_number(end_pos);
}

static jerry_value_t js_lv_label_ins_text(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_string(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object, number, and string.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    uint32_t pos = (uint32_t)jerry_value_as_number(args[1]);
    jerry_size_t text_size = jerry_string_size(args[2], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_RANGE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[2], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';
    lv_label_ins_text(label, pos, text);
    free(text);
    return jerry_undefined();
}

static jerry_value_t js_lv_label_cut_text(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object and two numbers.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, args[0], NULL);
    uint32_t pos = (uint32_t)jerry_value_as_number(args[1]);
    uint32_t count = (uint32_t)jerry_value_as_number(args[2]);
    lv_label_cut_text(label, pos, count);
    return jerry_undefined();
}

#if 0
static const jerry_cfunc_entry_t jerry_cfunc_entry_list[] = {
    JERRY_CFUNC_ENTRY("lv_scr_act",                        js_lv_scr_act),
    JERRY_CFUNC_ENTRY("lv_obj_align",                      js_lv_obj_align),
    JERRY_CFUNC_ENTRY("lv_label_create",                   js_lv_label_create),
    JERRY_CFUNC_ENTRY("lv_label_set_text",                 js_lv_label_set_text),
    JERRY_CFUNC_ENTRY("lv_label_set_text_fmt",             js_lv_label_set_text_fmt),
    JERRY_CFUNC_ENTRY("lv_label_set_text_static",          js_lv_label_set_text_static),
    JERRY_CFUNC_ENTRY("lv_label_set_long_mode",            js_lv_label_set_long_mode),
    JERRY_CFUNC_ENTRY("lv_label_set_recolor",              js_lv_label_set_recolor),
    JERRY_CFUNC_ENTRY("lv_label_set_text_sel_start",       js_lv_label_set_text_sel_start),
    JERRY_CFUNC_ENTRY("lv_label_get_text",                 js_lv_label_get_text),
    JERRY_CFUNC_ENTRY("lv_label_get_long_mode",            js_lv_label_get_long_mode),
    JERRY_CFUNC_ENTRY("lv_label_get_recolor",              js_lv_label_get_recolor),
    JERRY_CFUNC_ENTRY("lv_label_get_letter_pos",           js_lv_label_get_letter_pos),
    JERRY_CFUNC_ENTRY("lv_label_get_letter_on",            js_lv_label_get_letter_on),
    JERRY_CFUNC_ENTRY("lv_label_is_char_under_pos",        js_lv_label_is_char_under_pos),
    JERRY_CFUNC_ENTRY("lv_label_get_text_selection_start", js_lv_label_get_text_selection_start),
    JERRY_CFUNC_ENTRY("lv_label_get_text_selection_end",   js_lv_label_get_text_selection_end),
    JERRY_CFUNC_ENTRY("lv_label_ins_text",                 js_lv_label_ins_text),
    JERRY_CFUNC_ENTRY("lv_label_cut_text",                 js_lv_label_cut_text),
};
#else
static const jerry_cfunc_entry_t jerry_cfunc_entry_list[] = {
    JERRY_CFUNC_ENTRY("lv_scr_act",                        js_lv_scr_act),
    JERRY_CFUNC_ENTRY("lv_obj_align",                      js_lv_obj_align),
};
#endif

/*************************************************************************
Implemented following JS class mechanism
*************************************************************************/

static void js_lv_label_destructor_cb(void *native_p, jerry_object_native_info_t *call_info_p) {
    printf("js_lv_label_destructor_cb\n");
    /*
     * Temporally dissable because JS gabage collector
     * automaticlly destroys the JS obj
     */
    //lv_obj_t *label = (lv_obj_t *) native_p;
    //jr_lvgl_obj_desctruct(label);
}

static jerry_object_native_info_t jerry_obj_native_info = {
    .free_cb = js_lv_label_destructor_cb,
};

static jerry_value_t js_lv_label_constructor(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    printf("js_lv_label_constructor\n");
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object .");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, parent, args[0], NULL);
    lv_obj_t *label = lv_label_create(parent);
    if (label == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to create label");
    }

    //lv_obj_align(label, 2, 0, 0);
    lv_obj_set_user_data(label, (void *)call_info_p->this_value);
    jerry_object_set_native_ptr(call_info_p->this_value, /* jerry_value_t object */
                                &jerry_obj_native_info,  /* const jerry_object_native_info_t *native_info_p */
                                label                    /* void *native_pointer_p */
                                );
    return jerry_undefined();
}

static jerry_value_t js_label_set_text(const jerry_call_info_t *call_info_p,
                                       const jerry_value_t args[],
                                       const jerry_length_t args_count) {
    printf("js_lv_label_set_text\n");
    if (args_count < 1 || !jerry_value_is_string(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected (text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, call_info_p->this_value, &jerry_obj_native_info);
    if(label == NULL) {
       return jerry_undefined();
    }

    jerry_size_t size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *text = malloc(size + 1);
    if (text == NULL)
    {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Could not allocate text");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)text, size);
    text[size] = '\0';

    lv_label_set_text(label, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_obj_align(const jerry_call_info_t *call_info_p,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_count) {
    // Ensure at least 4 arguments: `align`, `x_ofs`, `y_ofs`, and `this`
    if (args_count < 3) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Insufficient arguments");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, label, call_info_p->this_value, &jerry_obj_native_info);
    if(label == NULL) {
       return jerry_undefined();
    }

    uint32_t align = (uint32_t)jerry_value_as_number(args[0]);
    lv_coord_t x_ofs = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t y_ofs = (lv_coord_t)jerry_value_as_number(args[2]);

    // Call LVGL function
    lv_obj_align(label, align, x_ofs, y_ofs);

    return jerry_undefined();
}

void js_register_align_constants(jerry_value_t global_obj) {
    jerry_value_t align_center = jerry_number(LV_ALIGN_CENTER);
    jerry_value_t align_name = jerry_string_sz("LV_ALIGN_CENTER");
    jerry_object_set(global_obj, align_name, align_center);
    jerry_value_free(align_center);
    jerry_value_free(align_name);
    // Repeat for other alignment constants
}

void jr_lv_label_init(void) {

    printf("jr_lv_label_init\r\n");

    /* Refer to: 10.EXT-REFERENCE-HANDLER.md  */
    const jerryx_property_entry methods[] =
    {
        JERRYX_PROPERTY_FUNCTION ("setText", js_label_set_text),
        JERRYX_PROPERTY_FUNCTION ("align",   js_obj_align),
        JERRYX_PROPERTY_LIST_END(),
    };

    /* Create the Label constructor */
    jerry_value_t label_constructor = jerry_function_external(js_lv_label_constructor);

    /* Create the prototype object */
    jerry_value_t prop_obj = jerry_object();

    /* Register all properties, methodes */
    jr_set_prop_list(prop_obj, methods);

    /* Link the prototype to the constructor */
    jerry_value_t prototype_property = jerry_string_sz("prototype");
    jerry_object_set(label_constructor, prototype_property, prop_obj);
    jerry_value_free(prototype_property);
    jerry_value_free(prop_obj);

    /* Register the Label constructor in the global object */
    jerry_value_t global_obj = jerry_current_realm();

    /* Register global constants */
    js_register_align_constants(global_obj);

    /* Register instance/constructor */
    jerry_value_t constructor_name = jerry_string_sz("Label");
    jerry_object_set(global_obj, constructor_name, label_constructor);
    jerry_value_free(constructor_name);
    jerry_value_free(label_constructor);
    jerry_value_free(global_obj);

    jr_register_cfunc_list(jerry_cfunc_entry_list, sizeof(jerry_cfunc_entry_list)/sizeof(jerry_cfunc_entry_t));
}
