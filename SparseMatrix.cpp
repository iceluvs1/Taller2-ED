#include "SparseMatrix.h"
#include <iostream>
using namespace std;

SparseMatrix::SparseMatrix() {
    start = nullptr;
}

// FUNCIÓN PARA AGREGAR UN VALOR
void SparseMatrix::add(int value, int xPos, int yPos) {
    if (value == 0) {
        remove(xPos, yPos);
        return;
    }
    if (start == nullptr) {
        start = new Node(value, xPos, yPos);
        return;
    }

    Node* current = start;
    Node* previous = nullptr;

    // Buscar la posición correcta o si ya existe el nodo
    while (current != nullptr) {
        if (current->xPos == xPos && current->yPos == yPos) {
            // Si el nodo ya existe, actualizamos su valor
            current->value = value;
            return;
        }
        previous = current;
        current = current->next;
    }

    // Si no se encontró la posición, agregar el nuevo nodo
    Node* newNode = new Node(value, xPos, yPos);

    if (previous == nullptr) {
        // Si no hay nodos, agregamos al principio
        newNode->next = start;
        start = newNode;
    } else {
        // Insertamos entre previous y current
        previous->next = newNode;
        newNode->next = current;
    }
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
    if (start == nullptr) return 0;

    int conteoNoCero = 0;
    int maxX = 0, maxY = 0;

    Node* actual = start;
    while (actual != nullptr) {
        conteoNoCero++;
        if (actual->xPos > maxX) maxX = actual->xPos;
        if (actual->yPos > maxY) maxY = actual->yPos;
        actual = actual->next;
    }

    long long filas = static_cast<long long>(maxX) + 1;
    long long columnas = static_cast<long long>(maxY) + 1;
    long long area = filas * columnas;
    if (area <= 0) return 0;

    int porcentaje = static_cast<int>((conteoNoCero * 100LL) / area);
    return porcentaje;
}


// FUNCIÓN PARA MULTIPLICAR LAS MATRICES
SparseMatrix* SparseMatrix::multiply(SparseMatrix* second) {
    if (second == nullptr) return nullptr;

    SparseMatrix* resultado = new SparseMatrix();

    // Recorremos A
    for (Node* a = start; a != nullptr; a = a->next) {
        for (Node* b = second->start; b != nullptr; b = b->next) {
            if (b->xPos == a->yPos) {
                int i = a->xPos;
                int k = b->yPos;
                int valorParcial = a->value * b->value;

                // Acumular en C(i, k)
                int anterior = resultado->get(i, k);
                resultado->add(anterior + valorParcial, i, k);
            }
        }
    }

    return resultado;
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
