#include <stdio.h>

/* FreeRTOS */
#include <FreeRTOS.h>
#include <task.h>

/* aos */
#include <cli.h>

/* fs */
#include <bl_romfs.h>

/* RISCV */
#include <csi_core.h>

/* Duktape */
#include "duktape.h"

// Define the JavaScript function (simplified for Duktape 2.7.0)
const char *js_function =
    "function solveQuadratic(a, b, c) {"
    "    if (a === 0) {"
    "        if (b === 0) {"
    "            return c === 0 ? 'Infinite solutions' : 'No solution';"
    "        }"
    "        return [-c / b];"
    "    }"
    "    var discriminant = b * b - 4 * a * c;"
    "    if (discriminant > 0) {"
    "        var root1 = (-b + Math.sqrt(discriminant)) / (2 * a);"
    "        var root2 = (-b - Math.sqrt(discriminant)) / (2 * a);"
    "        return [root1, root2];"
    "    } else if (discriminant === 0) {"
    "        var root = -b / (2 * a);"
    "        return [root];"
    "    } else {"
    "        var realPart = -b / (2 * a);"
    "        var imaginaryPart = Math.sqrt(-discriminant) / (2 * a);"
    "        return [realPart + ' + ' + imaginaryPart + 'i', realPart + ' - ' + imaginaryPart + 'i'];"
    "    }"
    "}";

static duk_ret_t native_print(duk_context *ctx) {
    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    printf("%s\r\n", duk_safe_to_string(ctx, -1));
    return 0;
}

static duk_ret_t native_adder(duk_context *ctx) {
    int i;
    int n = duk_get_top(ctx);  /* #args */
    double res = 0.0;

    for (i = 0; i < n; i++) {
        res += duk_to_number(ctx, i);
}

    duk_push_number(ctx, res);
    return 1;  /* one return value */
}

void main()
{
    duk_context *ctx = duk_create_heap_default();
    duk_push_c_function(ctx, native_print, DUK_VARARGS);
    duk_put_global_string(ctx, "print");
    duk_push_c_function(ctx, native_adder, DUK_VARARGS);
    duk_put_global_string(ctx, "adder");
    duk_eval_string(ctx, "print('Hello world!');");
    duk_eval_string(ctx, "print('2+3=' + adder(2, 3));");

    // Evaluate the JavaScript function in Duktape
    duk_eval_string(ctx, js_function);

    // Call the solveQuadratic function with arguments (1, -3, 2)
    duk_push_global_object(ctx);          // [global]
    duk_get_prop_string(ctx, -1, "solveQuadratic"); // [global solveQuadratic]
    duk_push_number(ctx, 1);             // Argument a
    duk_push_number(ctx, 0);            // Argument b
    duk_push_number(ctx, 1);             // Argument c
    duk_call(ctx, 3);                    // Call the function with 3 arguments

    // Print the result
    if (duk_is_array(ctx, -1)) {
        duk_size_t len = duk_get_length(ctx, -1);
        printf("Roots: ");
        for (duk_size_t i = 0; i < len; i++) {
            duk_get_prop_index(ctx, -1, i);
            printf("%s ", duk_to_string(ctx, -1));
            duk_pop(ctx);
        }
        printf("\r\n");
    } else if (duk_is_string(ctx, -1)) {
        printf("Result: %s\n", duk_to_string(ctx, -1));
    } else {
        printf("Unexpected result.\n");
    }

    duk_pop(ctx);  /* pop eval result */
    duk_destroy_heap(ctx);
}