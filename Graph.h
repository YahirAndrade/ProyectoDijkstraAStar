#pragma once
#include "Node.h"
#include "Edge.h"
#include <vector>
#include <random>

class Graph {
public:
    std::vector<Node> nodes;
    std::vector<Edge> edges; // Lista de aristas para visualización

    // Generar grafo aleatorio
    void generateRandomGraph(int numNodes, float width, float height, float edgeProb = 0.3f) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> xDist(50, width - 50);
        std::uniform_real_distribution<float> yDist(50, height - 50);
        std::uniform_real_distribution<float> weightDist(1.0f, 10.0f);
        std::uniform_real_distribution<float> posDist(60, std::min(width, height) - 60);
        std::bernoulli_distribution edgeDist(edgeProb);

        const float minDistance = 80.0f;

        // Generar nodos con separación mínima
        nodes.clear();
        int attempts = 0;
        while ((int)nodes.size() < numNodes && attempts < numNodes * 100) {
            float x = xDist(gen);
            float y = yDist(gen);

            bool tooClose = false;
            for (const auto& node : nodes) {
                float dx = x - node.x;
                float dy = y - node.y;
                if (std::sqrt(dx * dx + dy * dy) < minDistance) {
                    tooClose = true;
                    break;
                }
            }

            if (!tooClose) {
                nodes.emplace_back(nodes.size(), x, y);
            }

            ++attempts;
        }

        // Generar aristas aleatorias
        edges.clear();
        for (int i = 0; i < (int)nodes.size(); ++i) {
            for (int j = i + 1; j < (int)nodes.size(); ++j) {
                if (edgeDist(gen)) {
                    float weight = weightDist(gen);
                    nodes[i].addNeighbor(j, weight);
                   //nodes[j].addNeighbor(i, weight);
                    edges.emplace_back(i, j, weight);
                }
            }
        }
    }


    // Métodos para acceder a nodos y aristas
    const std::vector<Node>& getNodes() const {
        return nodes; 
    }
    const std::vector<Edge>& getEdges() const {
        return edges;
    }
};