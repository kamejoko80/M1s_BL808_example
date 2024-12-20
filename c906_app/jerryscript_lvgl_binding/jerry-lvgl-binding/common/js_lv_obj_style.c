#include <stdio.h>
#include "js_lv_obj_style.h"

static const js_lv_style_property_map_t style_properties[] = {
    /* const char *name     const char *arg_type                (*lv_style_func_v)                      (*lv_style_func_c)              (*lv_style_func_p) */
    {"width",              "lv_coord_t",                        lv_obj_set_style_width,                 NULL,                            NULL                             },
    {"min_width",          "lv_coord_t",                        lv_obj_set_style_min_width,             NULL,                            NULL                             },
    {"max_width",          "lv_coord_t",                        lv_obj_set_style_max_width,             NULL,                            NULL                             },
    {"height",             "lv_coord_t",                        lv_obj_set_style_height,                NULL,                            NULL                             },
    {"min_height",         "lv_coord_t",                        lv_obj_set_style_min_height,            NULL,                            NULL                             },
    {"max_height",         "lv_coord_t",                        lv_obj_set_style_max_height,            NULL,                            NULL                             },
    {"x",                  "lv_coord_t",                        lv_obj_set_style_x,                     NULL,                            NULL                             },
    {"y",                  "lv_coord_t",                        lv_obj_set_style_y,                     NULL,                            NULL                             },
    {"align",              "lv_align_t",                        lv_obj_set_style_align,                 NULL,                            NULL                             },
    {"transform_width",    "lv_coord_t",                        lv_obj_set_style_transform_width,       NULL,                            NULL                             },
    {"transform_height",   "lv_coord_t",                        lv_obj_set_style_transform_height,      NULL,                            NULL                             },
    {"translate_x",        "lv_coord_t",                        lv_obj_set_style_translate_x,           NULL,                            NULL                             },
    {"translate_y",        "lv_coord_t",                        lv_obj_set_style_translate_y,           NULL,                            NULL                             },
    {"transform_zoom",     "lv_coord_t",                        lv_obj_set_style_transform_zoom,        NULL,                            NULL                             },
    {"transform_angle",    "lv_coord_t",                        lv_obj_set_style_transform_angle,       NULL,                            NULL                             },
    {"transform_pivot_x",  "lv_coord_t",                        lv_obj_set_style_transform_pivot_x,     NULL,                            NULL                             },
    {"transform_pivot_y",  "lv_coord_t",                        lv_obj_set_style_transform_pivot_y,     NULL,                            NULL                             },
    {"pad_top",            "lv_coord_t",                        lv_obj_set_style_pad_top,               NULL,                            NULL                             },
    {"pad_bottom",         "lv_coord_t",                        lv_obj_set_style_pad_bottom,            NULL,                            NULL                             },
    {"pad_left",           "lv_coord_t",                        lv_obj_set_style_pad_left,              NULL,                            NULL                             },
    {"pad_right",          "lv_coord_t",                        lv_obj_set_style_pad_right,             NULL,                            NULL                             },
    {"pad_row",            "lv_coord_t",                        lv_obj_set_style_pad_row,               NULL,                            NULL                             },
    {"pad_column",         "lv_coord_t",                        lv_obj_set_style_pad_column,            NULL,                            NULL                             },
    {"bg_color",           "lv_color_t",                        NULL,                                   lv_obj_set_style_bg_color,       NULL                             },
    {"bg_opa",             "lv_opa_t",                          lv_obj_set_style_bg_opa,                NULL,                            NULL                             },
    {"bg_grad_color",      "lv_color_t",                        NULL,                                   lv_obj_set_style_bg_grad_color,  NULL                             },
    {"bg_grad_dir",        "lv_grad_dir_t",                     lv_obj_set_style_bg_grad_dir,           NULL,                            NULL                             },
    {"bg_main_stop",       "lv_coord_t",                        lv_obj_set_style_bg_main_stop,          NULL,                            NULL                             },
    {"bg_grad_stop",       "lv_coord_t",                        lv_obj_set_style_bg_grad_stop,          NULL,                            NULL                             },
    {"bg_grad",            "const lv_grad_dsc_t *",             NULL,                                   NULL,                            lv_obj_set_style_bg_grad         },
    {"bg_dither_mode",     "lv_dither_mode_t",                  lv_obj_set_style_bg_dither_mode,        NULL,                            NULL                             },
    {"bg_img_src",         "const void *",                      NULL,                                   NULL,                            lv_obj_set_style_bg_img_src      },
    {"bg_img_opa",         "lv_opa_t",                          lv_obj_set_style_bg_img_opa,            NULL,                            NULL                             },
    {"bg_img_recolor",     "lv_color_t",                        NULL,                                   lv_obj_set_style_bg_img_recolor, NULL                             },
    {"bg_img_recolor_opa", "lv_opa_t",                          lv_obj_set_style_bg_img_recolor_opa,    NULL,                            NULL                             },
    {"bg_img_tiled",       "bool",                              lv_obj_set_style_bg_img_tiled,          NULL,                            NULL                             },
    {"border_color",       "lv_color_t",                        NULL,                                   lv_obj_set_style_border_color,   NULL                             },
    {"border_opa",         "lv_opa_t",                          lv_obj_set_style_border_opa,            NULL,                            NULL                             },
    {"border_width",       "lv_coord_t",                        lv_obj_set_style_border_width,          NULL,                            NULL                             },
    {"border_side",        "lv_border_side_t",                  lv_obj_set_style_border_side,           NULL,                            NULL                             },
    {"border_post",        "bool",                              lv_obj_set_style_border_post,           NULL,                            NULL                             },
    {"outline_width",      "lv_coord_t",                        lv_obj_set_style_outline_width,         NULL,                            NULL                             },
    {"outline_color",      "lv_color_t",                        NULL,                                   lv_obj_set_style_outline_color,  NULL                             },
    {"outline_opa",        "lv_opa_t",                          lv_obj_set_style_outline_opa,           NULL,                            NULL                             },
    {"outline_pad",        "lv_coord_t",                        lv_obj_set_style_outline_pad,           NULL,                            NULL                             },
    {"shadow_width",       "lv_coord_t",                        lv_obj_set_style_shadow_width,          NULL,                            NULL                             },
    {"shadow_ofs_x",       "lv_coord_t",                        lv_obj_set_style_shadow_ofs_x,          NULL,                            NULL                             },
    {"shadow_ofs_y",       "lv_coord_t",                        lv_obj_set_style_shadow_ofs_y,          NULL,                            NULL                             },
    {"shadow_spread",      "lv_coord_t",                        lv_obj_set_style_shadow_spread,         NULL,                            NULL                             },
    {"shadow_color",       "lv_color_t",                        NULL,                                   lv_obj_set_style_shadow_color,   NULL                             },
    {"shadow_opa",         "lv_opa_t",                          lv_obj_set_style_shadow_opa,            NULL,                            NULL                             },
    {"img_opa",            "lv_opa_t",                          lv_obj_set_style_img_opa,               NULL,                            NULL                             },
    {"img_recolor",        "lv_color_t",                        NULL,                                   lv_obj_set_style_img_recolor,    NULL                             },
    {"img_recolor_opa",    "lv_opa_t",                          lv_obj_set_style_img_recolor_opa,       NULL,                            NULL                             },
    {"line_width",         "lv_coord_t",                        lv_obj_set_style_line_width,            NULL,                            NULL                             },
    {"line_dash_width",    "lv_coord_t",                        lv_obj_set_style_line_dash_width,       NULL,                            NULL                             },
    {"line_dash_gap",      "lv_coord_t",                        lv_obj_set_style_line_dash_gap,         NULL,                            NULL                             },
    {"line_rounded",       "bool",                              lv_obj_set_style_line_rounded,          NULL,                            NULL                             },
    {"line_color",         "lv_color_t",                        NULL,                                   lv_obj_set_style_line_color,     NULL                             },
    {"line_opa",           "lv_opa_t",                          lv_obj_set_style_line_opa,              NULL,                            NULL                             },
    {"arc_width",          "lv_coord_t",                        lv_obj_set_style_arc_width,             NULL,                            NULL                             },
    {"arc_rounded",        "bool",                              lv_obj_set_style_arc_rounded,           NULL,                            NULL                             },
    {"arc_color",          "lv_color_t",                        NULL,                                   lv_obj_set_style_arc_color,      NULL                             },
    {"arc_opa",            "lv_opa_t",                          lv_obj_set_style_arc_opa,               NULL,                            NULL                             },
    {"arc_img_src",        "const void *",                      NULL,                                   NULL,                            lv_obj_set_style_arc_img_src     },
    {"text_color",         "lv_color_t",                        NULL,                                   lv_obj_set_style_text_color,     NULL                             },
    {"text_opa",           "lv_opa_t",                          lv_obj_set_style_text_opa,              NULL,                            NULL                             },
    {"text_font",          "const lv_font_t *",                 NULL,                                   NULL,                            lv_obj_set_style_text_font       },
    {"text_letter_space",  "lv_coord_t",                        lv_obj_set_style_text_letter_space,     NULL,                            NULL                             },
    {"text_line_space",    "lv_coord_t",                        lv_obj_set_style_text_line_space,       NULL,                            NULL                             },
    {"text_decor",         "lv_text_decor_t",                   lv_obj_set_style_text_decor,            NULL,                            NULL                             },
    {"text_align",         "lv_text_align_t",                   lv_obj_set_style_text_align,            NULL,                            NULL                             },
    {"radius",             "lv_coord_t",                        lv_obj_set_style_radius,                NULL,                            NULL                             },
    {"clip_corner",        "bool",                              lv_obj_set_style_clip_corner,           NULL,                            NULL                             },
    {"opa",                "lv_opa_t",                          lv_obj_set_style_opa,                   NULL,                            NULL                             },
    {"color_filter_dsc",   "const lv_color_filter_dsc_t *",     NULL,                                   NULL,                            lv_obj_set_style_color_filter_dsc},
    {"color_filter_opa",   "lv_opa_t",                          lv_obj_set_style_color_filter_opa,      NULL,                            NULL                             },
    {"anim",               "const lv_anim_t *",                 NULL,                                   NULL,                            lv_obj_set_style_anim            },
    {"anim_time",          "uint32_t",                          lv_obj_set_style_anim_time,             NULL,                            NULL                             },
    {"anim_speed",         "uint32_t",                          lv_obj_set_style_anim_speed,            NULL,                            NULL                             },
    {"transition",         "const lv_style_transition_dsc_t *", NULL,                                   NULL,                            lv_obj_set_style_transition      },
    {"blend_mode",         "lv_blend_mode_t",                   lv_obj_set_style_blend_mode,            NULL,                            NULL                             },
    {"layout",             "uint16_t",                          lv_obj_set_style_layout,                NULL,                            NULL                             },
    {"base_dir",           "lv_base_dir_t",                     lv_obj_set_style_base_dir,              NULL,                            NULL                             },
    {NULL, NULL, NULL, NULL}
};

static const js_lv_style_property_map_t *js_lv_find_style_property(const char *name) {
    for (const js_lv_style_property_map_t *entry = style_properties; entry->name != NULL; ++entry) {
        if (strcmp(entry->name, name) == 0) {
            return entry;
        }
    }
    return NULL;
}

jerry_value_t js_lv_set_style(lv_obj_t *obj, const jerry_value_t args[], const jerry_length_t args_count) {

    jerry_size_t prop_size = jerry_string_size(args[0], JERRY_ENCODING_UTF8);
    char *property = malloc(prop_size + 1);
    if (!property) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
    }
    jerry_string_to_buffer(args[0], JERRY_ENCODING_UTF8, (jerry_char_t *)property, prop_size);
    property[prop_size] = '\0';

    const js_lv_style_property_map_t *map_entry = js_lv_find_style_property(property);
    free(property);

    if (!map_entry) {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Unknown style property");
    }

    lv_style_selector_t selector = (lv_style_selector_t)jerry_value_as_number(args[2]);

    if (strcmp(map_entry->arg_type, "lv_coord_t") == 0) {
        lv_coord_t value = (lv_coord_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, value, selector);
    } else if (strcmp(map_entry->arg_type, "lv_color_t") == 0) {
        uint32_t color_value = (uint32_t)jerry_value_as_number(args[1]);
        lv_color_t color = lv_color_make((color_value >> 16) & 0xFF, (color_value >> 8) & 0xFF, color_value & 0xFF);
        map_entry->lv_style_func_c(obj, color, selector);
    } else if (strcmp(map_entry->arg_type, "lv_opa_t") == 0) {
        lv_opa_t opa = (lv_opa_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, opa, selector);
    } else if (strcmp(map_entry->arg_type, "lv_align_t") == 0) {
        lv_align_t align = (lv_align_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, align, selector);
    } else if (strcmp(map_entry->arg_type, "lv_grad_dir_t") == 0) {
        lv_grad_dir_t grad_dir = (lv_grad_dir_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, grad_dir, selector);
    } else if (strcmp(map_entry->arg_type, "lv_dither_mode_t") == 0) {
        lv_dither_mode_t dither_mode = (lv_dither_mode_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, dither_mode, selector);
    } else if (strcmp(map_entry->arg_type, "lv_base_dir_t") == 0) {
        lv_base_dir_t base_dir = (lv_base_dir_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, base_dir, selector);
    } else if (strcmp(map_entry->arg_type, "lv_blend_mode_t") == 0) {
        lv_blend_mode_t blend_mode = (lv_blend_mode_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, blend_mode, selector);
    } else if (strcmp(map_entry->arg_type, "bool") == 0) {
        bool value = jerry_value_is_true(args[1]);
        map_entry->lv_style_func_v(obj, value, selector);
    } else if (strcmp(map_entry->arg_type, "uint16_t") == 0) {
        uint16_t value = (uint16_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, value, selector);
    } else if (strcmp(map_entry->arg_type, "uint32_t") == 0) {
        uint32_t value = (uint32_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_v(obj, value, selector);
    } else if (strcmp(map_entry->arg_type, "const void *") == 0) {
        jerry_size_t src_size = jerry_string_size(args[1], JERRY_ENCODING_UTF8);
        char *src = malloc(src_size + 1);
        if (!src) {
            return jerry_throw_sz(JERRY_ERROR_TYPE, "Memory allocation failed");
        }
        jerry_string_to_buffer(args[1], JERRY_ENCODING_UTF8, (jerry_char_t *)src, src_size);
        src[src_size] = '\0';
        map_entry->lv_style_func_p(obj, src, selector);
        free(src);
    } else if (strcmp(map_entry->arg_type, "const lv_grad_dsc_t *") == 0 ||
               strcmp(map_entry->arg_type, "const lv_font_t *") == 0 ||
               strcmp(map_entry->arg_type, "const lv_color_filter_dsc_t *") == 0 ||
               strcmp(map_entry->arg_type, "const lv_anim_t *") == 0 ||
               strcmp(map_entry->arg_type, "const lv_style_transition_dsc_t *") == 0) {
        void *value = (void *)(uintptr_t)jerry_value_as_number(args[1]);
        map_entry->lv_style_func_p(obj, value, selector);
    } else {
        return jerry_throw_sz(JERRY_ERROR_TYPE, "Unsupported argument type");
    }

    return jerry_undefined();
}