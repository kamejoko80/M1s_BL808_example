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
#include "js_lv_arc.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

/* Change the object appropriately */
#define LV_OBJ_NAME           "Arc"
#define LV_OBJ_CREATE(parent) lv_arc_create(parent)

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
    lv_obj_t *obj = (lv_obj_t *) native_p;
    jr_lvgl_obj_desctruct(obj);
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

// lv_arc_set_start_angle
static jerry_value_t js_lv_obj_set_start_angle(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (start)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t start = (uint16_t)jerry_value_as_number(args[0]);
    lv_arc_set_start_angle(obj, start);
    return jerry_undefined();
}

// lv_arc_set_end_angle
static jerry_value_t js_lv_obj_set_end_angle(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (end)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t end = (uint16_t)jerry_value_as_number(args[0]);
    lv_arc_set_end_angle(obj, end);
    return jerry_undefined();
}

// lv_arc_set_angles
static jerry_value_t js_lv_obj_set_angles(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (start, end)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t start = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t end = (uint16_t)jerry_value_as_number(args[1]);
    lv_arc_set_angles(obj, start, end);
    return jerry_undefined();
}

// lv_arc_set_bg_start_angle
static jerry_value_t js_lv_obj_set_bg_start_angle(const jerry_call_info_t *call_info_p,
                                                  const jerry_value_t args[],
                                                  const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (arc, start)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t start = (uint16_t)jerry_value_as_number(args[0]);
    lv_arc_set_bg_start_angle(obj, start);
    return jerry_undefined();
}

// lv_arc_set_bg_end_angle
static jerry_value_t js_lv_obj_set_bg_end_angle(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (end)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t end = (uint16_t)jerry_value_as_number(args[0]);
    lv_arc_set_bg_end_angle(obj, end);
    return jerry_undefined();
}

// lv_arc_set_bg_angles
static jerry_value_t js_lv_obj_set_bg_angles(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (start, end)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t start = (uint16_t)jerry_value_as_number(args[0]);
    uint16_t end = (uint16_t)jerry_value_as_number(args[1]);
    lv_arc_set_bg_angles(obj, start, end);
    return jerry_undefined();
}

// lv_arc_set_rotation
static jerry_value_t js_lv_obj_set_rotation(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (rotation)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t rotation = (uint16_t)jerry_value_as_number(args[0]);
    lv_arc_set_rotation(obj, rotation);
    return jerry_undefined();
}

// lv_arc_set_mode
static jerry_value_t js_lv_obj_set_mode(const jerry_call_info_t *call_info_p,
                                        const jerry_value_t args[],
                                        const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (mode)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    lv_arc_mode_t mode = (lv_arc_mode_t)jerry_value_as_number(args[0]);
    lv_arc_set_mode(obj, mode);
    return jerry_undefined();
}

// lv_arc_set_value
static jerry_value_t js_lv_obj_set_value(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 1 || !jerry_value_is_number(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (value)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int16_t value = (int16_t)jerry_value_as_number(args[0]);
    lv_arc_set_value(obj, value);
    return jerry_undefined();
}

// lv_arc_set_range
static jerry_value_t js_lv_obj_set_range(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (min, max)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if(obj == NULL) {
       return jerry_undefined();
    }

    int32_t min = (int32_t)jerry_value_as_number(args[0]);
    int32_t max = (int32_t)jerry_value_as_number(args[1]);
    lv_arc_set_range(obj, min, max);
    return jerry_undefined();
}

// lv_arc_get_angle_start
static jerry_value_t js_lv_obj_get_angle_start(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t angle = lv_arc_get_angle_start(obj);
    return jerry_number(angle);
}

// lv_arc_get_angle_end
static jerry_value_t js_lv_obj_get_angle_end(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t angle = lv_arc_get_angle_end(obj);
    return jerry_number(angle);
}

// lv_arc_get_bg_angle_start
static jerry_value_t js_lv_obj_get_bg_angle_start(const jerry_call_info_t *call_info_p,
                                                  const jerry_value_t args[],
                                                  const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t angle = lv_arc_get_bg_angle_start(obj);
    return jerry_number(angle);
}

// lv_arc_get_bg_angle_end
static jerry_value_t js_lv_obj_get_bg_angle_end(const jerry_call_info_t *call_info_p,
                                                const jerry_value_t args[],
                                                const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    uint16_t angle = lv_arc_get_bg_angle_end(obj);
    return jerry_number(angle);
}

// lv_arc_get_value
static jerry_value_t js_lv_obj_get_value(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(const lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int16_t value = lv_arc_get_value(obj);
    return jerry_number(value);
}

// lv_arc_get_min_value
static jerry_value_t js_lv_obj_get_min_value(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(const lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    int16_t value = lv_arc_get_min_value(obj);
    return jerry_number(value);
}

// lv_arc_align_obj_to_angle
static jerry_value_t js_lv_obj_align_obj_to_angle(const jerry_call_info_t *call_info_p,
                                                  const jerry_value_t args[],
                                                  const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (obj_to_align, r_offset)");
    }

    JERRY_GET_NATIVE_PTR(const lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj_to_align, args[0], &jerry_obj_native_info);
    if (obj == NULL || obj_to_align == NULL) {
        return jerry_undefined();
    }

    lv_coord_t r_offset = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_arc_align_obj_to_angle(obj, obj_to_align, r_offset);
    return jerry_undefined();
}

// lv_arc_rotate_obj_to_angle
static jerry_value_t js_lv_obj_rotate_obj_to_angle(const jerry_call_info_t *call_info_p,
                                                   const jerry_value_t args[],
                                                   const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (obj_to_rotate, r_offset)");
    }

    JERRY_GET_NATIVE_PTR(const lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    JERRY_GET_NATIVE_PTR(lv_obj_t, obj_to_rotate, args[0], &jerry_obj_native_info);
    if (obj == NULL || obj_to_rotate == NULL) {
        return jerry_undefined();
    }

    lv_coord_t r_offset = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_arc_rotate_obj_to_angle(obj, obj_to_rotate, r_offset);
    return jerry_undefined();
}

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
        JERRYX_PROPERTY_FUNCTION ("align",            js_lv_obj_align),
        JERRYX_PROPERTY_FUNCTION ("setSize",          js_lv_obj_set_size),
        JERRYX_PROPERTY_FUNCTION ("setStartAngle",    js_lv_obj_set_start_angle),
        JERRYX_PROPERTY_FUNCTION ("setEndAngle",      js_lv_obj_set_end_angle),
        JERRYX_PROPERTY_FUNCTION ("setAngles",        js_lv_obj_set_angles),
        JERRYX_PROPERTY_FUNCTION ("setBgStartAngle",  js_lv_obj_set_bg_start_angle),
        JERRYX_PROPERTY_FUNCTION ("setBgEndAngle",    js_lv_obj_set_bg_end_angle),
        JERRYX_PROPERTY_FUNCTION ("setBgAngle",       js_lv_obj_set_bg_angles),
        JERRYX_PROPERTY_FUNCTION ("setRotation",      js_lv_obj_set_rotation),
        JERRYX_PROPERTY_FUNCTION ("setMode",          js_lv_obj_set_mode),
        JERRYX_PROPERTY_FUNCTION ("setValue",         js_lv_obj_set_value),
        JERRYX_PROPERTY_FUNCTION ("setRange",         js_lv_obj_set_range),
        JERRYX_PROPERTY_FUNCTION ("getAngleStart",    js_lv_obj_get_angle_start),
        JERRYX_PROPERTY_FUNCTION ("getAngleEnd",      js_lv_obj_get_angle_end),
        JERRYX_PROPERTY_FUNCTION ("getBgAngleStart",  js_lv_obj_get_bg_angle_start),
        JERRYX_PROPERTY_FUNCTION ("getBgAngleEnd",    js_lv_obj_get_bg_angle_end),
        JERRYX_PROPERTY_FUNCTION ("getValue",         js_lv_obj_get_value),
        JERRYX_PROPERTY_FUNCTION ("getMinValue",      js_lv_obj_get_min_value),
        JERRYX_PROPERTY_FUNCTION ("alignObjToAngle",  js_lv_obj_align_obj_to_angle),
        JERRYX_PROPERTY_FUNCTION ("rotateObjToAngle", js_lv_obj_rotate_obj_to_angle),
        JERRYX_PROPERTY_FUNCTION ("onPress",          js_lv_obj_on_press),
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

void jr_lv_arc_init(void) {
    jr_lv_obj_class_register(js_lv_obj_constructor);
}
