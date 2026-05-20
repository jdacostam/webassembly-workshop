/*
 * =============================================================
 *  TALLER WEBASSEMBLY — Calculadora Panadera
 *  Archivo: bakery_calc.c
 * =============================================================
 *
 *  PASO 1: Compilar este archivo a WebAssembly con Emscripten:
 *
 *    emcc bakery_calc.c -O2 -o bakery_wasm/bakery_calc.wasm \
 *         --no-entry \
 *         -s EXPORTED_FUNCTIONS='["_calc_add","_calc_sub","_calc_mul","_calc_div","_benchmark"]' \
 *         -s EXPORTED_RUNTIME_METHODS='["cwrap"]'
 *
 *  PASO 2: Servir los archivos con un servidor local (los navegadores
 *          bloquean fetch() en file://):
 *
 *    python3 -m http.server 8080
 *    # luego abrir: http://localhost:8080/bakery_wasm/index.html
 *
 *  ALTERNATIVA ONLINE (sin instalar nada):
 *    Pegar este código en https://wasdk.github.io/WasmFiddle/
 *
 * =============================================================
 *  ZONA DE MODIFICACIÓN PARA EL TALLER
 *  Los asistentes pueden:
 *   - Cambiar el tipo de dato (float → double → int) y medir
 *   - Agregar operaciones más costosas (sqrt, pow, bucles)
 *   - Cambiar el nivel de optimización (-O0, -O1, -O2, -O3, -Os)
 *     y comparar el tamaño del .wasm resultante
 * =============================================================
 */

#include <stdlib.h>
#include <math.h>

/*
 * EMSCRIPTEN_KEEPALIVE evita que el compilador elimine estas funciones
 * al optimizar ("tree shaking"). Sin esta macro, -O2 podría eliminarlas
 * por considerarlas "no usadas".
 *
 * Si compilas con --no-entry y EXPORTED_FUNCTIONS, también puedes
 * omitir esta macro y solo listar las funciones en EXPORTED_FUNCTIONS.
 */
#define EMSCRIPTEN_KEEPALIVE __attribute__((used))

/* ----------------------------------------------------------
 * Operaciones básicas de la calculadora panadera
 * Trabajan con gramos, tazas, porciones, etc.
 * ---------------------------------------------------------- */

EMSCRIPTEN_KEEPALIVE
double calc_add(double a, double b) {
    return a + b;
}

EMSCRIPTEN_KEEPALIVE
double calc_sub(double a, double b) {
    return a - b;
}

EMSCRIPTEN_KEEPALIVE
double calc_mul(double a, double b) {
    return a * b;
}

EMSCRIPTEN_KEEPALIVE
double calc_div(double a, double b) {
    /* Prevenir división por cero — devolver 0 como convención */
    if (b == 0.0) return 0.0;
    return a / b;
}

/* ----------------------------------------------------------
 * FUNCIÓN DE BENCHMARK
 *
 * Ejecuta `iterations` operaciones aritméticas y devuelve
 * la suma acumulada (para evitar que el compilador optimice
 * el bucle y lo elimine al ver que el resultado no se usa).
 *
 * ZONA DE MODIFICACIÓN:
 *   Cambia la operación dentro del bucle para medir distintos
 *   tipos de cómputo. Por ejemplo:
 *
 *   Nivel 1 (básico):   acc += (double)i + (double)i;
 *   Nivel 2 (medio):    acc += sqrt((double)i);
 *   Nivel 3 (intenso): 
 *  
    double arr[1024];
    for (int j = 0; j < 1024; j++) arr[j] = j * 0.5;
        acc += arr[i % 1024];
 
 * 
 * ---------------------------------------------------------- */
EMSCRIPTEN_KEEPALIVE
double benchmark(int iterations) {
    double acc = 0.0;


    for (int i = 0; i < iterations; i++) {
    acc += (double)i + (double)i;
    }
    return acc; /* Retornar para que el compilador no elimine el bucle */
}
