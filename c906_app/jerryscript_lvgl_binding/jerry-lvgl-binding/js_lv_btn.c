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
#include "js_lv_btn.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

#define LV_LABEL_OBJ_NAME "Button"
#define FORMAT_TEXT_SIZE  256

/************************************************************************
* Native event handler for LVGL
*************************************************************************/

void js_lv_btn_event_cb(lv_event_t *event) {
    
    printf("js_lv_btn_event_cb\n");        
    lv_obj_t *btn = lv_event_get_target(event);
    // Retrieve the JavaScript callback from the user data
    jerry_value_t callback = (jerry_value_t)(uintptr_t)lv_obj_get_user_data(btn);
    if (jerry_value_is_function(callback)) {
        jerry_value_t global_object = jerry_current_realm();
        jerry_value_t ret_value = jerry_call(callback, global_object, NULL, 0);
        jerry_value_free(global_object);

        if (jerry_value_is_error(ret_value)) {
            printf("Error in JS callback\n");
        }

        jerry_value_free(ret_value);
    }
}

/************************************************************************
* Constructor & Desctructor
*************************************************************************/

static void js_lv_btn_destructor_cb(void *native_p, jerry_object_native_info_t *call_info_p) {
    printf("js_lv_btn_destructor_cb\n");
    /*
     * Temporally dissable because JS gabage collector
     * automaticlly destroys the JS obj
     */
    //lv_obj_t *btn = (lv_obj_t *) native_p;
    //jr_lvgl_obj_desctruct(btn);
}

static jerry_object_native_info_t jerry_obj_native_info = {
    .free_cb = js_lv_btn_destructor_cb,
};

static jerry_value_t js_lv_btn_constructor(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    printf("js_lv_btn_constructor\n");
    if (args_count < 1 || !jerry_value_is_object(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected object .");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, parent, args[0], NULL);
    lv_obj_t *btn = lv_btn_create(parent);
    if (btn == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to create button");
    }

    lv_obj_set_user_data(btn, (void *)(uintptr_t)call_info_p->this_value);
    jerry_object_set_native_ptr(call_info_p->this_value, /* jerry_value_t object */
                                &jerry_obj_native_info,  /* const jerry_object_native_info_t *native_info_p */
                                btn                      /* void *native_pointer_p */
                                );
    return jerry_undefined();
}

/************************************************************************
* Protperties, methodes definition
*************************************************************************/

static jerry_value_t js_obj_align(const jerry_call_info_t *call_info_p,
                                  const jerry_value_t args[],
                                  const jerry_length_t args_count) {
    // Ensure at least 3 arguments: `align`, `x_ofs`, `y_ofs`
    if (args_count < 3) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Insufficient arguments");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, btn, call_info_p->this_value, &jerry_obj_native_info);
    if(btn == NULL) {
       return jerry_undefined();
    }

    uint32_t align = (uint32_t)jerry_value_as_number(args[0]);
    lv_coord_t x_ofs = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t y_ofs = (lv_coord_t)jerry_value_as_number(args[2]);

    // Call LVGL function
    lv_obj_align(btn, align, x_ofs, y_ofs);

    return jerry_undefined();
}

static jerry_value_t js_lv_obj_set_size(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_cnt) {
    if (args_cnt < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected (width, height)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, btn, call_info_p->this_value, &jerry_obj_native_info);
    if(btn == NULL) {
       return jerry_undefined();
    }

    lv_coord_t width = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t height = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_obj_set_size(btn, width, height);
    return jerry_undefined();
}

static jerry_value_t js_lv_btn_on_press(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_function(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid arguments. Expected a callback function.");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, btn, call_info_p->this_value, &jerry_obj_native_info);
    if(btn == NULL) {
       return jerry_undefined();
    }

    // Store the JavaScript callback in the user data
    jerry_value_t callback = jerry_value_copy(args[0]); // Retain the JS function
    lv_obj_set_user_data(btn, (void *)callback);

    // Set the LVGL button event callback
    lv_obj_add_event_cb(btn, js_lv_btn_event_cb, LV_EVENT_CLICKED, btn);

    return jerry_undefined();
}

/************************************************************************
* Class register functions
*************************************************************************/

static void jr_lv_btn_class_register(jerry_external_handler_t constructor_handler) {

    jerryx_property_entry methods[] =
    {
        JERRYX_PROPERTY_FUNCTION ("align",   js_obj_align),
        JERRYX_PROPERTY_FUNCTION ("setSize", js_lv_obj_set_size),
        JERRYX_PROPERTY_FUNCTION ("onPress", js_lv_btn_on_press),
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
    jerry_value_t constructor_name = jerry_string_sz(LV_LABEL_OBJ_NAME);
    jerry_object_set(global_obj, constructor_name, constructor);
    jerry_value_free(constructor_name);
    jerry_value_free(constructor);
    jerry_value_free(global_obj);
}

void jr_lv_btn_init(void) {
    jr_lv_btn_class_register(js_lv_btn_constructor);
}
