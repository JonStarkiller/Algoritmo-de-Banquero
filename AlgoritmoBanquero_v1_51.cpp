// FADENA - Facultad de Defensa Nacional
// Licenciatura en Ciberdefensa
// Autor: J. Luis Rey
// Descripción: Implementación del Algoritmo del Banquero para la predicción de interbloqueo.

// Fecha: 2026-10-21
// Version 1.51

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <windows.h>

using namespace std;

const int MAX_PROCESOS = 10;
const int MAX_RECURSOS = 10;

// Códigos ANSI
const string RESET    = "\033[0m";
const string VERDE    = "\033[1;32m";
const string ROJO     = "\033[1;31m";
const string AMARILLO = "\033[1;33m";
const string CYAN     = "\033[1;36m";
const string MAGENTA  = "\033[1;35m";

int leerEnteroEnRango(const string& mensaje, int minVal, int maxVal) {
    int valor;
    while (true) {
        cout << mensaje;
        cin >> valor;
        if (cin.fail() || valor < minVal || valor > maxVal) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << ROJO << "   [!] Valor inválido. Ingrese un número entre "
                << minVal << " y " << maxVal << "." << RESET << "\n";
        } else {
            return valor;
        }
    }
}

void imprimirMatriz(const string& titulo, int numProcesos, int numRecursos,
                    const vector<vector<int>>& matriz) {
    cout << "\n" << CYAN << "=== " << titulo << " ===" << RESET << "\n";

    cout << "+---------+";
    for (int j = 0; j < numRecursos; ++j) cout << "----+";
    cout << "\n";

    cout << "| " << left << setw(7) << "Proceso" << " |";
    for (int j = 0; j < numRecursos; ++j) {
        cout << " R" << (j + 1) << " |";
    }
    cout << "\n";

    cout << "+---------+";
    for (int j = 0; j < numRecursos; ++j) cout << "----+";
    cout << "\n";

    for (int i = 0; i < numProcesos; ++i) {
        cout << "| " << AMARILLO << left << setw(7) << ("P" + to_string(i)) << RESET << " |";
        for (int j = 0; j < numRecursos; ++j) {
            cout << " " << right << setw(2) << matriz[i][j] << " |";
        }
        cout << "\n";

        cout << "+---------+";
        for (int j = 0; j < numRecursos; ++j) cout << "----+";
        cout << "\n";
    }
}

void imprimirVector(const string& titulo, const vector<int>& v) {
    cout << titulo << " [ ";
    for (int valor : v) cout << valor << " ";
    cout << "]\n";
}

bool esEstadoSeguro(int numProcesos, int numRecursos,
                    const vector<int>& available,
                    const vector<vector<int>>& allocation,
                    const vector<vector<int>>& need,
                    vector<int>& secuenciaSegura,
                    bool mostrarPasos = true)
{
    vector<int> work = available;
    vector<bool> finish(numProcesos, false);
    secuenciaSegura.clear();

    if (mostrarPasos) {
        cout << "\n" << CYAN << "--- Algoritmo de Seguridad (paso a paso) ---" << RESET << "\n";
    }

    bool progreso;
    do {
        progreso = false;

        for (int i = 0; i < numProcesos; ++i) {
            if (finish[i]) continue;

            bool puede = true;
            for (int j = 0; j < numRecursos; ++j) {
                if (need[i][j] > work[j]) {
                    puede = false;
                    break;
                }
            }

            if (puede) {
                if (mostrarPasos) {
                    cout << VERDE << "\n[+] P" << i << " puede ejecutarse (Need ≤ Work)" << RESET << "\n";
                    cout << "    Need: [ ";
                    for (int j = 0; j < numRecursos; ++j) cout << need[i][j] << " ";
                    cout << "]\n";
                }

                for (int j = 0; j < numRecursos; ++j) {
                    work[j] += allocation[i][j];
                }

                finish[i] = true;
                secuenciaSegura.push_back(i);
                progreso = true;

                if (mostrarPasos) {
                    cout << "    Work actualizado: [ ";
                    for (int val : work) cout << val << " ";
                    cout << "]\n";
                }
            }
        }
    } while (progreso);

    // Verificar si todos terminaron
    bool seguro = true;
    for (bool f : finish) {
        if (!f) {
            seguro = false;
            break;
        }
    }

    // Si es inseguro, mostrar el detalle de por qué cada proceso no pudo ejecutarse
    if (!seguro && mostrarPasos) {
        cout << "\n" << ROJO << "Detalle de procesos que NO pudieron ejecutarse:" << RESET << "\n";
        cout << "Work final: [ ";
        for (int val : work) cout << val << " ";
        cout << "]\n\n";

        for (int i = 0; i < numProcesos; ++i) {
            if (!finish[i]) {
                cout << ROJO << "[-] P" << i << " NO puede ejecutarse" << RESET << "\n";
                cout << "    Need: [ ";
                for (int j = 0; j < numRecursos; ++j) cout << need[i][j] << " ";
                cout << "]\n";
                cout << "    Motivo: ";
                bool primer = true;
                for (int j = 0; j < numRecursos; ++j) {
                    if (need[i][j] > work[j]) {
                        if (!primer) cout << " | ";
                        cout << "R" << (j + 1) << " necesita " << need[i][j] 
                            << " y solo hay " << work[j];
                        primer = false;
                    }
                }
                cout << "\n\n";
            }
        }
    }

    if (mostrarPasos) {
        if (seguro)
            cout << VERDE << "→ El estado es SEGURO." << RESET << "\n";
        else
            cout << ROJO << "→ El estado es INSEGURO." << RESET << "\n";
    }

    return seguro;
}

void ejecutarAlgoritmoBanquero() {
    int numProcesos, numRecursos;

    cout << CYAN << "==============================================" << RESET << "\n";
    cout << "   ALGORITMO DEL BANQUERO (W. Stallings)     \n";
    cout << "   Predicción de interbloqueo                 \n";
    cout << CYAN << "==============================================" << RESET << "\n\n";

    numProcesos = leerEnteroEnRango("1) Cantidad de procesos (1-" + to_string(MAX_PROCESOS) + "): ", 1, MAX_PROCESOS);
    numRecursos = leerEnteroEnRango("2) Cantidad de recursos (1-" + to_string(MAX_RECURSOS) + "): ", 1, MAX_RECURSOS);

    vector<int> totalRecursos(numRecursos);
    vector<int> available(numRecursos);
    vector<vector<int>> maximo(numProcesos, vector<int>(numRecursos));
    vector<vector<int>> allocation(numProcesos, vector<int>(numRecursos));
    vector<vector<int>> need(numProcesos, vector<int>(numRecursos));

    // 3) Recursos totales
    cout << "\n" << CYAN << "--- 3) Recursos Totales del Sistema (Vector R) ---" << RESET << "\n";
    for (int j = 0; j < numRecursos; ++j) {
        totalRecursos[j] = leerEnteroEnRango("Total del recurso R" + to_string(j + 1) + ": ", 0, 99);
    }

    // 4) Asignación Actual
    cout << "\n" << CYAN << "--- 4) Asignación Actual (Matriz Allocation / A) ---" << RESET << "\n";
    for (int i = 0; i < numProcesos; ++i) {
        for (int j = 0; j < numRecursos; ++j) {
            allocation[i][j] = leerEnteroEnRango("P" + to_string(i) + " → Asignado de R" + to_string(j + 1) + ": ", 0, 99);
        }
    }

    // 5) Requisitos Máximos
    cout << "\n" << CYAN << "--- 5) Requisitos Máximos Declarados (Matriz Max / N) ---" << RESET << "\n";
    for (int i = 0; i < numProcesos; ++i) {
        for (int j = 0; j < numRecursos; ++j) {
            maximo[i][j] = leerEnteroEnRango("P" + to_string(i) + " → Máximo de R" + to_string(j + 1) + ": ", 0, 99);

            while (maximo[i][j] < allocation[i][j]) {
                cout << ROJO << "   [!] El máximo no puede ser menor que lo ya asignado (" 
                    << allocation[i][j] << ")." << RESET << "\n";
                maximo[i][j] = leerEnteroEnRango("P" + to_string(i) + " → Máximo de R" + to_string(j + 1) + ": ", 0, 99);
            }
            while (maximo[i][j] > totalRecursos[j]) {
                cout << ROJO << "   [!] No puede superar el total del sistema (" << totalRecursos[j] << ")." << RESET << "\n";
                maximo[i][j] = leerEnteroEnRango("P" + to_string(i) + " → Máximo de R" + to_string(j + 1) + ": ", 0, 99);
            }
        }
    }

    // Calcular Available
    for (int j = 0; j < numRecursos; ++j) {
        int suma = 0;
        for (int i = 0; i < numProcesos; ++i) suma += allocation[i][j];

        if (suma > totalRecursos[j]) {
            cout << "\n" << ROJO << "[ERROR] R" << (j + 1) << " tiene " << suma
                << " unidades asignadas, pero solo existen " << totalRecursos[j] << "." << RESET << "\n";
            return;
        }
        available[j] = totalRecursos[j] - suma;
    }

    // Calcular Need
    for (int i = 0; i < numProcesos; ++i)
        for (int j = 0; j < numRecursos; ++j)
            need[i][j] = maximo[i][j] - allocation[i][j];

    // Mostrar estado inicial
    imprimirMatriz("MATRIZ DE ASIGNACIÓN ACTUAL (Allocation / A)", numProcesos, numRecursos, allocation);
    imprimirMatriz("MATRIZ DE REQUISITOS MÁXIMOS (Max / N)", numProcesos, numRecursos, maximo);
    imprimirMatriz("MATRIZ DE NECESIDAD RESTANTE (Need = N - A)", numProcesos, numRecursos, need);

    cout << "\n" << CYAN << "=== VECTORES DEL SISTEMA ===" << RESET << "\n";
    imprimirVector("Recursos Totales (R):     ", totalRecursos);
    imprimirVector("Recursos Disponibles (D): ", available);

    vector<int> secuencia;
    bool seguroInicial = esEstadoSeguro(numProcesos, numRecursos, available, allocation, need, secuencia, true);

    cout << "\n" << CYAN << "==============================================" << RESET << "\n";
    if (seguroInicial) {
        cout << "ESTADO INICIAL: " << VERDE << "SEGURO" << RESET << "\n";
        cout << "Secuencia segura: " << AMARILLO;
        for (size_t k = 0; k < secuencia.size(); ++k)
            cout << "P" << secuencia[k] << (k + 1 < secuencia.size() ? " → " : "");
        cout << RESET << "\n";
    } else {
        cout << "ESTADO INICIAL: " << ROJO << "INSEGURO (riesgo de interbloqueo)" << RESET << "\n";
    }
    cout << CYAN << "==============================================" << RESET << "\n";

    // Menú
    while (true) {
        cout << "\n" << MAGENTA << "--- Menú de acciones ---" << RESET << "\n";
        cout << "1) Modificar recursos totales del sistema (agregar o retirar)\n";
        cout << "0) Salir\n";
        int opcion = leerEnteroEnRango("Elija una opción: ", 0, 1);

        if (opcion == 0) break;

        cout << "\n" << CYAN << "--- Modificar Recursos Totales del Sistema ---" << RESET << "\n";
        imprimirVector("Totales actuales (R):     ", totalRecursos);
        imprimirVector("Disponibles actuales (D): ", available);

        cout << "\nIndique cuántas unidades desea agregar (+) o retirar (-) de cada recurso.\n";
        cout << "(Ejemplo: 2 agrega 2 unidades, -1 retira 1 unidad, 0 no modifica)\n\n";

        bool cambioValido = true;
        vector<int> delta(numRecursos);

        for (int j = 0; j < numRecursos; ++j) {
            delta[j] = leerEnteroEnRango("  Cambio para R" + to_string(j + 1) + ": ", -99, 99);

            if (delta[j] < 0 && (-delta[j]) > available[j]) {
                cout << ROJO << "   [!] No se pueden retirar " << (-delta[j])
                    << " unidades de R" << (j + 1) << ". Solo hay " << available[j] << " disponibles." << RESET << "\n";
                cambioValido = false;
            }
        }

        if (!cambioValido) {
            cout << ROJO << "\nOperación cancelada. No se modificó nada." << RESET << "\n";
            continue;
        }

        for (int j = 0; j < numRecursos; ++j) {
            totalRecursos[j] += delta[j];
            available[j]     += delta[j];
        }

        cout << "\n" << VERDE << "Recursos actualizados correctamente." << RESET << "\n";
        imprimirVector("Nuevos Totales (R):       ", totalRecursos);
        imprimirVector("Nuevos Disponibles (D):   ", available);

        vector<int> nuevaSecuencia;
        bool ahoraSeguro = esEstadoSeguro(numProcesos, numRecursos, available, allocation, need, nuevaSecuencia, true);

        cout << "\n" << CYAN << "==============================================" << RESET << "\n";
        if (ahoraSeguro) {
            cout << "NUEVO ESTADO: " << VERDE << "SEGURO" << RESET << "\n";
            cout << "Secuencia segura: " << AMARILLO;
            for (size_t k = 0; k < nuevaSecuencia.size(); ++k)
                cout << "P" << nuevaSecuencia[k] << (k + 1 < nuevaSecuencia.size() ? " → " : "");
            cout << RESET << "\n";
        } else {
            cout << "NUEVO ESTADO: " << ROJO << "INSEGURO (riesgo de interbloqueo)" << RESET << "\n";
        }
        cout << CYAN << "==============================================" << RESET << "\n";
    }

    // Estado final
    cout << "\n" << CYAN << "--- Estado final del sistema ---" << RESET << "\n";
    imprimirMatriz("Allocation final", numProcesos, numRecursos, allocation);
    imprimirMatriz("Need final", numProcesos, numRecursos, need);
    imprimirVector("Totales finales (R):      ", totalRecursos);
    imprimirVector("Disponibles finales (D):  ", available);

    cout << "\n\n";
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    ejecutarAlgoritmoBanquero();
    return 0;
}