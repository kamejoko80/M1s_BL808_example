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

/* Jerryscript */
#include "jerryscript.h"
#include "jerryscript-ext/handlers.h"
#include "jerryscript-ext/properties.h"

/**
 * Jerryscript simple test
 */
void jerryscript_demo(void)
{
    const char *js_script =
        "class Person {\n"
        "    constructor(name, age) {\n"
        "        this.name = name;\n"
        "        this.age = age;\n"
        "    }\n"
        "\n"
        "    greet() {\n"
        "        print(`Hello, my name is ${this.name} and I am ${this.age} years old.`);\n"
        "    }\n"
        "\n"
        "    getYearOfBirth(currentYear) {\n"
        "        return currentYear - this.age;\n"
        "    }\n"
        "}\n"
        "\n"
        "const person1 = new Person(\"Alice\", 25);\n"
        "person1.greet();\n"
        "const birthYear = person1.getYearOfBirth(2024);\n"
        "print(`I was born in the year ${birthYear}.`);\n";

    // Initialize JerryScript
    jerry_init(JERRY_INIT_EMPTY);

    /* Register the print function in the global object */
    jerry_log_set_level(JERRY_LOG_LEVEL_DEBUG);

    /* Register the print function in the global object */
    jerryx_register_global ("print", jerryx_handler_print);

    // Evaluate the JavaScript
    jerry_value_t result = jerry_eval((const jerry_char_t *)js_script, strlen(js_script), JERRY_PARSE_NO_OPTS);

    // Check for errors
    if (jerry_value_is_error(result)) {
        printf("Error evaluating script!\n");
    }

    // Cleanup
    jerry_value_free(result);
    jerry_cleanup();
}

void main()
{
    jerryscript_demo();
}