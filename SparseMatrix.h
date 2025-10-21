#pragma once
#include "Node.h"

class SparseMatrix {
private:
    Node* start;

public:
    SparseMatrix();

    void add(int value, int xPos, int yPos); // Agrega el avlor
    int get(int xPos, int yPos); // Obtiene el valor
    void remove(int xPos, int yPos); // Elimina un valor
    void printStoredValues(); // Imprime valores almacenados
    int density();          // Obtiene la densidad de la matriz
    SparseMatrix* multiply(SparseMatrix* second);

    ~SparseMatrix();
};