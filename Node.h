#pragma once

class Node {
public:
    int xPos;
    int yPos;
    int value;
    Node* next;

    Node(int x, int y, int val) : xPos(x), yPos(y), value(val), next(nullptr) {}
};