#include <stdio.h>
#include "lvgl.h"
#include <math.h>

/* Function to draw the segment shape on a canvas */
static void draw_segment_on_canvas(lv_obj_t *canvas, int start_angle, int end_angle, int outer_radius, int inner_radius, lv_color_t color)
{
    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = color;
    line_dsc.width = 2; // Line thickness

    lv_point_t outer_points[end_angle - start_angle + 1];
    lv_point_t inner_points[end_angle - start_angle + 1];

    int center_x = 100;
    int center_y = 100;

    // Calculate the points for the outer and inner arcs
    int idx = 0;
    for (int angle = start_angle; angle <= end_angle; angle++)
    {
        float rad = angle * M_PI / 180.0f;
        outer_points[idx].x = center_x + outer_radius * cos(rad);
        outer_points[idx].y = center_y + outer_radius * sin(rad);

        inner_points[idx].x = center_x + inner_radius * cos(rad);
        inner_points[idx].y = center_y + inner_radius * sin(rad);

        idx++;
    }

    // Draw the outer arc
    for (int i = 0; i < idx - 1; i++)
    {
        lv_point_t points[] = {outer_points[i], outer_points[i + 1]};
        lv_canvas_draw_line(canvas, points, 2, &line_dsc);
    }

    // Draw the inner arc
    for (int i = 0; i < idx - 1; i++)
    {
        lv_point_t points[] = {inner_points[i], inner_points[i + 1]};
        lv_canvas_draw_line(canvas, points, 2, &line_dsc);
    }

    // Connect the outer and inner arcs to close the shape
    lv_point_t connect1[] = {outer_points[0], inner_points[0]};
    lv_canvas_draw_line(canvas, connect1, 2, &line_dsc);

    lv_point_t connect2[] = {outer_points[idx - 1], inner_points[idx - 1]};
    lv_canvas_draw_line(canvas, connect2, 2, &line_dsc);
}

/* Function to create the launcher */
void create_launcher(lv_obj_t *parent)
{    
    // Create a canvas for drawing the segment
    static lv_color_t buf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(200, 200)];
    lv_obj_t *canvas = lv_canvas_create(parent);
    lv_obj_set_size(canvas, 200, 200);
    lv_obj_align(canvas, LV_ALIGN_CENTER, 0, 0);
    lv_canvas_set_buffer(canvas, buf, 200, 200, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(canvas, lv_color_make(255, 255, 255), LV_OPA_TRANSP);

    draw_segment_on_canvas(canvas, 0,   60,  90, 20, lv_color_make(255, 0, 0));
    draw_segment_on_canvas(canvas, 60,  120, 90, 20, lv_color_make(255, 0, 0));
    draw_segment_on_canvas(canvas, 120, 180, 90, 20, lv_color_make(255, 0, 0));
    draw_segment_on_canvas(canvas, 180, 240, 90, 20, lv_color_make(255, 0, 0));
    draw_segment_on_canvas(canvas, 240, 300, 90, 20, lv_color_make(255, 0, 0));
}

/* Execute the launcher on the active screen */
void execute_launcher(void)
{
    lv_obj_t *scr = lv_scr_act(); // Get the active screen
    create_launcher(scr);         // Create the launcher on the active screen
}