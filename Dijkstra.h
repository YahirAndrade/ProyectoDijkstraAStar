#pragma once
#include "Graph.h"
#include <vector>
#include <queue>
#include <set>
#include <limits>
#include <utility>

class Dijkstra {
public:
    struct Step {
        int currentNode;
        std::vector<float> distances;
        std::vector<int> prev;
        std::set<int> visited;
        std::vector<int> frontier; // Nodos en la cola de prioridad
    };

    std::vector<Step> steps;

    void run(const Graph& graph, int start, int end) {
        steps.clear();
        size_t n = graph.getNodes().size();
        std::vector<float> distances(n, std::numeric_limits<float>::infinity());
        std::vector<int> prev(n, -1);
        std::set<int> visited;
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<>> pq;
        std::vector<int> frontier;

        distances[start] = 0;
        pq.emplace(0, start);
        frontier.push_back(start);

        while (!pq.empty()) {
            int u = pq.top().second;
            float dist = pq.top().first;
            pq.pop();
            frontier.erase(std::remove(frontier.begin(), frontier.end(), u), frontier.end());

            if (visited.count(u)) continue;
            visited.insert(u);

            steps.push_back({ u, distances, prev, visited, frontier });

            if (u == end) break;

            for (const auto& [v, weight] : graph.getNodes()[u].neighbors) {
                if (visited.count(v)) continue;
                float newDist = distances[u] + weight;
                if (newDist < distances[v]) {
                    distances[v] = newDist;
                    prev[v] = u;
                    pq.emplace(newDist, v);
                    if (std::find(frontier.begin(), frontier.end(), v) == frontier.end()) {
                        frontier.push_back(v);
                    }
                }
            }
        }
    }

    std::vector<int> getPath(int end, size_t stepIndex) const {
        std::vector<int> path;
        if (stepIndex >= steps.size()) return path;

        const auto& prev = steps[stepIndex].prev;

        if (prev[end] == -1) return path;  // no hay camino hasta end en este paso

        int current = end;
        while (current != -1) {
            path.push_back(current);
            current = prev[current];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
};