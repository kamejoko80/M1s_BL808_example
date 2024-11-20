#include "duktape.h"
#include "lvgl.h"

/* Function to create an arc widget in LVGL */
static duk_ret_t lv_arc_create_js(duk_context *ctx) {
    /* Create an arc widget on the active screen */
    lv_obj_t *arc = lv_arc_create(lv_scr_act());  // lv_scr_act() gets the active screen

    /* Return the object pointer as the result of the function */
    duk_push_pointer(ctx, arc);
    return 1;  // Number of return values
}

/* Function to set the value of an arc widget */
static duk_ret_t lv_arc_set_value_js(duk_context *ctx) {
    /* Get the arc object from the stack */
    lv_obj_t *arc = (lv_obj_t *)duk_get_pointer(ctx, 0);

    /* Get the value to set from the stack */
    int value = duk_get_int(ctx, 1);

    /* Set the arc value */
    lv_arc_set_value(arc, value);

    return 0;  // No return value
}

// Register the LVGL functions to Duktape
void register_lv_arc_bindings(duk_context *ctx) {

    duk_push_c_function(ctx, lv_arc_create_js, 0);
    duk_put_global_string(ctx, "lv_arc_create");

    duk_push_c_function(ctx, lv_arc_set_value_js, 2);
    duk_put_global_string(ctx, "lv_arc_set_value"); 
}
