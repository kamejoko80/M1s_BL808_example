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
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "const myLabel = new Label(screen);\n"
        "myLabel.align(LV_ALIGN_CENTER, 0, 0);\n"
        "myLabel.setText('Hello LVGL!');\n"
        "print(myLabel.getText());\n"
        "print('Label created and displayed.');"
        "const myButton = new Button(screen);\n"
        "myButton.align(LV_ALIGN_BOTTOM_MID, 0, 0);\n"
        "myButton.setSize(60, 60);\n"
        "myButton.onPress(function(event_code){\n"
        "if (event_code === 7) {\n"
        "print('Button clicked!');\n"
        "}});\n"
        "let myArc = new Arc(screen);\n"
        "myButton.setSize(100, 100);\n"
        "myArc.align(LV_ALIGN_TOP_MID, 0, 0);\n"
        "myArc.setRange(0, 100);\n"
        "myArc.setStartAngle(45);\n"
        "myArc.setEndAngle(270);\n"
        "myArc.setBgStartAngle(0);\n"
        "myArc.setBgEndAngle(360);\n"
        "myArc.setAngles(0, 180);\n"        
        "myArc.setRotation(90);\n"
        "myArc.setMode(LV_ARC_MODE_REVERSE);\n"
        "myArc.setValue(50);\n"
        "myArc.alignObjToAngle(myLabel, 50);\n"
        "print('Button created and displayed.');";

    const jerry_length_t script_size = sizeof (script) - 1;

    /* Initialize engine */
    jerry_init(JERRY_INIT_EMPTY);

    /* Register the adder function in the global object */
    jr_gbl_const_init();
    jr_gbl_func_init();
    jr_lv_label_init();
    jr_lv_btn_init();
    jr_lv_arc_init();

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

void main()
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    printf("Execute jerryscript demo\r\n");

    jerryscript_lvgl_demo();

    lv_task_handler();
    xTaskCreate(lvgl_task, (char *)"lvgl task", 512, NULL, 15, NULL);
}