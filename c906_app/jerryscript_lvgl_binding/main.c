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
#include "js_lv_checkbox.h"
#include "js_lv_dropdown.h"
#include "js_lv_img.h"
#include "js_lv_line.h"

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

#if 0 /* Test canvas */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let cv = new Canvas(screen);\n"
        "cv.setSize(200, 200);\n"
        "cv.align(LV_ALIGN_CENTER, 0, 0);\n"
        "cv.setBuffer(200, 200, LV_IMG_CF_TRUE_COLOR);\n"
        /********** fill BG demo ************/
        "cv.fillBg(lv_color_make(0, 255, 0), LV_OPA_COVER);\n"
        "for(let i = 0; i < 50; i++)\n"
        "for(let j = 0; j < 50; j++){\n"
            "cv.setPx(i, j, lv_color_make(255, 0, 0));\n"
        "}\n"
        /********** get img demo ************/
        "let img_dsc = cv.getImg();\n"
        "if (img_dsc.header) {\n"
            "print(`Header: ${JSON.stringify(img_dsc.header)}`);\n"
            "print(`Pixels: ${img_dsc.data.byteLength} bytes`);\n"
        "}else{\n"
            "print('There is no image');\n"
        "}\n"
        /********** copy buf demo ************/
        "const small_buf_size = 20 * 20 * 2;\n"
        "let small_buf = new ArrayBuffer(small_buf_size);\n"
        "let small_buf_view = new Uint16Array(small_buf);\n"
        "for (let i = 0; i < small_buf_view.length; i++){\n"
            "small_buf_view[i] = lv_color_make(0, 0, 255);\n"
        "}\n"
        "cv.copyBuf(small_buf, 100, 100, 20, 20);\n"
        /********** transform demo ************/
        "let img = {\n"
            "data: new ArrayBuffer(40 * 40 * 2),\n"
            "width: 40,\n"
            "height: 40,\n"
            "cf: LV_IMG_CF_TRUE_COLOR\n"
        "};\n"
        "let img_view = new Uint16Array(img.data);\n"
        "for (let i = 0; i < img_view.length; i++) {\n"
            "img_view[i] = lv_color_make(0xFF, 0, 0xEC);\n"
        "}"
        "cv.transform(\n"
            "img, // Image descriptor\n"
            "75,  // Angle in degrees\n"
            "256, // Zoom (1x = 256)\n"
            "50,  // Offset X\n"
            "50,  // Offset Y\n"
            "10,  // Pivot X\n"
            "10,  // Pivot Y\n"
            "true // Antialiasing\n"
        ");\n"
        /********** blur hor demo ************/
        "let blur_area = {\n"
            "x1: 50,\n"
            "y1: 50,\n"
            "x2: 150,\n"
            "y2: 150\n"
        "};\n"
        "cv.blurHor(blur_area, 10);\n"
        /********** draw rect demo ************/
        "let draw_rect_dsc = {\n"
            "radius: 10,\n"
            "bg_color: 0x00FF00,     // Green\n"
            "border_color: 0xFF0000, // Red\n"
            "border_width: 5,\n"
            "shadow_color: 0x0000FF, // Blue\n"
            "shadow_width: 10,\n"
            "shadow_ofs_x: 5,\n"
            "shadow_ofs_y: 5\n"
        "};\n"
        "cv.drawRect(0, 150, 50, 50, draw_rect_dsc);\n"
        /********** draw text demo ************/
        "let draw_text_dsc = {\n"
            "color: 0xFF0000,        // Red text\n"
            "sel_color: 0xFFFFFF,    // White selection\n"
            "sel_bg_color: 0x0000FF, // Blue selection background\n"
            "line_space: 5,\n"
            "letter_space: 2,\n"
            "align: LV_TEXT_ALIGN_CENTER\n"
        "};\n"
        "cv.drawText(50, 150, 100, draw_text_dsc, 'Hello, LVGL!');\n"
        /********** draw img demo ************/
        "let src = {\n"
            "data: new ArrayBuffer(20 * 20 * 2), // 20x20 pixels, RGB565\n"
            "width: 20,\n"
            "height: 20,\n"
            "cf: LV_IMG_CF_TRUE_COLOR\n"
        "};\n"
        "let srcView = new Uint16Array(src.data);\n"
        "for (let i = 0; i < srcView.length; i++) {\n"
            "srcView[i] = lv_color_make(0xDE, 0xE3, 0x19);\n"
        "}\n"
        "let draw_img_dsc = {\n"
            "angle: 45,\n"
            "zoom: 256,\n"
            "pivot: { x: 10, y: 10 },\n"
            "recolor: lv_color_make(0, 0, 0xFF),\n"
            "recolor_opa: LV_OPA_50,\n"
            "antialias: true\n"
        "};\n"
        "cv.drawImg(100, 0, src, draw_img_dsc);\n"
        /********** draw line demo ************/
        "let line_points = [\n"
            "{ x: 50, y: 50 },\n"
            "{ x: 150, y: 150 },\n"
            "{ x: 150, y: 50 }\n"
        "];\n"
        "let draw_line_dsc = {\n"
            "color: lv_color_make(0, 0, 0xFF), // Blue color\n"
            "width: 5,                         // Line width\n"
            "opa: 255,                         // Full opacity\n"
            "blend_mode: 0,\n"
            "dash_width: 10,                   // Dash width for dashed line\n"
            "dash_gap: 5,                      // Gap width for dashed line\n"
            "round_start: true,                // Rounded start\n"
            "round_end: true,                  // Rounded end\n"
            "raw_end: false                    // Do not skip perpendicular end lines\n"
        "};"
        "cv.drawLine(line_points, line_points.length, draw_line_dsc);\n"
        /********** draw polygon demo ************/
        "let poly_points = [\n"
            "{ x: 150, y: 150 },\n"
            "{ x: 170, y: 150 },\n"
            "{ x: 170, y: 170 },\n"
            "{ x: 150, y: 170 }\n"
        "];\n"
        "let draw_poly_dsc = {\n"
            "radius: 10,                                    // Rounded corners\n"
            "bg_color: lv_color_make(0xFF, 0, 0),           // Red background\n"
            "bg_opa: 255,                                   // Fully opaque background\n"
            "border_color: lv_color_make(0, 0, 0xFF),       // Blue border\n"
            "border_width: 10,                              // Border width of 5\n"
            "border_opa: 200,                               // Semi-transparent border\n"
            "outline_color: lv_color_make(0, 0 , 0xFF),     // Blue outline\n"
            "outline_width: 10,                             // Outline width of 3\n"
            "shadow_color: lv_color_make(0, 0, 0),          // Black shadow\n"
            "shadow_width: 10,                              // Shadow width of 10\n"
            "shadow_ofs_x: 5,                               // Shadow offset x\n"
            "shadow_ofs_y: 5,                               // Shadow offset y\n"
            "shadow_opa: 128,                               // Semi-transparent shadow\n"
            "bg_img_tiled: false,                           // No background image tiling\n"
            "border_post: true                              // Border is drawn on top\n"
        "};\n"
        "cv.drawPolygon(poly_points, poly_points.length, draw_poly_dsc);\n"
        /********** draw arc demo ************/
        "let draw_arc_dsc = {\n"
            "color: lv_color_make(0xFF, 0, 0), // Red color\n"
            "width: 5,                         // Arc width\n"
            "opa: 255,                         // Full opacity\n"
            "blend_mode: 0,\n"
            "rounded: true                     // Rounded ends\n"
        "};\n"
        "cv.drawArc(100, 100, 50, 45, 270, draw_arc_dsc);\n";
#endif

#if 0 /* Test checkbox */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let chb1 = new CheckBox(screen);\n"
        "let chb2 = new CheckBox(screen);\n"
        "chb1.align(LV_ALIGN_TOP_LEFT, 0, 0);\n"
        "chb2.align(LV_ALIGN_BOTTOM_LEFT, 0, 0);\n"
        "chb1.setText('Dynamic Text Example');\n"
        "chb2.setTextStatic('Static Text Example');\n"
        "print(chb1.getText());\n"
        "print(chb2.getText());\n";
#endif

#if 0 /* Test dropdown */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let drop = new DropDown(screen);\n"
        "drop.setSize(200, 50);\n"
        "drop.align(LV_ALIGN_TOP_MID, 0, 0);\n"
        "drop.setText('Choose an option');\n"
        "drop.setOptionsStatic('Opt 1\\nOpt 2\\nOpt 3');\n"
        "drop.addOption('Opt 4', 1);\n"
        "drop.setSelected(4);\n"
        // "drop.setDir(LV_DIR_ALL);\n"
        "drop.setSelectedHighLight(true);\n"
        "const options = drop.getOptions();\n"
        "print('Options:', options);\n"
        "const text = drop.getText();\n"
        "print('Text:', text);\n"
        "const list = drop.getList();\n"
        "print('Dropdown list object:', list);\n"
        "const optionCount = drop.getOptionCnt();\n"
        "print('Number of options:', optionCount);\n"
        "const bufSize = 50; // Buffer size for the selected string\n"
        "const selectedStr = drop.getSelectedStr(bufSize);\n"
        "print('Selected option text:', selectedStr);\n"
        "const optionIndex = drop.getOptionIndex('Opt 2');\n"
        "print('Option index:', optionIndex);\n"
        "const symbol = drop.getSymbol();\n"
        "print('Current symbol:', symbol);\n"
        "print('Selected highlight enabled:', drop.getSelectedHighLight());\n"
        "const dir = drop.getDir();\n"
        "print('Dropdown direction:', dir);\n"
        "drop.open();\n"
        "print('Dropdown is open:', drop.isOpen());\n"
        "drop.close();\n"
        "print('Dropdown is open after close:', drop.isOpen());\n"
        "drop.onChanged(function(e){\n"
            "const selectedIndex = drop.getSelected();\n"
            "const bufSize = 50;\n"
            "const selectedStr = drop.getSelectedStr(bufSize);\n"
            "print(`Dropdown selection changed: Index=${selectedIndex}, Text=${selectedStr}`);\n"
        "});\n";
#endif

#if 0 /* Test img */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "const width = 50;\n"
        "const height = 50;\n"
        "const bufferSize = width * height * 2;\n"
        "const imgBuffer = new ArrayBuffer(bufferSize);\n"
        "const imgView = new Uint8Array(imgBuffer);\n"
        "for (let y = 0; y < height; y++) {\n"
            "for (let x = 0; x < width; x++) {\n"
                "const index = (y * width + x) * 2;\n"
                "const color = ((x & 0x1F) << 11) | ((y & 0x3F) << 5) | (x & 0x1F);\n"
                "imgView[index] = color & 0xFF;\n"
                "imgView[index + 1] = (color >> 8) & 0xFF;\n"
            "}\n"
        "}\n"
        "const src = [width, height, imgBuffer];\n"
        "const img = new Img(screen);\n"
        "img.setSrc(src);\n"
        "img.align(LV_ALIGN_CENTER, 0, 0);\n"
        "// test getSrc()\n"
        "const retrievedBuffer = img.getSrc();\n"
        "if (retrievedBuffer instanceof ArrayBuffer) {\n"
            "const retrievedView = new Uint8Array(retrievedBuffer);\n"
            "// Example size of lv_img_dsc_t, depends on your system\n"
            "const structSize = 16;\n"
            "print('Image metadata:');\n"
            "for (let i = 0; i < structSize; i++) {\n"
                "print(`Byte ${i}: ${retrievedView[i]}`);\n"
            "}\n"
            "print('Image data (first 16 bytes):');\n"
            "for (let i = structSize; i < structSize + 16 && i < retrievedView.length; i++) {\n"
                "print(`Byte ${i - structSize}: ${retrievedView[i]}`);\n"
            "}\n"
        "} else {\n"
            "print('Failed to retrieve the image source.');\n"
        "}\n";
#endif

#if 1 /* Test line */
    const jerry_char_t script[] =
        "print('LVGL initialization done.');\n"
        "const screen = lv_scr_act();\n"
        "let line = new Line(screen);\n"
        "const points = [\n"
            "{ x: 5, y: 5 },\n"
            "{ x: 70, y: 70 },\n"
            "{ x: 120, y: 10 },\n"
            "{ x: 180, y: 60 },\n"
            "{ x: 240, y: 10 }\n"
        "];\n"
        "line.setPoints(points, 5);\n"
        "line.setYInvert(true);\n"
        "if (line.getYInvert()) {\n"
            "print('Y-axis inversion is enabled.');\n"
        "} else {\n"
            "print('Y-axis inversion is disabled.');\n"
        "}\n";
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
    jr_lv_checkbox_init();
    jr_lv_dropdown_init();
    jr_lv_img_init();
    jr_lv_line_init();

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

void lv_canvas_draw_img_demo(void) {

    // Create a canvas
    lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
    lv_obj_set_size(canvas, 200, 200);

    // Allocate a buffer for the canvas
    static lv_color_t canvas_buf[200 * 200];
    lv_canvas_set_buffer(canvas, canvas_buf, 200, 200, LV_IMG_CF_TRUE_COLOR);

    // Fill the canvas with white color
    lv_canvas_fill_bg(canvas, lv_color_hex(0xFFFFFF), LV_OPA_COVER);

    // Create image data (20x20, red color, RGB565 format)
    static lv_color_t img_buf[20 * 20];
    for (int i = 0; i < 20 * 20; i++) {
        img_buf[i] = lv_color_hex(0xFF0000); // Red color
    }

    // Create the image descriptor
    lv_img_dsc_t img_dsc;
    img_dsc.header.always_zero = 0;
    img_dsc.header.w = 20;                      // Image width
    img_dsc.header.h = 20;                      // Image height
    img_dsc.header.cf = LV_IMG_CF_TRUE_COLOR;   // Color format
    img_dsc.data = (const uint8_t *)img_buf;    // Pointer to image data
    img_dsc.data_size = sizeof(img_buf);        // Size of image data

    // Create a draw descriptor for the image
    lv_draw_img_dsc_t draw_dsc;
    lv_draw_img_dsc_init(&draw_dsc);            // Initialize with default values
    draw_dsc.angle = 45;                        // Rotate by 45 degrees
    draw_dsc.zoom = 256;                        // No zoom (256 is 1x)
    draw_dsc.pivot.x = 10;                      // Set pivot point for rotation
    draw_dsc.pivot.y = 10;
    draw_dsc.recolor = lv_color_hex(0x00FF00);  // Recolor to green
    draw_dsc.recolor_opa = LV_OPA_50;           // 50% recolor opacity
    draw_dsc.antialias = 1;                     // Enable antialiasing

    // Draw the image on the canvas at position (50, 50)
    lv_canvas_draw_img(canvas, 50, 50, &img_dsc, &draw_dsc);
}

void lv_canvas_draw_polygon_demo(void) {

    // Create a canvas
    lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
    lv_obj_set_size(canvas, 200, 200);

    // Allocate a buffer for the canvas
    static lv_color_t canvas_buf[200 * 200];
    lv_canvas_set_buffer(canvas, canvas_buf, 200, 200, LV_IMG_CF_TRUE_COLOR);

    // Fill the canvas with a white background
    lv_canvas_fill_bg(canvas, lv_color_hex(0xFFFFFF), LV_OPA_COVER);

    // Define points for the polygon
    static lv_point_t points[] = {
        {50, 50},
        {150, 50},
        {150, 150},
        {50, 150}
    };

    // Create and initialize the draw descriptor
    lv_draw_rect_dsc_t draw_dsc;
    lv_draw_rect_dsc_init(&draw_dsc);

    // Set the draw descriptor properties
    draw_dsc.radius = 10;                           // Rounded corners
    draw_dsc.bg_color = lv_color_hex(0x00FF00);     // Green background
    draw_dsc.bg_opa = LV_OPA_COVER;                 // Fully opaque background
    draw_dsc.border_color = lv_color_hex(0xFF0000); // Red border
    draw_dsc.border_width = 5;                      // Border width
    draw_dsc.border_opa = LV_OPA_80;                // Semi-transparent border
    draw_dsc.outline_color = lv_color_hex(0x0000FF);// Blue outline
    draw_dsc.outline_width = 3;                     // Outline width
    draw_dsc.shadow_color = lv_color_hex(0x888888); // Grey shadow
    draw_dsc.shadow_width = 10;                     // Shadow width
    draw_dsc.shadow_ofs_x = 5;                      // Shadow offset X
    draw_dsc.shadow_ofs_y = 5;                      // Shadow offset Y
    draw_dsc.shadow_opa = LV_OPA_50;                // Semi-transparent shadow
    draw_dsc.bg_img_tiled = 0;                      // No background image tiling
    draw_dsc.border_post = 1;                       // Border drawn on top

    // Draw the polygon on the canvas
    lv_canvas_draw_polygon(canvas, points, 4, &draw_dsc);
}

void lv_canvas_draw_arc_demo(void) {
    // Allocate a buffer for the canvas
    static lv_color_t canvas_buf[200 * 200];
    lv_obj_t *canvas = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(canvas, canvas_buf, 200, 200, LV_IMG_CF_TRUE_COLOR);

    // Fill the canvas with a white background
    lv_canvas_fill_bg(canvas, lv_color_make(255, 255, 255), LV_OPA_COVER);

    // Define and initialize the draw descriptor
    lv_draw_arc_dsc_t draw_dsc;
    lv_draw_arc_dsc_init(&draw_dsc);

    draw_dsc.color = lv_color_make(0xFF, 0, 0); // Red color
    draw_dsc.width = 5;                         // Arc width
    draw_dsc.opa = LV_OPA_COVER;                // Full opacity
    draw_dsc.blend_mode = LV_BLEND_MODE_NORMAL;
    draw_dsc.rounded = 1;                    // Rounded ends

    // Draw an arc on the canvas
    lv_canvas_draw_arc(canvas, 100, 100, 50, 45, 270, &draw_dsc);
}

void create_dropdown_with_symbol(void) {

    static const char * opts = LV_SYMBOL_WIFI" WI-FI\n"
    						   LV_SYMBOL_FILE" File\n"
							   LV_SYMBOL_BATTERY_FULL" Battery\n"
							   LV_SYMBOL_USB" USB";

    lv_obj_t *dropdown = lv_dropdown_create(lv_scr_act());
    lv_obj_align(dropdown, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_width(dropdown, 200);
    lv_obj_set_height(dropdown, 40);
    lv_dropdown_set_options_static(dropdown, opts);
}

extern void execute_launcher(void);

void test_lv_img_with_buffer(void) {
    const lv_coord_t width = 50;
    const lv_coord_t height = 50;
    const size_t buffer_size = width * height * 2; // RGB565 format (2 bytes per pixel)

    static uint8_t img_buffer[50 * 50 * 2]; // Raw buffer for the image

    // Fill the buffer with a gradient pattern
    for (lv_coord_t y = 0; y < height; y++) {
        for (lv_coord_t x = 0; x < width; x++) {
            uint16_t color = ((x & 0x1F) << 11) | ((y & 0x3F) << 5) | (x & 0x1F); // RGB565
            size_t index = (y * width + x) * 2;
            img_buffer[index] = color & 0xFF;          // Low byte
            img_buffer[index + 1] = (color >> 8) & 0xFF; // High byte
        }
    }

    // Define the image descriptor
    static lv_img_dsc_t img_dsc = {
        .header.always_zero = 0,
        .header.w = width,
        .header.h = height,
        .header.cf = LV_IMG_CF_TRUE_COLOR,
        .data_size = buffer_size,
        .data = img_buffer,
    };

    // Create an image object
    lv_obj_t *img = lv_img_create(lv_scr_act());
    lv_img_set_src(img, &img_dsc);

    // Align the image to the center of the screen
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}

void lv_example_line(void)
{
    /*Create an array for the points of the line*/
    static lv_point_t line_points[] = { {5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10} };

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 8);
    lv_style_set_line_color(&style_line, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_line_rounded(&style_line, true);

    /*Create a line and apply the new style*/
    lv_obj_t * line;
    line = lv_line_create(lv_scr_act());
    lv_line_set_points(line, line_points, 5);     /*Set the points*/
    lv_obj_add_style(line, &style_line, 0);
    lv_obj_center(line);
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
    //lv_canvas_draw_img_demo();
    //lv_canvas_draw_polygon_demo();
    //lv_canvas_draw_arc_demo();
    //execute_launcher();
    //create_dropdown_with_symbol();
    //test_lv_img_with_buffer();
    //lv_example_line();

    lv_task_handler();
    xTaskCreate(lvgl_task, (char *)"lvgl task", 512, NULL, 15, NULL);
}