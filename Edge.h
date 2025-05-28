#pragma once
#include"Node.h"

#pragma once

class Edge {
public:
    int source; // ID del nodo origen
    int destination; // ID del nodo destino
    float weight; // Peso de la arista

    // Constructor
    Edge(int _source, int _destination, float _weight)
        : source(_source), destination(_destination), weight(_weight) {}

    // Métodos para acceder a los datos
    int getSource() const { return source; }
    int getDestination() const { return destination; }
    float getWeight() const { return weight; }
};