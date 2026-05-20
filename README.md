# 🍞 Taller WebAssembly — Calculadora Panadera

Calculadora de operaciones básicas implementada en **JavaScript puro** y
**WebAssembly (compilado desde C)**, diseñada para que los asistentes comparen
el rendimiento de ambas tecnologías en tiempo real.

---

## Estructura del proyecto

```
taller-wasm/
├── bakery_calc.c          ← Código C con las operaciones (compilar a .wasm)
├── README.md              ← Este archivo
│
├── index.html       ← Calculadora en JavaScript puro (abrir directo)
│
├── wasm_index.html         ← Calculadora que carga el módulo .wasm

├──  bakery_calc.wasm   ← Generado al compilar bakery_calc.c
├── guia_taller.pdf
```

---

## Requisitos

1. Visual Studio Code
2. Python 3
3. Extensión Live Server de Visual Studio Code


---


## Paso 1 — Compilar el módulo WebAssembly

### Opción A: Emscripten (instalar localmente)

```bash
# Instalar Emscripten (una sola vez)
git clone https://github.com/emscripten-core/emsdk
cd emsdk
./emsdk install latest
./emsdk activate latest


# Compilar bakery_calc.c
emcc bakery_calc.c -O2 -o bakery_calc.wasm --no-entry -s EXPORTED_FUNCTIONS='["_calc_add","_calc_sub","_calc_mul","_calc_div","_benchmark"]'
```

### Opción B: WasmFiddle (sin instalar nada)

1. Abrir https://wasdk.github.io/WasmFiddle/
2. Pegar el contenido de `bakery_calc.c` en el panel izquierdo.
3. Hacer clic en **Build** → descargar el `.wasm` generado.
4. Renombrarlo a `bakery_calc.wasm` y copiarlo a la carpeta del taller.


## Paso 2 — Ejecutar ambas versiones del taller

Los navegadores bloquean `fetch()` en `file://`, así que necesitas un
servidor HTTP local, para eso abre un servidor local con la extensión Live Server :



Luego abrir: **http://localhost:5500/index.html**
Luego abrir: **http://localhost:5500/wasm_index.html**

Ejecuta el benchmark con el mismo número de iteraciones y compara los tiempos.

---

## Actividad del taller — Modificar las operaciones

### En la versión JavaScript

Editar `bakery_js/index.html`, buscar la función `jsBenchmark()` y
cambiar la operación dentro del bucle:

```javascript
// Nivel 1 — suma simple (el JIT la optimiza muy bien)
acc += i + i;

// Nivel 2 — raíz cuadrada (más costosa)
acc += Math.sqrt(i);

// Nivel 3 — operación compleja
const arr = new Float64Array(1024);
    for (let j = 0; j < 1024; j++) arr[j] = j * 0.5;
    acc += arr[i % 1024];
```

### En la versión WebAssembly

Editar `bakery_calc.c`, buscar la función `benchmark()` y cambiar la
misma operación:

```c
// Nivel 1 — suma simple
acc += (double)i + (double)i;

// Nivel 2 — raíz cuadrada (enlazar con -lm)
acc += sqrt((double)iterations);

// Nivel 3 — operación compleja
  double arr[1024];
    for (int j = 0; j < 1024; j++) arr[j] = j * 0.5;

    for (int i = 0; i < iterations; i++) {
        acc += arr[i % 1024];
    }
```

Recompilar y medir de nuevo. Con nivel 2 y 3 la diferencia entre JS y
Wasm se vuelve más pronunciada.

---

## Tabla de resultados esperados

| Operación    | JavaScript | WebAssembly | Diferencia |
|--------------|-----------|-------------|------------|
| Suma (10M)   | ~80 ms    | ~60 ms      | ~1.3×      |
| sqrt (10M)   | ~150 ms   | ~80 ms      | ~1.9×      |
| pow+div (1M) | ~120 ms   | ~50 ms      | ~2.4×      |

*Los tiempos varían según navegador, hardware y nivel de optimización del compilador.*

### Para una guía detallada paso a paso abre el documento [guia_taller_web_Assembly.pdf](guia_taller_web_Assembly.pdf)

---

## Niveles de optimización de Emscripten (opcional, no será evaluado como parte del taller)

Prueba compilar con distintos niveles y observa el tamaño del `.wasm`:

```bash
emcc bakery_calc.c -O0 -o bakery_calc_O0.wasm --no-entry ...  # Sin optimizar
emcc bakery_calc.c -O1 -o bakery_calc_O1.wasm --no-entry ...  # Básico
emcc bakery_calc.c -O2 -o bakery_calc_O2.wasm --no-entry ...  # Recomendado
emcc bakery_calc.c -O3 -o bakery_calc_O3.wasm --no-entry ...  # Agresivo
emcc bakery_calc.c -Os -o bakery_calc_Os.wasm --no-entry ...  # Optimizar tamaño

ls -lh bakery_calc_O*.wasm
```

---

## Recursos adicionales

- [MDN WebAssembly](https://developer.mozilla.org/en-US/docs/WebAssembly)
- [Emscripten docs](https://emscripten.org/docs/)
- [WasmFiddle](https://wasdk.github.io/WasmFiddle/) — probar sin instalar
- [WebAssembly Studio](https://webassembly.studio/) — IDE online
- *The Art of WebAssembly* — Rick Battagline (No Starch Press)
