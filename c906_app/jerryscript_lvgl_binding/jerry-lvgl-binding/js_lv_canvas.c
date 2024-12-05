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
    if (args_count < 4 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1]) ||
        !jerry_value_is_number(args[2]) || !jerry_value_is_number(args[3])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (buf, w, h, cf)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, obj, call_info_p->this_value, &jerry_obj_native_info);
    if (obj == NULL) {
        return jerry_undefined();
    }

    jerry_value_t buf_arg = args[0];
    jerry_length_t buf_size = jerry_arraybuffer_size(buf_arg);

    void *buf = malloc(buf_size);
    if (buf == NULL) {
        printf("%s %s error buffer\n", LV_OBJ_NAME, __FUNCTION__);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }

    /* Read the ArrayBuffer content into the allocated memory */
    jerry_length_t written = jerry_arraybuffer_write(buf_arg, 0, buf, buf_size);
    if (written != buf_size) {
        printf("%s %s buffer written failed\n", LV_OBJ_NAME, __FUNCTION__);
        free(buf);
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Failed to read ArrayBuffer");
    }

    /* store buffer pointer into user data */
    jerry_lv_user_data_t *user_data = (jerry_lv_user_data_t *)lv_obj_get_user_data(obj);
    if(user_data != NULL){
        user_data->buf = buf;
    }

    lv_coord_t w = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t h = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_img_cf_t cf = (lv_img_cf_t)jerry_value_as_number(args[3]);

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

    lv_color_t color = LV_COLOR_MAKE(
        (uint8_t)((color_value >> 16) & 0xFF), // Red component
        (uint8_t)((color_value >> 8) & 0xFF),  // Green component
        (uint8_t)(color_value & 0xFF)         // Blue component
    );

    lv_canvas_set_px_color(canvas, x, y, color);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_set_px(const jerry_call_info_t *call_info_p,
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

    lv_color_t color = LV_COLOR_MAKE(
        (uint8_t)((color_value >> 16) & 0xFF), // Red component
        (uint8_t)((color_value >> 8) & 0xFF),  // Green component
        (uint8_t)(color_value & 0xFF)         // Blue component
    );

    lv_canvas_set_px(canvas, x, y, color);
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

    lv_color_t color = LV_COLOR_MAKE(
        (uint8_t)((color_value >> 16) & 0xFF),
        (uint8_t)((color_value >> 8) & 0xFF),
        (uint8_t)(color_value & 0xFF)
    );

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

    uint8_t red = LV_COLOR_GET_R(color);
    uint8_t green = LV_COLOR_GET_G(color);
    uint8_t blue = LV_COLOR_GET_B(color);

    uint32_t color_value = (red << 16) | (green << 8) | blue;

    return jerry_number(color_value);
}

static jerry_value_t js_lv_canvas_get_img(const jerry_call_info_t *call_info_p,
                                          const jerry_value_t args[],
                                          const jerry_length_t args_count) {
    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_img_dsc_t *img = lv_canvas_get_img(canvas);
    if (img == NULL) {
        return jerry_undefined();
    }

    jerry_value_t js_img = jerry_object();
    jerry_object_set_native_ptr(js_img, &jerry_obj_native_info, img);
    return js_img;
}

static jerry_value_t js_lv_canvas_copy_buf(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    if (args_count < 5 || !jerry_value_is_object(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_number(args[2]) ||
        !jerry_value_is_number(args[3]) || !jerry_value_is_number(args[4])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (to_copy, x, y, w, h)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    JERRY_GET_NATIVE_PTR(void, to_copy, args[0], &jerry_obj_native_info);
    if (to_copy == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t w = (lv_coord_t)jerry_value_as_number(args[3]);
    lv_coord_t h = (lv_coord_t)jerry_value_as_number(args[4]);

    lv_canvas_copy_buf(canvas, to_copy, x, y, w, h);
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

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    JERRY_GET_NATIVE_PTR(lv_img_dsc_t, img, args[0], &jerry_obj_native_info);
    if (img == NULL) {
        return jerry_undefined();
    }

    int16_t angle = (int16_t)jerry_value_as_number(args[1]);
    uint16_t zoom = (uint16_t)jerry_value_as_number(args[2]);
    lv_coord_t offset_x = (lv_coord_t)jerry_value_as_number(args[3]);
    lv_coord_t offset_y = (lv_coord_t)jerry_value_as_number(args[4]);
    int32_t pivot_x = (int32_t)jerry_value_as_number(args[5]);
    int32_t pivot_y = (int32_t)jerry_value_as_number(args[6]);
    bool antialias = jerry_value_is_true(args[7]);

    lv_canvas_transform(canvas, img, angle, zoom, offset_x, offset_y, pivot_x, pivot_y, antialias);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_blur_hor(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (area, r)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    JERRY_GET_NATIVE_PTR(lv_area_t, area, args[0], &jerry_obj_native_info);
    if (area == NULL) {
        return jerry_undefined();
    }

    uint16_t r = (uint16_t)jerry_value_as_number(args[1]);
    lv_canvas_blur_hor(canvas, area, r);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_blur_ver(const jerry_call_info_t *call_info_p,
                                           const jerry_value_t args[],
                                           const jerry_length_t args_count) {
    if (args_count < 2 || !jerry_value_is_object(args[0]) || !jerry_value_is_number(args[1])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (area, r)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    JERRY_GET_NATIVE_PTR(lv_area_t, area, args[0], &jerry_obj_native_info);
    if (area == NULL) {
        return jerry_undefined();
    }

    uint16_t r = (uint16_t)jerry_value_as_number(args[1]);
    lv_canvas_blur_ver(canvas, area, r);
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

    uint32_t color_value = (uint32_t)jerry_value_as_number(args[0]);
    lv_color_t color = LV_COLOR_MAKE(
        (uint8_t)((color_value >> 16) & 0xFF),
        (uint8_t)((color_value >> 8) & 0xFF),
        (uint8_t)(color_value & 0xFF)
    );

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

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[2]);
    lv_coord_t w = (lv_coord_t)jerry_value_as_number(args[3]);
    lv_coord_t h = (lv_coord_t)jerry_value_as_number(args[4]);

    JERRY_GET_NATIVE_PTR(lv_draw_rect_dsc_t, draw_dsc, args[4], &jerry_obj_native_info);
    if (draw_dsc == NULL) {
        return jerry_undefined();
    }

    lv_canvas_draw_rect(canvas, x, y, w, h, draw_dsc);
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

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t max_w = (lv_coord_t)jerry_value_as_number(args[2]);

    JERRY_GET_NATIVE_PTR(lv_draw_label_dsc_t, draw_dsc, args[3], &jerry_obj_native_info);
    if (draw_dsc == NULL) {
        return jerry_undefined();
    }

    jerry_size_t txt_len = jerry_string_size(args[4], JERRY_ENCODING_UTF8);
    char *txt = malloc(txt_len + 1);
    if(txt == NULL) {
        return jerry_undefined();
    }
    jerry_string_to_buffer(args[4], JERRY_ENCODING_UTF8, (jerry_char_t *)txt, txt_len);
    txt[txt_len] = '\0';

    lv_canvas_draw_text(canvas, x, y, max_w, draw_dsc, txt);
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

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);

    JERRY_GET_NATIVE_PTR(void, src, args[2], &jerry_obj_native_info);
    if (src == NULL) {
        return jerry_undefined();
    }

    JERRY_GET_NATIVE_PTR(lv_draw_img_dsc_t, draw_dsc, args[3], &jerry_obj_native_info);
    if (src == NULL) {
        return jerry_undefined();
    }

    lv_canvas_draw_img(canvas, x, y, src, draw_dsc);
    return jerry_undefined();
}

static jerry_value_t js_lv_canvas_draw_line(const jerry_call_info_t *call_info_p,
                                            const jerry_value_t args[],
                                            const jerry_length_t args_count) {
    if (args_count < 3 || !jerry_value_is_array(args[0]) ||
        !jerry_value_is_number(args[1]) || !jerry_value_is_object(args[2])) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Expected (points, point_cnt, draw_dsc)");
    }

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    uint32_t point_cnt = (uint32_t)jerry_value_as_number(args[1]);

    JERRY_GET_NATIVE_PTR(lv_draw_line_dsc_t, draw_dsc, args[2], &jerry_obj_native_info);
    if (draw_dsc == NULL) {
        return jerry_undefined();
    }

    lv_point_t *points = malloc(sizeof(lv_point_t) * point_cnt);
    if (points == NULL) {
        return jerry_undefined();
    }

    for (uint32_t i = 0; i < point_cnt; i++) {
        jerry_value_t index = jerry_number(i);
        jerry_value_t point_obj = jerry_object_get(args[0], index);
        jerry_value_free(index);

        if (!jerry_value_is_object(point_obj)) {
            free(points);
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid point format");
        }

        jerry_value_t x_key = jerry_string_sz("x");
        jerry_value_t y_key = jerry_string_sz("y");
        jerry_value_t x_val = jerry_object_get(point_obj, x_key);
        jerry_value_t y_val = jerry_object_get(point_obj, y_key);
        jerry_value_free(x_key);
        jerry_value_free(y_key);

        points[i].x = (lv_coord_t)jerry_value_as_number(x_val);
        points[i].y = (lv_coord_t)jerry_value_as_number(y_val);

        jerry_value_free(x_val);
        jerry_value_free(y_val);
        jerry_value_free(point_obj);
    }

    lv_canvas_draw_line(canvas, points, point_cnt, draw_dsc);
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

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    uint32_t point_cnt = (uint32_t)jerry_value_as_number(args[1]);

    JERRY_GET_NATIVE_PTR(lv_draw_rect_dsc_t, draw_dsc, args[2], &jerry_obj_native_info);
    if (draw_dsc == NULL) {
        return jerry_undefined();
    }

    lv_point_t *points = malloc(sizeof(lv_point_t) * point_cnt);
    if (points == NULL) {
        return jerry_undefined();
    }

    for (uint32_t i = 0; i < point_cnt; i++) {
        jerry_value_t index = jerry_number(i);
        jerry_value_t point_obj = jerry_object_get(args[0], index);
        jerry_value_free(index);

        if (!jerry_value_is_object(point_obj)) {
            free(points);
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Invalid point format");
        }

        jerry_value_t x_key = jerry_string_sz("x");
        jerry_value_t y_key = jerry_string_sz("y");
        jerry_value_t x_val = jerry_object_get(point_obj, x_key);
        jerry_value_t y_val = jerry_object_get(point_obj, y_key);
        jerry_value_free(x_key);
        jerry_value_free(y_key);

        points[i].x = (lv_coord_t)jerry_value_as_number(x_val);
        points[i].y = (lv_coord_t)jerry_value_as_number(y_val);

        jerry_value_free(x_val);
        jerry_value_free(y_val);
        jerry_value_free(point_obj);
    }

    lv_canvas_draw_polygon(canvas, points, point_cnt, draw_dsc);
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

    JERRY_GET_NATIVE_PTR(lv_obj_t, canvas, call_info_p->this_value, &jerry_obj_native_info);
    if (canvas == NULL) {
        return jerry_undefined();
    }

    lv_coord_t x = (lv_coord_t)jerry_value_as_number(args[0]);
    lv_coord_t y = (lv_coord_t)jerry_value_as_number(args[1]);
    lv_coord_t r = (lv_coord_t)jerry_value_as_number(args[2]);
    int32_t start_angle = (int32_t)jerry_value_as_number(args[3]);
    int32_t end_angle = (int32_t)jerry_value_as_number(args[4]);

    JERRY_GET_NATIVE_PTR(lv_draw_arc_dsc_t, draw_dsc, args[5], &jerry_obj_native_info);
    if (draw_dsc == NULL) {
        return jerry_undefined();
    }

    lv_canvas_draw_arc(canvas, x, y, r, start_angle, end_angle, draw_dsc);
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
        JERRYX_PROPERTY_FUNCTION ("setPx",       js_lv_canvas_set_px),
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
