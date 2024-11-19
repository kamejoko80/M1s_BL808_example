#include <stdbool.h>
#include <stdio.h>

/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>

/* bl808 c906 std driver */
#include <bl808_glb.h>

#include "demos/lv_demos.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lvgl.h"

#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"

lv_ui guider_ui;

static void lvgl_task(void *param)
{
    while (1) {
        lv_task_handler();
        vTaskDelay(1);
    }
    vTaskDelete(NULL);
}

void main()
{
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    //lv_demo_benchmark();    
    /*Create a GUI-Guider app */
	setup_ui(&guider_ui);
    events_init(&guider_ui);    
    custom_init(&guider_ui);
    
    lv_task_handler();

    xTaskCreate(lvgl_task, (char *)"lvgl task", 512, NULL, 15, NULL);
}