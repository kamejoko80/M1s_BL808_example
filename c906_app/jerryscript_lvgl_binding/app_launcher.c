#include <stdio.h>
#include "lvgl.h"
#include <math.h>

/* Global variable to track the selected container */
static lv_obj_t *selected_icon = NULL;

/* Event callback for icon press */
static void app_icon_event_cb(lv_event_t *e) {
    lv_obj_t *cont = lv_event_get_target(e);

    /* Remove highlight from the previously selected icon */
    if (selected_icon) {
        lv_obj_clear_state(selected_icon, LV_STATE_CHECKED);
        selected_icon = NULL;
    }

    /* Highlight the currently selected icon */
    selected_icon = cont;
    lv_obj_add_state(cont, LV_STATE_CHECKED);

    LV_LOG_USER("App icon selected!");
}

/* Function to create a single app icon */
static lv_obj_t *create_app_icon(lv_obj_t *parent, const void *icon_src, const char *label_text) {
    /* Create a container for the icon and label */
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 70, 80); // Icon container size
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* Add the app icon */
    lv_obj_t *icon = lv_img_create(cont);
    lv_img_set_src(icon, icon_src);
    lv_obj_set_style_pad_all(icon, 0, 0);

    /* Add the app label */
    lv_obj_t *label = lv_label_create(cont);
    lv_label_set_text(label, label_text);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);

    /* Add a futuristic neon style to the container */
    static lv_style_t style_default, style_checked, style_pressed;
    static bool style_initialized = false;

    if (!style_initialized) {
        /* Default style with neon glow */
        lv_style_init(&style_default);
        lv_style_set_radius(&style_default, 10);
        lv_style_set_border_width(&style_default, 4);
        lv_style_set_border_color(&style_default, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_shadow_color(&style_default, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_shadow_width(&style_default, 10); // Neon glow effect
        lv_style_set_shadow_spread(&style_default, 2);

        /* Checked state style */
        lv_style_init(&style_checked);
        lv_style_set_bg_color(&style_checked, lv_palette_darken(LV_PALETTE_BLUE, 3));
        lv_style_set_border_color(&style_checked, lv_palette_main(LV_PALETTE_CYAN));

        /* Pressed state style */
        lv_style_init(&style_pressed);
        lv_style_set_bg_color(&style_pressed, lv_palette_darken(LV_PALETTE_BLUE, 1));

        style_initialized = true;
    }

    lv_obj_add_style(cont, &style_default, 0);              // Apply default style
    lv_obj_add_style(cont, &style_checked, LV_STATE_CHECKED); // Apply checked style
    lv_obj_add_style(cont, &style_pressed, LV_STATE_PRESSED); // Apply pressed style

    /* Add event callback to the container */
    lv_obj_add_event_cb(cont, app_icon_event_cb, LV_EVENT_CLICKED, NULL);

    return cont;
}

/* Function to create the app launcher */
void create_app_launcher(void) {
    /* Get the active screen */
    lv_obj_t *scr = lv_scr_act();

    /* Create a container for the app icons */
    lv_obj_t *grid = lv_obj_create(scr);
    lv_obj_set_style_bg_color(grid, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(grid, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_size(grid, LV_PCT(100), LV_PCT(100));
    lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(grid, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_pad_column(grid, 10, 0);
    lv_obj_set_style_pad_row(grid, 20, 0);

    /* Create app icons */
    create_app_icon(grid, LV_SYMBOL_HOME, "Home");
    create_app_icon(grid, LV_SYMBOL_SETTINGS, "Settings");
    create_app_icon(grid, LV_SYMBOL_FILE, "Files");
    create_app_icon(grid, LV_SYMBOL_WIFI, "Wi-Fi");
    create_app_icon(grid, LV_SYMBOL_BATTERY_FULL, "Battery");
    create_app_icon(grid, LV_SYMBOL_BELL, "Notifications");
}

/* Execute the launcher on the active screen */
void execute_launcher(void)
{
    create_app_launcher();
}
