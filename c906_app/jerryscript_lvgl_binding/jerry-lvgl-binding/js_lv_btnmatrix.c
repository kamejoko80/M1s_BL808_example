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
#define LV_OBJ_NAME           "BtnMatrix"
#define LV_OBJ_CREATE(parent) lv_btnmatrix_create(parent)

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

static void js_lv_obj_destructor_cb(void *native_p, jerry_object_native_info_t *call_info_p) {
    printf("%s %s\n", LV_OBJ_NAME, __FUNCTION__);
    lv_obj_t *btn = (lv_obj_t *) native_p;
    jr_lvgl_obj_desctruct(btn);
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
    //jerry_user_data_t *user_data = malloc(sizeof(jerry_user_data_t));
    //user_data->value1 = 1;
    //user_data->value1 = 2;
    //user_data->name = strdup("Some text");
    //lv_obj_set_user_data(obj, user_data);

    jerry_object_set_native_ptr(call_info_p->this_value, /* jerry_value_t object */
                                &jerry_obj_native_info,  /* const jerry_object_native_info_t *native_info_p */
                                obj                      /* void *native_pointer_p */
                                );
    return jerry_undefined();
}

/************************************************************************
* Protperties, methodes definition
*************************************************************************/

static jerry_value_t js_obj_align(const jerry_call_info_t *call_info_p,
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

#if 0

static jerry_value_t js_lv_obj_set_map(const jerry_call_info_t *call_info_p,
                                       const jerry_value_t args[],
                                       const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_array(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (map)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint32_t len = jerry_get_array_length(args[1]);
    const char *map[len + 1];
    for (uint32_t i = 0; i < len; i++) {
        jerry_value_t item = jerry_get_property_by_index(args[1], i);
        if (!jerry_value_is_string(item)) {
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected strings in map");
        }
        size_t size = jerry_get_string_size(item);
        map[i] = malloc(size + 1);
        jerry_string_to_char_buffer(item, (jerry_char_t *)map[i], size);
        map[i][size] = '\0';
        jerry_release_value(item);
    }
    map[len] = NULL;

    lv_btnmatrix_set_map(obj, map);

    for (uint32_t i = 0; i < len; i++) {
        free((void *)map[i]);
    }

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_ctrl_map(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_array(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (ctrl_map)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint32_t len = jerry_get_array_length(args[1]);
    lv_btnmatrix_ctrl_t ctrl_map[len];
    for (uint32_t i = 0; i < len; i++) {
        jerry_value_t item = jerry_get_property_by_index(args[1], i);
        if (!jerry_value_is_number(item)) {
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected numbers in ctrl_map");
        }
        ctrl_map[i] = (lv_btnmatrix_ctrl_t)jerry_value_as_number(item);
        jerry_release_value(item);
    }

    lv_btnmatrix_set_ctrl_map(obj, ctrl_map);
    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_selected_btn(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (btn_id)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t btn_id = (uint16_t)jerry_value_as_number(args[1]);
    lv_btnmatrix_set_selected_btn(obj, btn_id);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_btn_ctrl(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (btn_id, ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t btn_id = (uint16_t)jerry_value_as_number(args[1]);
    lv_btnmatrix_ctrl_t ctrl = (lv_btnmatrix_ctrl_t)jerry_value_as_number(args[2]);

    lv_btnmatrix_set_btn_ctrl(obj, btn_id, ctrl);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_clear_btn_ctrl(const jerry_call_info_t *call_info_p,
                                              const jerry_value_t args[],
                                              const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (btn_id, ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t btn_id = (uint16_t)jerry_value_as_number(args[1]);
    lv_btnmatrix_ctrl_t ctrl = (lv_btnmatrix_ctrl_t)jerry_value_as_number(args[2]);

    lv_btnmatrix_clear_btn_ctrl(obj, btn_id, ctrl);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_btn_ctrl_all(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    lv_btnmatrix_ctrl_t ctrl = (lv_btnmatrix_ctrl_t)jerry_value_as_number(args[1]);

    lv_btnmatrix_set_btn_ctrl_all(obj, ctrl);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_clear_btn_ctrl_all(const jerry_call_info_t *call_info_p,
                                                  const jerry_value_t args[],
                                                  const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    lv_btnmatrix_ctrl_t ctrl = (lv_btnmatrix_ctrl_t)jerry_value_as_number(args[1]);

    lv_btnmatrix_clear_btn_ctrl_all(obj, ctrl);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_btn_width(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (btn_id, width)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t btn_id = (uint16_t)jerry_value_as_number(args[1]);
    uint8_t width = (uint8_t)jerry_value_as_number(args[2]);

    lv_btnmatrix_set_btn_width(obj, btn_id, width);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_one_checked(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_boolean(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (en)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    bool en = jerry_value_is_true(args[1]);

    lv_btnmatrix_set_one_checked(obj, en);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_get_map(const jerry_call_info_t *call_info_p,
                                       const jerry_value_t args[],
                                       const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    const char **map = lv_btnmatrix_get_map(obj);
    if (map == NULL) {
        return jerry_undefined();
    }

    jerry_value_t js_array = jerry_array(args_count);
    uint32_t index = 0;

    while (map[index] != NULL) {
        jerry_value_t js_str = jerry_string_sz(map[index]);
        jerry_object_set_indexed_property(js_array, index, js_str);
        jerry_value_free(js_str);
        index++;
    }

    return js_array;
}

static jerry_value_t js_lv_obj_get_selected_btn(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t selected_btn = lv_btnmatrix_get_selected_btn(obj);
    return jerry_number(selected_btn);
}

static jerry_value_t js_lv_obj_get_btn_text(const jerry_call_info_p *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (btn_id)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t btn_id = (uint16_t)jerry_value_as_number(args[1]);
    const char *text = lv_btnmatrix_get_btn_text(obj, btn_id);

    if (text == NULL) {
        return jerry_undefined();
    }

    return jerry_create_string((const jerry_char_t *)text);
}

static jerry_value_t js_lv_obj_has_btn_ctrl(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (btn_id, ctrl)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t btn_id = (uint16_t)jerry_value_as_number(args[1]);
    lv_btnmatrix_ctrl_t ctrl = (lv_btnmatrix_ctrl_t)jerry_value_as_number(args[2]);

    bool has_ctrl = lv_btnmatrix_has_btn_ctrl(obj, btn_id, ctrl);
    return jerry_boolean(has_ctrl);
}

static jerry_value_t js_lv_obj_get_one_checked(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    bool checked = lv_btnmatrix_get_one_checked(obj);
    return jerry_boolean(checked);
}

#endif

static jerry_value_t js_lv_obj_on_press(const jerry_call_info_t *call_info_p,
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
    lv_obj_add_event_cb(obj, js_lv_obj_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)args[0]);

    return jerry_undefined();
}

/************************************************************************
* Class register functions
*************************************************************************/

static void jr_lv_obj_class_register(jerry_external_handler_t constructor_handler) {

    jerryx_property_entry methods[] =
    {
        JERRYX_PROPERTY_FUNCTION ("align",           js_obj_align),
        JERRYX_PROPERTY_FUNCTION ("setSize",         js_lv_obj_set_size),
#if 0
        JERRYX_PROPERTY_FUNCTION ("setMap",          js_lv_obj_set_map),
        JERRYX_PROPERTY_FUNCTION ("setCtrlMap",      js_lv_obj_set_ctrl_map),
        JERRYX_PROPERTY_FUNCTION ("setSelectedBtn",  js_lv_obj_set_selected_btn),
        JERRYX_PROPERTY_FUNCTION ("setBtnCtrl",      js_lv_obj_set_btn_ctrl),
        JERRYX_PROPERTY_FUNCTION ("clearBtnCtrl",    js_lv_obj_clear_btn_ctrl),
        JERRYX_PROPERTY_FUNCTION ("setBtnCtrlAll",   js_lv_obj_set_btn_ctrl_all),
        JERRYX_PROPERTY_FUNCTION ("clearBtnCtrlAll", js_lv_obj_clear_btn_ctrl_all),
        JERRYX_PROPERTY_FUNCTION ("setBtnWidth",     js_lv_obj_set_btn_width),
        JERRYX_PROPERTY_FUNCTION ("setOneChecked",   js_lv_obj_set_one_checked),
        JERRYX_PROPERTY_FUNCTION ("getMap",          js_lv_obj_get_map),
        JERRYX_PROPERTY_FUNCTION ("getSelectedBtn",  js_lv_obj_get_selected_btn),
        JERRYX_PROPERTY_FUNCTION ("getBtnText",      js_lv_obj_get_btn_text),
        JERRYX_PROPERTY_FUNCTION ("hasBtnCtrl",      js_lv_obj_has_btn_ctrl),
        JERRYX_PROPERTY_FUNCTION ("getOneChecked",   js_lv_obj_get_one_checked),
#endif
        JERRYX_PROPERTY_FUNCTION ("onPress",         js_lv_obj_on_press),
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

void jr_lv_btnmatrix_init(void) {
    jr_lv_obj_class_register(js_lv_obj_constructor);
}
