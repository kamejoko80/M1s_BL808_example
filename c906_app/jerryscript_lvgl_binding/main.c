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
#if 0
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "const label = lv_label_create(screen);\n"
        "const button = lv_btn_create(screen);\n"
        "lv_label_set_text(label, 'Hello, JerryScript!');\n"
        "lv_obj_align(label, 2, 0, 0);\n"
        "lv_obj_set_size(button, 100, 50);\n"
        "lv_obj_align(button, 2, 0, 20);\n"
        "print('Label created and displayed.');";
#else
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
        "myButton.setSize(100, 50);\n"
        "myButton.onPress(() => {\n"
        "print('Button pressed!');\n"
        "});\n"
        "print('Button created and displayed.');";        
#endif

    const jerry_length_t script_size = sizeof (script) - 1;

    /* Initialize engine */
    jerry_init(JERRY_INIT_EMPTY);

    /* Register the adder function in the global object */
    jr_gbl_const_init();
    jr_gbl_func_init();    
    jr_lv_label_init();
    jr_lv_btn_init();

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

    /* Cleanup engine */
    jerry_cleanup();
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