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

static jerry_value_t adder_handler(const jerry_call_info_t *call_info_p,
                                   const jerry_value_t arguments[],
                                   const jerry_length_t arguments_count)
{
   jerry_value_t result = 0;

  /* There should be at least one argument */
  if (arguments_count > 0)
  {
    /* Convert the first argument to a value (a) */
    jerry_value_t a = jerry_value_to_number(arguments[0]);
    /* Convert the first argument to a value (b) */
    jerry_value_t b = jerry_value_to_number(arguments[1]);
    result = a + b;
  }

  /* Return value to the JavaScript engine */
  return result;
}

void register_functions(void)
{
    /* Get the "global" object */
    jerry_value_t global_object = jerry_current_realm();
    /* Create a "adder" JS string */
    jerry_value_t property_name_print = jerry_string_sz("adder");
    /* Create a function from a native C method (this function will be called from JS) */
    jerry_value_t property_value_func = jerry_function_external(adder_handler);
    /* Add the "print" property with the function value to the "global" object */
    jerry_value_t set_result = jerry_object_set(global_object, property_name_print, property_value_func);

    /* Check if there was no error when adding the property (in this case it should never happen) */
    if (jerry_value_is_exception (set_result)) {
        printf ("Failed to add the 'adder' property\n");
    }

    /* Release all jerry_value_t-s */
    jerry_value_free(set_result);
    jerry_value_free(property_value_func);
    jerry_value_free(property_name_print);
    jerry_value_free(global_object);
}

void jerryscript_native_c_call_demo(void)
{
    const jerry_char_t script[] =
        "var result = adder(5, 3);\n"
        "print('The result is: ' + result);\n";
    const jerry_length_t script_size = sizeof (script) - 1;

    /* Initialize engine */
    jerry_init(JERRY_INIT_EMPTY);

    /* Register the adder function in the global object */
    register_functions();

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
    jerry_cleanup ();
}

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
    jerryscript_native_c_call_demo();
}