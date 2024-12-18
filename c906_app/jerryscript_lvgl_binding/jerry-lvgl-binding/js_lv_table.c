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
#define LV_OBJ_NAME           "Table"
#define LV_OBJ_CREATE(parent) lv_table_create(parent)

/************************************************************************
* Typedef
*************************************************************************/

typedef struct {
    uint32_t value;
    char     *name;
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

static jerry_value_t js_lv_obj_on_clicked(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_function(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected a callback function");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if(obj == NULL) {
       return jerry_undefined();
    }

    lv_obj_add_event_cb(obj, js_lv_obj_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)args[0]);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_set_cell_value(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_string(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (row, col, text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t col = (uint16_t)jerry_value_as_number(args[1]);

    jerry_size_t text_size = jerry_string_size(args[2], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[2], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    lv_table_set_cell_value(obj, row, col, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_set_cell_value_fmt(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_string(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (row, col, formatted_text)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t col = (uint16_t)jerry_value_as_number(args[1]);

    jerry_size_t text_size = jerry_string_size(args[2], JERRY_ENCODING_UTF8);
    char *text = malloc(text_size + 1);
    if (!text) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[2], JERRY_ENCODING_UTF8, (jerry_char_t *)text, text_size);
    text[text_size] = '\0';

    lv_table_set_cell_value(obj, row, col, text);
    free(text);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_set_row_cnt(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (row_cnt)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row_cnt = (uint16_t)jerry_value_as_number(args[0]);
    lv_table_set_row_cnt(obj, row_cnt);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_set_col_cnt(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (col_cnt)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t col_cnt = (uint16_t)jerry_value_as_number(args[0]);
    lv_table_set_col_cnt(obj, col_cnt);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_set_col_width(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (col_id, width)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t col_id = (uint16_t)jerry_value_as_number(args[0]);
    lv_coord_t width = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_table_set_col_width(obj, col_id, width);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_add_cell_ctrl(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (row, col, ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t col = (uint16_t)jerry_value_as_number(args[1]);
    lv_table_cell_ctrl_t ctrl = (lv_table_cell_ctrl_t)jerry_value_as_number(args[2]);

    lv_table_add_cell_ctrl(obj, row, col, ctrl);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_clear_cell_ctrl(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (row, col, ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t col = (uint16_t)jerry_value_as_number(args[1]);
    lv_table_cell_ctrl_t ctrl = (lv_table_cell_ctrl_t)jerry_value_as_number(args[2]);

    lv_table_clear_cell_ctrl(obj, row, col, ctrl);

    return jerry_undefined();
}

static jerry_value_t js_lv_table_get_cell_value(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (row, col)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t col = (uint16_t)jerry_value_as_number(args[1]);

    const char *cell_value = lv_table_get_cell_value(obj, row, col);
    if (!cell_value) {
        return jerry_undefined();
    }

    return jerry_string_sz(cell_value);
}

static jerry_value_t js_lv_table_get_row_cnt(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row_cnt = lv_table_get_row_cnt(obj);
    return jerry_number(row_cnt);
}

static jerry_value_t js_lv_table_get_col_cnt(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t col_cnt = lv_table_get_col_cnt(obj);
    return jerry_number(col_cnt);
}

static jerry_value_t js_lv_table_get_col_width(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (col_id)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t col_id = (uint16_t)jerry_value_as_number(args[0]);
    lv_coord_t width = lv_table_get_col_width(obj, col_id);

    return jerry_number(width);
}

static jerry_value_t js_lv_table_has_cell_ctrl(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (row, col, ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t col = (uint16_t)jerry_value_as_number(args[1]);
    lv_table_cell_ctrl_t ctrl = (lv_table_cell_ctrl_t)jerry_value_as_number(args[2]);

    bool has_ctrl = lv_table_has_cell_ctrl(obj, row, col, ctrl);
    return jerry_boolean(has_ctrl);
}

static jerry_value_t js_lv_table_get_selected_cell(const jerry_call_info_t *call_info_p, const jerry_value_t args[], const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid Table object");
    }

    uint16_t row, col;
    lv_table_get_selected_cell(obj, &row, &col);

    jerry_value_t result = jerry_object();
    jerry_object_set(result, jerry_string_sz("row"), jerry_number(row));
    jerry_object_set(result, jerry_string_sz("col"), jerry_number(col));

    return result;
}

/************************************************************************
* Class register functions
*************************************************************************/

static void jr_lv_obj_class_register(jerry_external_handler_t constructor_handler) {

    jerryx_property_entry methods[] =
    {
        JERRYX_PROPERTY_FUNCTION ("align",           js_lv_obj_align),
        JERRYX_PROPERTY_FUNCTION ("setSize",         js_lv_obj_set_size),
        JERRYX_PROPERTY_FUNCTION ("onClicked",       js_lv_obj_on_clicked),
        JERRYX_PROPERTY_FUNCTION ("setCellValue",    js_lv_table_set_cell_value),
        JERRYX_PROPERTY_FUNCTION ("setCellValueFmt", js_lv_table_set_cell_value_fmt),
        JERRYX_PROPERTY_FUNCTION ("setRowCnt",       js_lv_table_set_row_cnt),
        JERRYX_PROPERTY_FUNCTION ("setColCnt",       js_lv_table_set_col_cnt),
        JERRYX_PROPERTY_FUNCTION ("setColWidth",     js_lv_table_set_col_width),
        JERRYX_PROPERTY_FUNCTION ("addCellCtrl",     js_lv_table_add_cell_ctrl),
        JERRYX_PROPERTY_FUNCTION ("clearCellCtrl",   js_lv_table_clear_cell_ctrl),
        JERRYX_PROPERTY_FUNCTION ("getCellValue",    js_lv_table_get_cell_value),
        JERRYX_PROPERTY_FUNCTION ("getRowCnt",       js_lv_table_get_row_cnt),
        JERRYX_PROPERTY_FUNCTION ("getColCnt",       js_lv_table_get_col_cnt),
        JERRYX_PROPERTY_FUNCTION ("getColWidth",     js_lv_table_get_col_width),
        JERRYX_PROPERTY_FUNCTION ("hasCellCtrl",     js_lv_table_has_cell_ctrl),
        JERRYX_PROPERTY_FUNCTION ("getSelectedCell", js_lv_table_get_selected_cell),
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

void jr_lv_table_init(void) {
    jr_lv_obj_class_register(js_lv_obj_constructor);
}
