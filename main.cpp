#include <iostream>
#include <ctime>
#include <cstdlib>
#include <set>
#include <vector>
#include <cmath>
#include <iomanip>
#include "SparseMatrix.h"
using namespace std;

// FUNCIÓN PARA GENERAR COORDENADAS ÚNICAS (SIN SOBRESCRIBIR)
// Genera N coordenadas únicas dentro de una matriz de tamaño filas x columnas
vector<pair<int,int>> generarCoordenadasUnicas(int N, int filas, int columnas, set<pair<int,int>>& usadas) {
    usadas.clear();
    vector<pair<int,int>> res;
    res.reserve(N);
    while ((int)res.size() < N) {
        int x = rand() % filas;
        int y = rand() % columnas;
        if (!usadas.count({x,y})) {
            usadas.insert({x,y});
            res.push_back({x,y});
        }
    }
    return res;
}
// FUNCIÓN PARA GENERAR COORDENADAS QUE NO ESTÉN EN EL CONJUNTO USADAS
// Sirve para saber el peor caso
vector<pair<int,int>> generarCoordenadasNoUsadas(int N, int filas, int columnas, const set<pair<int,int>>& usadas) {
    vector<pair<int,int>> res;
    res.reserve(N);
    while ((int)res.size() < N) {
        int x = rand() % filas;
        int y = rand() % columnas;
        if (!usadas.count({x,y})) res.push_back({x,y});
    }
    return res;
}

// FUNCIÓN PARA MEDIR TIEMPO DE INSERCIÓN
// Inserta N pares (x,y) con valores aleatorios en una SparseMatrix nueva
double medir_add(int N, int filas, int columnas, int& densidadOut) {
    set<pair<int,int>> usadas;
    auto coords = generarCoordenadasUnicas(N, filas, columnas, usadas);
    SparseMatrix M;

    clock_t ini = clock();
    for (auto& c : coords) {
        int valor = 1 + (rand() % 100);  // simple
        M.add(valor, c.first, c.second);
    }
    clock_t fin = clock();
    densidadOut = M.density();
    return (double(fin - ini) * 1000.0) / CLOCKS_PER_SEC;
}

// FUNCIÓN PARA MEDIR TIEMPO DE OBTENCIÓN (GET) EN COORDENADAS EXISTENTES
// Consulta exactamente las mismas coordenadas que fueron insertadas
double medir_get_existentes(int N, int filas, int columnas) {
    set<pair<int,int>> usadas;
    auto coords = generarCoordenadasUnicas(N, filas, columnas, usadas);
    SparseMatrix M;
    for (auto& c : coords) M.add(1 + rand()%100, c.first, c.second);

    clock_t ini = clock();
    for (auto& c : coords) (void)M.get(c.first, c.second);
    clock_t fin = clock();
    return (double(fin - ini) * 1000.0) / CLOCKS_PER_SEC;
}

// FUNCIÓN PARA MEDIR TIEMPO DE OBTENCIÓN (GET) EN COORDENADAS INEXISTENTES
// Consulta N coordenadas que NO fueron insertadas (peor caso)
double medir_get_inexistentes(int N, int filas, int columnas) {
    set<pair<int,int>> usadas;
    auto coords = generarCoordenadasUnicas(N, filas, columnas, usadas);
    SparseMatrix M;
    for (auto& c : coords) M.add(1 + rand()%100, c.first, c.second);

    auto consultas = generarCoordenadasNoUsadas(N, filas, columnas, usadas);

    clock_t ini = clock();
    for (auto& c : consultas) (void)M.get(c.first, c.second);
    clock_t fin = clock();
    return (double(fin - ini) * 1000.0) / CLOCKS_PER_SEC;
}


// FUNCIÓN PARA MEDIR TIEMPO DE MULTIPLICACIÓN (ms)
// C = A * B
double medir_multiply(int N, int filasA, int columnasA, int columnasB) {
    // Densidad viene dada por N / (filas*cols). Aquí generamos N elementos en cada matriz.
    set<pair<int,int>> usadasA, usadasB;
    auto coordsA = generarCoordenadasUnicas(N, filasA, columnasA, usadasA);
    auto coordsB = generarCoordenadasUnicas(N, columnasA, columnasB, usadasB); // filasB = columnasA

    SparseMatrix A, B;
    for (auto& c : coordsA) A.add(1 + rand()%9, c.first, c.second);
    for (auto& c : coordsB) B.add(1 + rand()%9, c.first, c.second);

    clock_t ini = clock();
    SparseMatrix* C = A.multiply(&B);
    clock_t fin = clock();
    delete C;

    return (double(fin - ini) * 1000.0) / CLOCKS_PER_SEC;
}


int main() {
    srand((unsigned)time(nullptr));

    // Mostrar milisegundos con 3 decimales
    std::cout << std::fixed << std::setprecision(3);

    int tamanos[] = {50, 250, 500, 1000, 5000};

    //Una medicion por caso

    for (int N : tamanos) {
        // Densidad baja (<40%)
        int filas_baja = N * 2;
        int cols_baja  = N * 2;
        int densidad;
        std::cout << "[N=" << N << "] Densidad baja (<40%)\n";
        std::cout << "  ADD: " << medir_add(N, filas_baja, cols_baja, densidad)
          << " ms (densidad " << densidad << "%)\n";
        std::cout << "  GET existentes: "   << medir_get_existentes(N, filas_baja, cols_baja)<<" ms\n";
        std::cout << "  GET inexistentes: " << medir_get_inexistentes(N, filas_baja, cols_baja)<<" ms\n";
        // Multiplicación: B tiene filas = columnasA
        std::cout << "  MULTIPLY: "         << medir_multiply(N, filas_baja, cols_baja, cols_baja)<<" ms\n";

        // Densidad alta (>70%)
        int lado = (int)std::ceil(std::sqrt(double(N) / 0.75)); // asegura ≳75%
        if (lado < 1) lado = 1;
        std::cout << "[N=" << N << "] Densidad alta (>70%)\n";
        std::cout << "  ADD: " << medir_add(N, lado, lado, densidad)
          << " ms (densidad " << densidad << "%)\n";
        std::cout << "  GET existentes: "   << medir_get_existentes(N, lado, lado)<<" ms\n";
        std::cout << "  GET inexistentes: " << medir_get_inexistentes(N, lado, lado)<<" ms\n";
        std::cout << "  MULTIPLY: "         << medir_multiply(N, lado, lado, lado)<<" ms\n";

        std::cout << "--------------------------------------------------\n";
    }
    std::cout << "Listo.\n";
    return 0;
}
