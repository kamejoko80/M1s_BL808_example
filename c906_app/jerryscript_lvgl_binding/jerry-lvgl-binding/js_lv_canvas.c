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
#define LV_OBJ_NAME           "Canvas"
#define LV_OBJ_CREATE(parent) lv_canvas_create(parent)

/************************************************************************
* Typedef
*************************************************************************/

typedef struct {
    void   *buf;
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

    /* user data settings */
    jerry_lv_user_data_t *user_data = malloc(sizeof(jerry_lv_user_data_t));
    lv_obj_set_user_data(obj, user_data);

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

static jerry_value_t js_lv_obj_set_buffer(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) ||
                          !jerry_value_is_number(args[1]) ||
                          !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (buf, w, h, cf)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    lv_coord_t w = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t h = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_img_cf_t cf = (lv_img_cf_t)jerry_value_as_number(args[2]);

    uint32_t pixel_size = w * h * sizeof(lv_color_t);

    void *buf = malloc(pixel_size);
    if (buf == NULL) {
        printf("%s %s error buffer\n", LV_OBJ_NAME, __FUNCTION__);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    memset(buf, 0, pixel_size);

    /* store buffer pointer into user data */
    jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    if(user_data != NULL){
        user_data->buf = buf;
    }

    lv_canvas_set_buffer(obj, buf, w, h, cf);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_set_px_color(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y, color)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[2]);
    lv_color_t color = jr_lvgl_color_to_color_t(color_value);

    lv_canvas_set_px_color(canvas, x, y, color);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_set_px_opa(const jerry_call_info_t *call_info_p,
                                             const jerry_value_t args[],
                                             const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_number(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y, opa)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_opa_t opa = (lv_opa_t)jerry_value_as_number(args[2]);

    lv_canvas_set_px_opa(canvas, x, y, opa);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_set_palette(const jerry_call_info_t *call_info_p,
                                              const jerry_value_t args[],
                                              const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (id, color)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    uint8_t id = (uint8_t)jerry_value_as_number(args[0]);
    uint32_t color_value = (uint32_t)jerry_value_as_number(args[1]);
    lv_color_t color = jr_lvgl_color_to_color_t(color_value);

    lv_canvas_set_palette(canvas, id, color);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_get_px(const jerry_call_info_t *call_info_p,
                                         const jerry_value_t args[],
                                         const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_color_t color = lv_canvas_get_px(canvas, x, y);
    uint32_t color_value = jr_lvgl_color_t_to_color(color);
    return jerry_number(color_value);
}

static jerry_value_t js_lv_canvas_get_img(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    // Retrieve the native lv_obj_t pointer
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Get the lv_img_dsc_t from the canvas
    lv_img_dsc_t *img = lv_canvas_get_img(canvas);
    if (img == NULL) {
        return jerry_undefined();
    }

    // Create a new JavaScript object to represent lv_img_dsc_t
    jerry_value_t js_img = jerry_object();

    // Add the `header` object
    jerry_value_t js_header = jerry_object();
    jerry_object_set(js_header, jerry_string_sz("width"), jerry_number(img->header.w));
    jerry_object_set(js_header, jerry_string_sz("height"), jerry_number(img->header.h));
    jerry_object_set(js_header, jerry_string_sz("cf"), jerry_number(img->header.cf));
    jerry_object_set(js_img, jerry_string_sz("header"), js_header);
    jerry_value_free(js_header);

    // Add the `data` buffer (if applicable)
    if (img->data != NULL) {
        size_t data_size = img->header.w * img->header.h * sizeof(lv_color_t);
        jerry_value_t js_data = jerry_arraybuffer_external((uint8_t *)img->data, data_size, NULL);
        jerry_object_set(js_img, jerry_string_sz("data"), js_data);
        jerry_value_free(js_data);
    } else {
        jerry_object_set(js_img, jerry_string_sz("data"), jerry_undefined());
    }

    return js_img;
}

static jerry_value_t js_lv_canvas_copy_buf(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    // Validate argument count and types
    if (args_count < 5 || !jerry_value_is_arraybuffer(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2]) ||
        !jerry_value_is_number(args[3]) || !jerry_value_is_number(args[4])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (to_copy, x, y, w, h)");
    }

    // Get the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Get the ArrayBuffer from args[0]
    jerry_value_t arraybuffer = args[0];
    jerry_length_t buf_size = jerry_arraybuffer_size(arraybuffer);

    // Allocate a native buffer
    void *to_copy = malloc(buf_size);
    if (to_copy == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    // Copy the data from the ArrayBuffer to the native buffer
    jerry_arraybuffer_read(arraybuffer, 0, to_copy, buf_size);

    // Retrieve the other arguments
    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t w = (lv_coord_t)jerry_value_as_number(args[3]);
    lv_coord_t h = (lv_coord_t)jerry_value_as_number(args[4]);

    // Execute the LVGL function
    lv_canvas_copy_buf(canvas, to_copy, x, y, w, h);

    // Free the native buffer
    free(to_copy);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_transform(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 8 || !jerry_value_is_object(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2]) ||
        !jerry_value_is_number(args[3]) || !jerry_value_is_number(args[4]) ||
        !jerry_value_is_number(args[5]) || !jerry_value_is_number(args[6]) ||
        !jerry_value_is_boolean(args[7])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (img, angle, zoom, offset_x, offset_y, pivot_x, pivot_y, antialias)");
    }

    // Get the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Extract `img` properties from the JavaScript object
    jerry_value_t img_obj = args[0];

    // Extract `data` property (ArrayBuffer)
    jerry_value_t data_val = jerry_object_get(img_obj, jerry_string_sz("data"));
    if (!jerry_value_is_arraybuffer(data_val)) {
        jerry_value_free(data_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected 'data' to be an ArrayBuffer");
    }

    // Extract buffer size and copy data
    jerry_length_t buf_size = jerry_arraybuffer_size(data_val);
    void *img_data = malloc(buf_size);
    if (img_data == NULL) {
        jerry_value_free(data_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed for image data");
    }
    jerry_arraybuffer_read(data_val, 0, img_data, buf_size);
    jerry_value_free(data_val);

    // Extract `width`, `height`, and `cf` (color format)
    jerry_value_t width_val = jerry_object_get(img_obj, jerry_string_sz("width"));
    jerry_value_t height_val = jerry_object_get(img_obj, jerry_string_sz("height"));
    jerry_value_t cf_val = jerry_object_get(img_obj, jerry_string_sz("cf"));

    if (!jerry_value_is_number(width_val) || !jerry_value_is_number(height_val) || !jerry_value_is_number(cf_val)) {
        free(img_data);
        jerry_value_free(width_val);
        jerry_value_free(height_val);
        jerry_value_free(cf_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid or missing 'width', 'height', or 'cf' properties");
    }

    lv_img_dsc_t img;
    img.data = img_data;
    img.header.w = (lv_coord_t)jerry_value_as_number(width_val);
    img.header.h = (lv_coord_t)jerry_value_as_number(height_val);
    img.header.cf = (lv_img_cf_t)jerry_value_as_number(cf_val);

    jerry_value_free(width_val);
    jerry_value_free(height_val);
    jerry_value_free(cf_val);

    // Retrieve the other arguments
    int16_t angle = (int16_t)jerry_value_as_number(args[1]);
    uint16_t zoom = (uint16_t)jerry_value_as_number(args[2]);
    lv_coord_t offset_x = (lv_coord_t)jerry_value_as_number(args[3]);
    lv_coord_t offset_y = (lv_coord_t)jerry_value_as_number(args[4]);
    int32_t pivot_x = (int32_t)jerry_value_as_number(args[5]);
    int32_t pivot_y = (int32_t)jerry_value_as_number(args[6]);
    bool antialias = jerry_value_is_true(args[7]);

    // Call the LVGL transformation function
    lv_canvas_transform(canvas, &img, angle, zoom, offset_x, offset_y, pivot_x, pivot_y, antialias);

    // Free the allocated image data
    free(img_data);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_blur_hor(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (area, r)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Extract area properties from the JavaScript object
    jerry_value_t area_obj = args[0];
    jerry_value_t x1_val = jerry_object_get(area_obj, jerry_string_sz("x1"));
    jerry_value_t y1_val = jerry_object_get(area_obj, jerry_string_sz("y1"));
    jerry_value_t x2_val = jerry_object_get(area_obj, jerry_string_sz("x2"));
    jerry_value_t y2_val = jerry_object_get(area_obj, jerry_string_sz("y2"));

    if (!jerry_value_is_number(x1_val) || !jerry_value_is_number(y1_val) ||
        !jerry_value_is_number(x2_val) || !jerry_value_is_number(y2_val)) {
        jerry_value_free(x1_val);
        jerry_value_free(y1_val);
        jerry_value_free(x2_val);
        jerry_value_free(y2_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid or missing properties in area object");
    }

    // Build the lv_area_t structure
    lv_area_t area;
    area.x1 = (lv_coord_t)jerry_value_as_number(x1_val);
    area.y1 = (lv_coord_t)jerry_value_as_number(y1_val);
    area.x2 = (lv_coord_t)jerry_value_as_number(x2_val);
    area.y2 = (lv_coord_t)jerry_value_as_number(y2_val);

    // Free JavaScript property values
    jerry_value_free(x1_val);
    jerry_value_free(y1_val);
    jerry_value_free(x2_val);
    jerry_value_free(y2_val);

    // Retrieve the blur radius
    uint16_t r = (uint16_t)jerry_value_as_number(args[1]);

    // Call the LVGL blur function
    lv_canvas_blur_hor(canvas, &area, r);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_blur_ver(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (area, r)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Extract area properties from the JavaScript object
    jerry_value_t area_obj = args[0];
    jerry_value_t x1_val = jerry_object_get(area_obj, jerry_string_sz("x1"));
    jerry_value_t y1_val = jerry_object_get(area_obj, jerry_string_sz("y1"));
    jerry_value_t x2_val = jerry_object_get(area_obj, jerry_string_sz("x2"));
    jerry_value_t y2_val = jerry_object_get(area_obj, jerry_string_sz("y2"));

    if (!jerry_value_is_number(x1_val) || !jerry_value_is_number(y1_val) ||
        !jerry_value_is_number(x2_val) || !jerry_value_is_number(y2_val)) {
        jerry_value_free(x1_val);
        jerry_value_free(y1_val);
        jerry_value_free(x2_val);
        jerry_value_free(y2_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid or missing properties in area object");
    }

    // Build the lv_area_t structure
    lv_area_t area;
    area.x1 = (lv_coord_t)jerry_value_as_number(x1_val);
    area.y1 = (lv_coord_t)jerry_value_as_number(y1_val);
    area.x2 = (lv_coord_t)jerry_value_as_number(x2_val);
    area.y2 = (lv_coord_t)jerry_value_as_number(y2_val);

    // Free JavaScript property values
    jerry_value_free(x1_val);
    jerry_value_free(y1_val);
    jerry_value_free(x2_val);
    jerry_value_free(y2_val);

    // Retrieve the blur radius
    uint16_t r = (uint16_t)jerry_value_as_number(args[1]);

    // Call the LVGL blur function
    lv_canvas_blur_ver(canvas, &area, r);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_fill_bg(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_number(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (color, opa)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    uint32_t color_full = (uint32_t)jerry_value_as_number(args[0]);
    lv_color_t color = jr_lvgl_color_to_color_t(color_full);
    lv_opa_t opa = (lv_opa_t)jerry_value_as_number(args[1]);
    lv_canvas_fill_bg(canvas, color, opa);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_draw_rect(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 5 || !jerry_value_is_number(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2]) ||
        !jerry_value_is_number(args[3]) || !jerry_value_is_object(args[4])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y, w, h, draw_dsc)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Extract the rectangle's position and size
    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t w = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t h = (lv_coord_t)jerry_value_as_number(args[3]);

    // Extract the draw descriptor object
    jerry_value_t draw_dsc_obj = args[4];
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc); // Initialize the descriptor with default values

    // Assign draw descriptor properties
    SET_FIELD_IF_PRESENT("radius", radius, lv_coord_t);
    SET_FIELD_IF_PRESENT("blend_mode", blend_mode, lv_blend_mode_t);
    SET_FIELD_IF_PRESENT("bg_opa", bg_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("border_width", border_width, lv_coord_t);
    SET_FIELD_IF_PRESENT("border_opa", border_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("outline_width", outline_width, lv_coord_t);
    SET_FIELD_IF_PRESENT("outline_pad", outline_pad, lv_coord_t);
    SET_FIELD_IF_PRESENT("outline_opa", outline_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("shadow_width", shadow_width, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_ofs_x", shadow_ofs_x, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_ofs_y", shadow_ofs_y, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_spread", shadow_spread, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_opa", shadow_opa, lv_opa_t);

    // For fields like `bg_color` that use `lv_color_t`
    jerry_value_t bg_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("bg_color"));
    if (jerry_value_is_number(bg_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(bg_color_val);
        draw_dsc.bg_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(bg_color_val);

    jerry_value_t border_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("border_color"));
    if (jerry_value_is_number(border_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(border_color_val);
        draw_dsc.border_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(border_color_val);

    jerry_value_t shadow_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("shadow_color"));
    if (jerry_value_is_number(shadow_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(shadow_color_val);
        draw_dsc.shadow_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(shadow_color_val);

    // Call the LVGL function
    lv_canvas_draw_rect(canvas, x, y, w, h, &draw_dsc);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_draw_text(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 5 || !jerry_value_is_number(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2]) ||
        !jerry_value_is_object(args[3]) || !jerry_value_is_string(args[4])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y, max_w, draw_dsc, txt)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Extract the text position and maximum width
    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t max_w = (lv_coord_t)jerry_value_as_number(args[2]);

    // Extract the draw descriptor object
    jerry_value_t draw_dsc_obj = args[3];
    lv_draw_label_dsc_t draw_dsc;
    lv_draw_label_dsc_init(&draw_dsc); // Initialize the descriptor with default values

    // Assign draw descriptor properties
    SET_FIELD_IF_PRESENT("line_space", line_space, lv_coord_t);
    SET_FIELD_IF_PRESENT("letter_space", letter_space, lv_coord_t);
    SET_FIELD_IF_PRESENT("ofs_x", ofs_x, lv_coord_t);
    SET_FIELD_IF_PRESENT("ofs_y", ofs_y, lv_coord_t);
    SET_FIELD_IF_PRESENT("opa", opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("bidi_dir", bidi_dir, lv_base_dir_t);
    SET_FIELD_IF_PRESENT("align", align, lv_text_align_t);
    SET_FIELD_IF_PRESENT("flag", flag, lv_text_flag_t);
    SET_FIELD_IF_PRESENT("decor", decor, lv_text_decor_t);
    SET_FIELD_IF_PRESENT("blend_mode", blend_mode, lv_blend_mode_t);

    // For fields like `color`, `sel_color`, etc., which use `lv_color_t`
    jerry_value_t color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("color"));
    if (jerry_value_is_number(color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(color_val);
        draw_dsc.color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(color_val);

    jerry_value_t sel_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("sel_color"));
    if (jerry_value_is_number(sel_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(sel_color_val);
        draw_dsc.sel_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(sel_color_val);

    jerry_value_t sel_bg_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("sel_bg_color"));
    if (jerry_value_is_number(sel_bg_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(sel_bg_color_val);
        draw_dsc.sel_bg_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(sel_bg_color_val);

    // Extract the text string
    jerry_size_t txt_len = jerry_string_size(args[4], JERRY_ENCODING_UTF8);
    char *txt = malloc(txt_len + 1);
    if (txt == NULL) {
        return jerry_undefined();
    }
    jerry_string_to_buffer(args[4], JERRY_ENCODING_UTF8, (jerry_char_t *)txt, txt_len);
    txt[txt_len] = '\0';

    // Call the LVGL function
    lv_canvas_draw_text(canvas, x, y, max_w, &draw_dsc, txt);

    // Free the allocated text
    free(txt);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_draw_img(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    if (args_count < 4 || !jerry_value_is_number(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_object(args[2]) ||
        !jerry_value_is_object(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y, src, draw_dsc)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Extract x and y coordinates
    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);

    // Extract and build the `lv_img_dsc_t` source
    jerry_value_t src_obj = args[2];
    lv_img_dsc_t src;
    jerry_value_t data_val = jerry_object_get(src_obj, jerry_string_sz("data"));
    if (!jerry_value_is_arraybuffer(data_val)) {
        printf("failed 1\n");
        jerry_value_free(data_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected 'data' to be an ArrayBuffer");
    }

    jerry_length_t buf_size = jerry_arraybuffer_size(data_val);
    void *data_buffer = malloc(buf_size);
    if (data_buffer == NULL) {
        jerry_value_free(data_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed for src data");
    }
    jerry_arraybuffer_read(data_val, 0, data_buffer, buf_size);
    jerry_value_free(data_val);

    // Extract other properties of `lv_img_dsc_t`
    jerry_value_t width_val = jerry_object_get(src_obj, jerry_string_sz("width"));
    jerry_value_t height_val = jerry_object_get(src_obj, jerry_string_sz("height"));
    jerry_value_t cf_val = jerry_object_get(src_obj, jerry_string_sz("cf"));

    if (!jerry_value_is_number(width_val) || !jerry_value_is_number(height_val) || !jerry_value_is_number(cf_val)) {
        printf("failed 3\n");
        free(data_buffer);
        jerry_value_free(width_val);
        jerry_value_free(height_val);
        jerry_value_free(cf_val);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid or missing 'width', 'height', or 'cf' in src object");
    }

    src.data = data_buffer;
    src.data_size = buf_size;
    src.header.always_zero = 0;
    src.header.w = (lv_coord_t)jerry_value_as_number(width_val);
    src.header.h = (lv_coord_t)jerry_value_as_number(height_val);
    src.header.cf = (lv_img_cf_t)jerry_value_as_number(cf_val);

    jerry_value_free(width_val);
    jerry_value_free(height_val);
    jerry_value_free(cf_val);

    // Extract draw descriptor object
    jerry_value_t draw_dsc_obj = args[3];
    lv_draw_img_dsc_t draw_dsc;
    lv_draw_img_dsc_init(&draw_dsc); // Initialize with default values

    // Assign draw descriptor properties
    SET_FIELD_IF_PRESENT("angle", angle, int16_t);
    SET_FIELD_IF_PRESENT("zoom", zoom, uint16_t);
    SET_FIELD_IF_PRESENT("recolor_opa", recolor_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("opa", opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("blend_mode", blend_mode, lv_blend_mode_t);
    SET_FIELD_IF_PRESENT("frame_id", frame_id, int32_t);

    // Handle `pivot`
    jerry_value_t pivot_obj = jerry_object_get(draw_dsc_obj, jerry_string_sz("pivot"));
    if (jerry_value_is_object(pivot_obj)) {
        jerry_value_t x_val = jerry_object_get(pivot_obj, jerry_string_sz("x"));
        jerry_value_t y_val = jerry_object_get(pivot_obj, jerry_string_sz("y"));
        if (jerry_value_is_number(x_val) && jerry_value_is_number(y_val)) {
            draw_dsc.pivot.x = (lv_coord_t)jerry_value_as_number(x_val);
            draw_dsc.pivot.y = (lv_coord_t)jerry_value_as_number(y_val);
        }
        jerry_value_free(x_val);
        jerry_value_free(y_val);
    }
    jerry_value_free(pivot_obj);

    // Handle `recolor`
    jerry_value_t recolor_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("recolor"));
    if (jerry_value_is_number(recolor_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(recolor_val);
        draw_dsc.recolor = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(recolor_val);

    // Handle `antialias`
    jerry_value_t antialias_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("antialias"));
    if (jerry_value_is_boolean(antialias_val)) {
        draw_dsc.antialias = jerry_value_is_true(antialias_val) ? 1 : 0;
    }
    jerry_value_free(antialias_val);

    // Call the LVGL function
    lv_canvas_draw_img(canvas, x, y, &src, &draw_dsc);

    // Free allocated memory
    free(data_buffer);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_draw_line(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_array(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_object(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (points, point_cnt, draw_dsc)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Get the number of points
    uint32_t point_cnt = (uint32_t)jerry_value_as_number(args[1]);
    if (point_cnt == 0) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "point_cnt must be greater than 0");
    }

    // Allocate memory for points
    lv_point_t *points = malloc(sizeof(lv_point_t) * point_cnt);
    if (points == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed for points");
    }

    // Iterate through the points array
    jerry_value_t points_array = args[0];
    for (uint32_t i = 0; i < point_cnt; i++) {
        jerry_value_t point_obj = jerry_object_get_index(points_array, i);
        if (!jerry_value_is_object(point_obj)) {
            free(points);
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid point format in array");
        }

        // Extract x and y coordinates
        jerry_value_t x_val = jerry_object_get(point_obj, jerry_string_sz("x"));
        jerry_value_t y_val = jerry_object_get(point_obj, jerry_string_sz("y"));

        if (!jerry_value_is_number(x_val) || !jerry_value_is_number(y_val)) {
            jerry_value_free(x_val);
            jerry_value_free(y_val);
            jerry_value_free(point_obj);
            free(points);
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid x or y value in point object");
        }

        points[i].x = (lv_coord_t)jerry_value_as_number(x_val);
        points[i].y = (lv_coord_t)jerry_value_as_number(y_val);

        // Free temporary values
        jerry_value_free(x_val);
        jerry_value_free(y_val);
        jerry_value_free(point_obj);
    }

    // Extract and initialize the draw descriptor
    jerry_value_t draw_dsc_obj = args[2];
    lv_draw_line_dsc_t draw_dsc;
    lv_draw_line_dsc_init(&draw_dsc); // Initialize with default values

    SET_FIELD_IF_PRESENT("width", width, lv_coord_t);
    SET_FIELD_IF_PRESENT("opa", opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("blend_mode", blend_mode, lv_blend_mode_t);
    SET_FIELD_IF_PRESENT("dash_width", dash_width, lv_coord_t);
    SET_FIELD_IF_PRESENT("dash_gap", dash_gap, lv_coord_t);

    // Handle color
    jerry_value_t color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("color"));
    if (jerry_value_is_number(color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(color_val);
        draw_dsc.color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(color_val);

    // Handle boolean flags: round_start, round_end, raw_end
    jerry_value_t round_start_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("round_start"));
    if (jerry_value_is_boolean(round_start_val)) {
        draw_dsc.round_start = jerry_value_is_true(round_start_val);
    }
    jerry_value_free(round_start_val);

    jerry_value_t round_end_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("round_end"));
    if (jerry_value_is_boolean(round_end_val)) {
        draw_dsc.round_end = jerry_value_is_true(round_end_val);
    }
    jerry_value_free(round_end_val);

    jerry_value_t raw_end_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("raw_end"));
    if (jerry_value_is_boolean(raw_end_val)) {
        draw_dsc.raw_end = jerry_value_is_true(raw_end_val);
    }
    jerry_value_free(raw_end_val);

    // Draw the line
    lv_canvas_draw_line(canvas, points, point_cnt, &draw_dsc);

    // Free allocated memory
    free(points);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_draw_polygon(const jerry_call_info_t *call_info_p,
                                               const jerry_value_t args[],
                                               const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_array(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_object(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (points, point_cnt, draw_dsc)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Get the number of points
    uint32_t point_cnt = (uint32_t)jerry_value_as_number(args[1]);
    if (point_cnt == 0) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "point_cnt must be greater than 0");
    }

    // Allocate memory for points
    lv_point_t *points = malloc(sizeof(lv_point_t) * point_cnt);
    if (points == NULL) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed for points");
    }

    // Iterate through the points array
    jerry_value_t points_array = args[0];
    for (uint32_t i = 0; i < point_cnt; i++) {
        jerry_value_t point_obj = jerry_object_get_index(points_array, i);
        if (!jerry_value_is_object(point_obj)) {
            free(points);
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid point format in array");
        }

        // Extract x and y coordinates
        jerry_value_t x_val = jerry_object_get(point_obj, jerry_string_sz("x"));
        jerry_value_t y_val = jerry_object_get(point_obj, jerry_string_sz("y"));

        if (!jerry_value_is_number(x_val) || !jerry_value_is_number(y_val)) {
            jerry_value_free(x_val);
            jerry_value_free(y_val);
            jerry_value_free(point_obj);
            free(points);
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid x or y value in point object");
        }

        points[i].x = (lv_coord_t)jerry_value_as_number(x_val);
        points[i].y = (lv_coord_t)jerry_value_as_number(y_val);

        // Free temporary values
        jerry_value_free(x_val);
        jerry_value_free(y_val);
        jerry_value_free(point_obj);
    }

    // Extract and initialize the draw descriptor
    jerry_value_t draw_dsc_obj = args[2];
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc); // Initialize with default values

    // Assign draw descriptor properties
    SET_FIELD_IF_PRESENT("radius", radius, lv_coord_t);
    SET_FIELD_IF_PRESENT("bg_opa", bg_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("bg_img_opa", bg_img_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("bg_img_recolor_opa", bg_img_recolor_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("border_width", border_width, lv_coord_t);
    SET_FIELD_IF_PRESENT("border_opa", border_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("outline_width", outline_width, lv_coord_t);
    SET_FIELD_IF_PRESENT("outline_pad", outline_pad, lv_coord_t);
    SET_FIELD_IF_PRESENT("outline_opa", outline_opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("shadow_width", shadow_width, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_ofs_x", shadow_ofs_x, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_ofs_y", shadow_ofs_y, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_spread", shadow_spread, lv_coord_t);
    SET_FIELD_IF_PRESENT("shadow_opa", shadow_opa, lv_opa_t);

    // Handle colors
    jerry_value_t bg_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("bg_color"));
    if (jerry_value_is_number(bg_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(bg_color_val);
        draw_dsc.bg_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(bg_color_val);

    jerry_value_t border_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("border_color"));
    if (jerry_value_is_number(border_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(border_color_val);
        draw_dsc.border_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(border_color_val);

    jerry_value_t outline_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("outline_color"));
    if (jerry_value_is_number(outline_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(outline_color_val);
        draw_dsc.outline_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(outline_color_val);

    jerry_value_t shadow_color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("shadow_color"));
    if (jerry_value_is_number(shadow_color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(shadow_color_val);
        draw_dsc.shadow_color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(shadow_color_val);

    // Handle boolean flags
    jerry_value_t bg_img_tiled_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("bg_img_tiled"));
    if (jerry_value_is_boolean(bg_img_tiled_val)) {
        draw_dsc.bg_img_tiled = jerry_value_is_true(bg_img_tiled_val);
    }
    jerry_value_free(bg_img_tiled_val);

    jerry_value_t border_post_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("border_post"));
    if (jerry_value_is_boolean(border_post_val)) {
        draw_dsc.border_post = jerry_value_is_true(border_post_val);
    }
    jerry_value_free(border_post_val);

    // Draw the polygon
    lv_canvas_draw_polygon(canvas, points, point_cnt, &draw_dsc);

    // Free allocated memory
    free(points);

    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_draw_arc(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    if (args_count < 6 || !jerry_value_is_number(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2]) ||
        !jerry_value_is_number(args[3]) || !jerry_value_is_number(args[4]) ||
        !jerry_value_is_object(args[5])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (x, y, r, start_angle, end_angle, draw_dsc)");
    }

    // Retrieve the native canvas object
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    // Extract positional arguments
    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t r = (lv_coord_t)jerry_value_as_number(args[2]);
    int32_t start_angle = (int32_t)jerry_value_as_number(args[3]);
    int32_t end_angle = (int32_t)jerry_value_as_number(args[4]);

    // Extract and initialize the draw descriptor
    jerry_value_t draw_dsc_obj = args[5];
    lv_draw_arc_dsc_t draw_dsc;
    lv_draw_arc_dsc_init(&draw_dsc); // Initialize with default values

    // Assign draw descriptor properties
    SET_FIELD_IF_PRESENT("width", width, lv_coord_t);
    SET_FIELD_IF_PRESENT("opa", opa, lv_opa_t);
    SET_FIELD_IF_PRESENT("blend_mode", blend_mode, lv_blend_mode_t);

    // Handle color
    jerry_value_t color_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("color"));
    if (jerry_value_is_number(color_val)) {
        uint32_t color = (uint32_t)jerry_value_as_number(color_val);
        draw_dsc.color = jr_lvgl_color_to_color_t(color);
    }
    jerry_value_free(color_val);

    // Handle img_src (if provided as ArrayBuffer)
    jerry_value_t img_src_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("img_src"));
    if (jerry_value_is_arraybuffer(img_src_val)) {
        jerry_length_t buf_size = jerry_arraybuffer_size(img_src_val);
        void *native_buf = malloc(buf_size);
        if (native_buf == NULL) {
            jerry_value_free(img_src_val);
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed for img_src");
        }
        jerry_arraybuffer_read(img_src_val, 0, native_buf, buf_size);
        draw_dsc.img_src = native_buf;
    }
    jerry_value_free(img_src_val);

    // Handle rounded flag
    jerry_value_t rounded_val = jerry_object_get(draw_dsc_obj, jerry_string_sz("rounded"));
    if (jerry_value_is_boolean(rounded_val)) {
        draw_dsc.rounded = jerry_value_is_true(rounded_val);
    }
    jerry_value_free(rounded_val);

    // Draw the arc
    lv_canvas_draw_arc(canvas, x, y, r, start_angle, end_angle, &draw_dsc);

    // Free allocated buffer for img_src (if used)
    if (draw_dsc.img_src) {
        free((void *)draw_dsc.img_src);
    }

    return jerry_undefined();
}

/************************************************************************
* Class register functions
*************************************************************************/

static void jr_lv_obj_class_register(jerry_external_handler_t constructor_handler) {

    jerryx_property_entry methods[] =
    {
        JERRYX_PROPERTY_FUNCTION ("align",       js_lv_obj_align),
        JERRYX_PROPERTY_FUNCTION ("setSize",     js_lv_obj_set_size),
        JERRYX_PROPERTY_FUNCTION ("onPress",     js_lv_obj_on_press),
        JERRYX_PROPERTY_FUNCTION ("setBuffer",   js_lv_obj_set_buffer),
        JERRYX_PROPERTY_FUNCTION ("setPxColor",  js_lv_canvas_set_px_color),
        JERRYX_PROPERTY_FUNCTION ("setPx",       js_lv_canvas_set_px_color),
        JERRYX_PROPERTY_FUNCTION ("setPxOpa",    js_lv_canvas_set_px_opa),
        JERRYX_PROPERTY_FUNCTION ("setPalette",  js_lv_canvas_set_palette),
        JERRYX_PROPERTY_FUNCTION ("getPx",       js_lv_canvas_get_px),
        JERRYX_PROPERTY_FUNCTION ("getImg",      js_lv_canvas_get_img),
        JERRYX_PROPERTY_FUNCTION ("copyBuf",     js_lv_canvas_copy_buf),
        JERRYX_PROPERTY_FUNCTION ("transform",   js_lv_canvas_transform),
        JERRYX_PROPERTY_FUNCTION ("blurHor",     js_lv_canvas_blur_hor),
        JERRYX_PROPERTY_FUNCTION ("blurVer",     js_lv_canvas_blur_ver),
        JERRYX_PROPERTY_FUNCTION ("fillBg",      js_lv_canvas_fill_bg),
        JERRYX_PROPERTY_FUNCTION ("drawRect",    js_lv_canvas_draw_rect),
        JERRYX_PROPERTY_FUNCTION ("drawText",    js_lv_canvas_draw_text),
        JERRYX_PROPERTY_FUNCTION ("drawImg",     js_lv_canvas_draw_img),
        JERRYX_PROPERTY_FUNCTION ("drawLine",    js_lv_canvas_draw_line),
        JERRYX_PROPERTY_FUNCTION ("drawPolygon", js_lv_canvas_draw_polygon),
        JERRYX_PROPERTY_FUNCTION ("drawArc",     js_lv_canvas_draw_arc),
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

void jr_lv_canvas_init(void) {
    jr_lv_obj_class_register(js_lv_obj_constructor);
}
