#pragma once
#include <vector>
#include <utility>

class Node {
public:
    int id; // Identificador �nico del nodo
    float x, y; // Coordenadas para visualizaci�n
    std::vector<std::pair<int, float>> neighbors; // Lista de adyacencia: (ID del vecino, peso)

    // Constructor
    Node(int _id, float _x, float _y) : id(_id), x(_x), y(_y) {}

    // M�todo para agregar un vecino
    void addNeighbor(int neighborId, float weight) {
        neighbors.emplace_back(neighborId, weight);
    }
};