#include <stdio.h>
#include <string.h>

/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>

/* aos */
#include <cli.h>

/* fs */
#include <bl_romfs.h>

/* RISCV */
#include <csi_core.h>

/* LVGL */
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"

/* Jerryscript */
#include "jerryscript.h"
#include "jerryscript-macro.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

#include "jr_lvgl.h"
#include "js_gbl_const.h"
#include "js_gbl_func.h"
#include "js_lv_label.h"
#include "js_lv_btn.h"
#include "js_lv_arc.h"
#include "js_lv_bar.h"
#include "js_lv_btnmatrix.h"
#include "js_lv_canvas.h"

static void lvgl_task(void *param)
{
    while (1) {
        lv_task_handler();
        vTaskDelay(1);
    }
    vTaskDelete(NULL);
}

void jerryscript_lvgl_demo(void)
{

#if 0 /* Test lable */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "const myLabel = new Label(screen);\n"
        "myLabel.align(LV_ALIGN_CENTER, 0, 0);\n"
        "myLabel.setText('Hello LVGL!');\n"
        "print(myLabel.getText());\n";
#endif

#if 0 /* Test button */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let myButton = new Button(screen);\n"
        "myButton.align(LV_ALIGN_CENTER, 0, 0);\n"
        "myButton.setSize(60, 60);\n"
        "myButton.onPress(function(event_code){\n"
        "if (event_code === 7) {\n"
        "print('Button clicked!');\n"
        "}});\n";
#endif

#if 0 /* Test arc */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let myArc = new Arc(screen);\n"
        "myArc.setSize(200, 200);\n"
        "myArc.align(LV_ALIGN_CENTER, 0, 0);\n"
        "myArc.setRange(0, 100);\n"
        "myArc.setStartAngle(45);\n"
        "myArc.setEndAngle(270);\n"
        "myArc.setBgStartAngle(0);\n"
        "myArc.setBgEndAngle(360);\n"
        "myArc.setAngles(0, 180);\n"
        "myArc.setRotation(90);\n"
        "myArc.setMode(LV_ARC_MODE_REVERSE);\n"
        "myArc.setValue(50);\n"
        "myArc.alignObjToAngle(myLabel, 50);\n";
#endif

#if 0 /* Test bar */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let myBar = new Bar(screen);\n"
        "myBar.setSize(150, 20);\n"
        "myBar.align(LV_ALIGN_CENTER, 0, 0);\n"
        "myBar.setValue(50, ANIM_ON);";
#endif

#if 0 /* Test button matrix */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "const btn_map = [\n"
           "'1', '2', '3','\\n',\n"
           "'4', '5', '6','\\n',\n"
           "'7', '8', '9','\\n',\n"
           "'*', '0', '#',''\n"
        "];\n"
        "let btnm = new BtnMatrix(screen);\n"
        "btnm.setMap(btn_map);\n"
        "btnm.setSize(250, 170);\n"
        "btnm.align(LV_ALIGN_CENTER, 0, 0);"
        "btnm.onChanged(function(e){\n"
            "const btn_id = btnm.getSelectedBtn();\n"
            "const txt = btnm.getBtnText(btn_id);\n"
            "print('Button ' + txt + ' pressed');\n"
        "});\n"
        "const btnMapArray = btnm.getMap();\n"
        "if (Array.isArray(btnMapArray)) {\n"
            "for (let i = 0; i < btnMapArray.length; i++) {\n"
                "print(`Button ${i}: ${btnMapArray[i]}`);\n"
            "}\n"
        "}\n";    
#endif

#if 1 /* Test canvas */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let cv = new Canvas(screen);\n"
        "cv.setSize(200, 200);\n"
        "cv.align(LV_ALIGN_CENTER, 0, 0);\n"
        "cv.setBuffer(200, 200, LV_IMG_CF_TRUE_COLOR);\n"
        "cv.fillBg(lv_color_make(0, 255, 0), LV_OPA_COVER);\n";
#endif

    const jerry_length_t script_size = sizeof (script) - 1;

    /* Initialize engine */
    jerry_init(JERRY_INIT_EMPTY);

    /* Register the adder function in the global object */
    jr_gbl_const_init();
    jr_gbl_func_init();
    jr_lv_label_init();
    jr_lv_btn_init();
    jr_lv_arc_init();
    jr_lv_bar_init();
    jr_lv_btnmatrix_init();
    jr_lv_canvas_init();

    /* Register the print function in the global object */
    jerryx_register_global("print", jerryx_handler_print);

    /* Setup Global scope code */
    jerry_value_t parsed_code = jerry_parse(script, script_size, NULL);

    if (!jerry_value_is_exception (parsed_code))
    {
        /* Execute the parsed source code in the Global scope */
        jerry_value_t ret_value = jerry_run(parsed_code);

        /* Returned value must be freed */
        jerry_value_free(ret_value);
    }

    /* Parsed source code must be freed */
    jerry_value_free(parsed_code);

    /* Cleanup engine this cause object destructor callback call */
    //jerry_cleanup();
}

/* Event callback function */
void btnm_event_cb(lv_event_t *e)
{
    lv_obj_t *btnm = lv_event_get_target(e);
    uint16_t btn_id = lv_btnmatrix_get_selected_btn(btnm);
    const char *txt = lv_btnmatrix_get_btn_text(btnm, btn_id);
    printf("Button %s pressed\n", txt);
}

#if 0
void create_button_matrix(void)
{
    static const char *btn_map[] = {
        "1", "2", "3", "\n",
        "4", "5", "6", "\n",
        "7", "8", "9", "\n",
        "*", "0", "#", ""
    };


    /* Create a button matrix */
    lv_obj_t *btnm = lv_btnmatrix_create(lv_scr_act());
    lv_btnmatrix_set_map(btnm, btn_map);
    lv_obj_set_size(btnm, 200, 150);
    lv_obj_center(btnm);

    /* Add an event callback */
    lv_obj_add_event_cb(btnm, btnm_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
}
#endif

void lvgl_canvas_demo(void)
{
    static lv_color_t buf[200 * 200];
    lv_obj_t *screen = lv_scr_act();
    lv_obj_t *cv = lv_canvas_create(screen);
    lv_obj_set_size(cv, 200, 200);
    lv_obj_align(cv, LV_ALIGN_CENTER, 0, 0);
    lv_canvas_set_buffer(cv, buf, 200, 200, LV_IMG_CF_TRUE_COLOR);
    lv_canvas_fill_bg(cv, lv_color_make(0, 0, 255), LV_OPA_COVER);
    lv_color_t color = lv_color_make(0, 0, 255);
#if LV_COLOR_DEPTH == 32
    printf("color depth 32\n");
    printf("color value: %x\n", color.full); // Use `.full` for 32-bit colors (ARGB8888)
#elif LV_COLOR_DEPTH == 16
    printf("color depth 16\n");
    printf("color r:  %x\n", color.ch.red);
    printf("color gh: %x\n", color.ch.green_h);
    printf("color gl: %x\n", color.ch.green_l);    
    printf("color b:  %x\n", color.ch.blue);
    printf("color value: %x\n", color.full); // Use `.full` for 16-bit colors (RGB565)
#elif LV_COLOR_DEPTH == 8
    printf("color depth 8\n");
    printf("color value: %x\n", color.full); // Use `.full` for 8-bit colors
#endif    
}

void main()
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    printf("Execute jerryscript demo\r\n");

    jerryscript_lvgl_demo();
    //create_button_matrix();
    //lvgl_canvas_demo();

    lv_task_handler();
    xTaskCreate(lvgl_task, (char *)"lvgl task", 512, NULL, 15, NULL);
}