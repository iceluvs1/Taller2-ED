#include "SparseMatrix.h"
#include <iostream>
using namespace std;

SparseMatrix::SparseMatrix() {
    start = nullptr;
}

// FUNCIÓN PARA AGREGAR UN VALOR
void SparseMatrix::add(int value, int xPos, int yPos) {
    if (start == nullptr) {
        start = new Node(value, xPos, yPos);
        return;
    }
    Node* current = start;
    while (current->next != nullptr) {
        current = current->next;
    }
    // Agregar el nuevo nodo al final de la lista
    current->next = new Node(value, xPos, yPos);
}


// FUNCIÓN PARA OBTENER UN VALOR
int SparseMatrix::get(int xPos, int yPos) {
    Node* current = start;
    while (current != nullptr) {
        if (current->xPos == xPos && current->yPos == yPos) {
            return current->value;  // Si encuentra el nodo, retorna su valor
        }
        current = current->next;  // Si no encuentra continúa buscando
    }
    return 0;
}


// FUNCIÓN PARA ELIMINAR UN VALOR
void SparseMatrix::remove(int xPos, int yPos) {
    if (start == nullptr) {
        return;
    }
    // Cuando el nodo a eliminar es el primero
    if (start->xPos == xPos && start->yPos == yPos) {
        Node* temp = start;
        start = start->next;
        delete temp;  // Libera la memoria del nodo eliminado
        return;
    }
    // Si el nodo a eliminar no es el primero
    Node* current = start;
    while (current->next != nullptr) {
        if (current->next->xPos == xPos && current->next->yPos == yPos) {
            Node* temp = current->next;
            current->next = current->next->next;  // Enlaza el nodo anterior con el siguiente
            delete temp;
            return;
        }
        current = current->next;
    }
}


// FUNCIÓN PARA IMPRIMIR VALORES ALMACENADOS
void SparseMatrix::printStoredValues() {
    Node* current = start;
    while (current != nullptr) {
        cout << "(" << current->xPos << ", " << current->yPos << ") --> " << current->value << endl;
        current = current->next;  // Pasa al siguiente nodo
    }
}


// FUNCIÓN PARA CALCULAR DENSIDAD DE LA MATRIZ
int SparseMatrix::density() {
    int contador = 0;  // Contador de elementos no nulos
    int maxSize = 0;  // (determinado por la coordenada más alta)

    Node* current = start;
    while (current != nullptr) {
        contador++;
        if (current->xPos > maxSize) maxSize = current->xPos;
        if (current->yPos > maxSize) maxSize = current->yPos;
        current = current->next;
    }

    return (contador / (maxSize * maxSize));  // Densidad = elementos no nulos / tamaño máximo
}


// FUNCIÓN PARA MULTIPLICAR LAS MATRICES
SparseMatrix* SparseMatrix::multiply(SparseMatrix* second) {
    SparseMatrix* result = new SparseMatrix(); // Crear una nueva matriz vacía para almacenar el resultado de la multiplicación

    Node* current1 = start;

    while (current1 != nullptr) {
        Node* current2 = second->start; // Recorrer todos los nodos no nulos de la segunda matriz

        while (current2 != nullptr) {
            // Verificamos coordenadas
            if (current1->yPos == current2->xPos) {
                int newValue = current1->value * current2->value;
                result->add(newValue, current1->xPos, current2->yPos);
            }
            current2 = current2->next;
        }
        current1 = current1->next;
    }

    return result;
}


// DESTRUCTOR
SparseMatrix::~SparseMatrix() {
    Node* current = start;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}
