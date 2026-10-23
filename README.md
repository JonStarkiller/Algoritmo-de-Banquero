# Algoritmo del Banquero (Banker's Algorithm)

Implementación en C++ del **Algoritmo del Banquero** según el libro *Sistemas Operativos. Aspectos internos y principios de diseño* de **William Stallings** (sección 6.3 - Predicción del Interbloqueo).

Permite simular el estado de un sistema con múltiples procesos y recursos, determinar si el estado es **seguro** o **inseguro**, y experimentar agregando o retirando recursos para observar cómo cambia la seguridad del sistema.

---

## Características

- Carga de:
  - Cantidad de procesos y recursos
  - Vector de **Recursos Totales (R)**
  - Matriz de **Asignación Actual (Allocation / A)**
  - Matriz de **Requisitos Máximos (Max / N)**
- Cálculo automático de:
  - Vector de **Recursos Disponibles (D)**
  - Matriz de **Necesidad Restante (Need = N - A)**
- Validaciones según las relaciones del libro:
  - \( R_j = D_j + \sum A_{ij} \)
  - \( N_{ij} \leq R_j \)
  - \( A_{ij} \leq N_{ij} \)
- **Algoritmo de Seguridad** completo (Safety Algorithm) con salida paso a paso
- Cuando el estado es inseguro, muestra el detalle de por qué cada proceso no puede ejecutarse
- Menú interactivo para **agregar o retirar recursos** y re-evaluar la seguridad
- Interfaz con colores ANSI y tablas alineadas
- Soporte de caracteres acentuados en Windows

---

## Requisitos

- Compilador C++ (g++, MinGW, MSVC, Clang, etc.)
- Sistema operativo **Windows** (utiliza `windows.h` y `SetConsoleOutputCP` para UTF-8)

---

## Uso

1. Ingresar cantidad de procesos y recursos.
2. Ingresar el vector de Recursos Totales (R).
3. Ingresar la matriz de Asignación Actual (A).
4. Ingresar la matriz de Requisitos Máximos (Max / N).
5. El programa calcula Disponibles y Need, ejecuta el algoritmo de seguridad y muestra el resultado.
6. En el menú se puede:
   - Modificar los recursos totales (agregar o retirar unidades).
   - Volver a evaluar si el sistema queda seguro o inseguro.
   - Salir.

---

## Ejemplo de uso (datos de la actividad)
```
Procesos: 3
Recursos: 2

Totales (R):     3  2

Allocation:
  P0: 1  0
  P1: 1  1
  P2: 1  1

Max:
  P0: 3  2
  P1: 2  2
  P2: 3  1

Resultado: Estado INSEGURO (Disponibles = [0, 0])

Al agregar 2 unidades de R1 y 1 unidad de R2, el sistema se vuelve SEGURO
```

---

## Referencia
- Sistemas Operativos. Aspectos internos y principios de diseño (5ta Ed)<br>
Capítulo 6 – Concurrencia: Interbloqueo e inanición<br>
Sección 6.3 – Predicción del interbloqueo (Algoritmo del Banquero)<br>
Autor: William Stalling
---
