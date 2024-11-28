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

// Define user data structure to store callback info
typedef struct {
    jerry_value_t js_callback; // JavaScript function callback
} btn_user_data_t;

void js_lv_btn_event_cb(lv_event_t *e) {

    printf("js_lv_btn_event_cb\n");

    // Get the JavaScript callback from the user data
    jerry_value_t js_callback = (jerry_value_t)lv_event_get_user_data(e);

    printf("Function call js_callback %u\n", js_callback);

    if (!jerry_value_is_function(js_callback)) {
        printf("Error: Invalid JS callback\n");
        return;
    }

    printf("Cb pass 1\r\n");

    // Call the JS callback with the event code as an argument
    jerry_value_t args[1];
    args[0] = jerry_number(lv_event_get_code(e)); // Pass event code to JS
    jerry_value_t result = jerry_call(js_callback, jerry_undefined(), args, 1);

    if (jerry_value_is_exception(result)) {
        printf("Error: Exception thrown in JS callback\n");
    }

    jerry_value_free(result);
    jerry_value_free(args[0]);
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
    
    // Only clean up if necessary
    if (native_p) {
        lv_obj_t *btn = (lv_obj_t *)native_p;
        lv_obj_del(btn);
    }
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

    jerry_object_set_native_ptr(call_info_p->this_value, /* jerry_value_t object */
                                &jerry_obj_native_info,  /* const jerry_object_native_info_t *native_info_p */
                                btn                      /* void *native_pointer_p */
                                );
                                
    // Retain the JS object to prevent it from being garbage collected
    jerry_value_t global = jerry_current_realm();
    jerry_object_set(global, jerry_string_sz("retainedButton"), call_info_p->this_value);
    jerry_value_free(global);                                
                                                     
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
    printf("js_lv_btn_on_press\n");
    if (args_count < 1 || !jerry_value_is_function(args[0])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected a callback function");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, btn, call_info_p->this_value, &jerry_obj_native_info);
    if(btn == NULL) {
       printf("Cannot get btn object\n");
       return jerry_undefined();
    }

    printf("Pass 1\n");
    printf("Function call %u\n", args[0]);

    // Register the event callback
    lv_obj_add_event_cb(btn, js_lv_btn_event_cb, LV_EVENT_CLICKED, (void *)args[0]);
    printf("Pass 2\n");

    // Retain the JS function to ensure it isn't garbage collected
    //jerry_value_t retained_callback = args[0];
    //jerry_value_t global = jerry_current_realm();
    //jerry_object_set(global, jerry_string_sz("retainedCallback"), retained_callback);
    //jerry_value_free(global);

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
