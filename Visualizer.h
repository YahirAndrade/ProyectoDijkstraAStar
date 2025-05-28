#pragma once
#include <raylib.h>32D HN
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "Graph.h"
#include "Dijkstra.h"
#include "AStar.h"

class Visualizer {
public:
    Graph& graph;
    Dijkstra& dijkstra;
    AStar& aStar;
    size_t stepIndex;
    bool showDijkstra;
    float scale;

    Visualizer(Graph& g, Dijkstra& d, AStar& a, unsigned int width, unsigned int height)
        : graph(g), dijkstra(d), aStar(a), stepIndex(0), showDijkstra(true), scale(1.2f) {
        InitWindow(width, height, "Vizualización de los algoritmos");
        SetTargetFPS(60);
    }

    ~Visualizer() {
        CloseWindow();
    }

    void run() {
        while (!WindowShouldClose()) {
            handleEvents();
            render();
        }
    }

private:
    void handleEvents() {
        if (IsKeyPressed(KEY_RIGHT)) {
            size_t maxSteps = showDijkstra ? dijkstra.steps.size() : aStar.steps.size();
            if (stepIndex < maxSteps) ++stepIndex;
        }
        else if (IsKeyPressed(KEY_LEFT) && stepIndex > 0) {
            --stepIndex;
        }
        else if (IsKeyPressed(KEY_D)) {
            showDijkstra = true;
            stepIndex = 0;
        }
        else if (IsKeyPressed(KEY_A)) {
            showDijkstra = false;
            stepIndex = 0;
        }
        else if (IsKeyPressed(KEY_M)) {
            scale *= 1.5f;
        }
        else if (IsKeyPressed(KEY_N)) {
            scale /= 1.5f;
        }
        else if (IsKeyPressed(KEY_R)) {
            graph.generateRandomGraph(10, 800, 600);
            dijkstra.run(graph, 0, 9);
            aStar.run(graph, 0, 9);
            stepIndex = 0;
        }
    }

    void render() {
        BeginDrawing();
        ClearBackground(WHITE);

        // Instrucciones y estado
        std::string algo = showDijkstra ? "Dijkstra" : "A*";
        std::string status = "Step " + std::to_string(stepIndex) + "/" +
            std::to_string(showDijkstra ? dijkstra.steps.size() - 1 : aStar.steps.size() - 1) +
            " (" + algo + ")";
        if (showDijkstra && dijkstra.steps.empty()) {
            DrawText("Error: No Dijkstra steps!", 10, 70, 25, RED);
        }
        else if (!showDijkstra && aStar.steps.empty()) {
            DrawText("Error: No A* steps!", 10, 70, 25, RED);
        }
        DrawText("D: Dijkstra, A: A*, R: Regenerate, M/N: Zoom, Left/Right: Steps", 10, 10, 25, BLACK);
        DrawText(status.c_str(), 10, 40, 25, BLACK);

        // Dibujar aristas
        for (const auto& edge : graph.getEdges()) {
            const Node& src = graph.getNodes()[edge.getSource()];
            const Node& dst = graph.getNodes()[edge.getDestination()];
            bool explored = false;
            if (showDijkstra && stepIndex < dijkstra.steps.size()) {
                const auto& step = dijkstra.steps[stepIndex];
                if (step.currentNode == edge.getSource() || step.currentNode == edge.getDestination()) {
                    explored = true;
                }
            }
            else if (!showDijkstra && stepIndex < aStar.steps.size()) {
                const auto& step = aStar.steps[stepIndex];
                if (step.currentNode == edge.getSource() || step.currentNode == edge.getDestination()) {
                    explored = true;
                }
            }
            DrawLineEx({ src.x * scale, src.y * scale }, { dst.x * scale, dst.y * scale }, 2.0f * scale, explored ? GREEN : BLACK);
            DrawText(std::to_string(static_cast<int>(edge.getWeight())).c_str(),
                ((src.x + dst.x) / 2 - 10) * scale, ((src.y + dst.y) / 2 - 10) * scale, 20 * scale, BLACK);
        }

        // Dibujar nodos y estado
        if (showDijkstra && !dijkstra.steps.empty() && stepIndex < dijkstra.steps.size()) {
            const auto& step = dijkstra.steps[stepIndex];
            drawState(step.visited, step.currentNode, step.frontier, step.distances);
        }
        else if (!showDijkstra && !aStar.steps.empty() && stepIndex < aStar.steps.size()) {
            const auto& step = aStar.steps[stepIndex];
            drawState(step.visited, step.currentNode, step.frontier, step.gScores);
        }
        else {
            drawState({}, -1, {}, std::vector<float>(graph.getNodes().size(), 0.0f));
        }

        // Dibujar camino más corto
        int endNode = graph.getNodes().size() - 1;
        std::vector<int> path = showDijkstra ? dijkstra.getPath(endNode, stepIndex) : aStar.getPath(endNode, stepIndex);
        if (!path.empty()) {
            for (size_t i = 1; i < path.size(); ++i) {
                const Node& src = graph.getNodes()[path[i - 1]];
                const Node& dst = graph.getNodes()[path[i]];
                DrawLineEx({ src.x * scale, src.y * scale }, { dst.x * scale, dst.y * scale }, 3.0f * scale, RED);
            }
        }

        EndDrawing();
    }

    void drawState(const std::set<int>& visited, int current, const std::vector<int>& frontier, const std::vector<float>& scores) {
        for (size_t i = 0; i < graph.getNodes().size(); ++i) {
            const Node& node = graph.getNodes()[i];
            Color color;
            if (static_cast<int>(i) == current) {
                color = RED; // Nodo actual
            }
            else if (std::find(frontier.begin(), frontier.end(), i) != frontier.end()) {
                color = YELLOW; // Nodo en la frontera
            }
            else if (visited.count(i)) {
                color = GREEN; // Nodo visitado
            }
            else {
                color = BLUE; // Nodo no visitado
            }
            DrawCircle(node.x * scale, node.y * scale, 20 * scale, color);
            DrawText(std::to_string(node.id).c_str(), (node.x - 5) * scale, (node.y - 25) * scale, 18 * scale, BLACK);

            // Mostrar distancia/puntaje
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1) << scores[i];
            std::string scoreStr = (scores[i] == std::numeric_limits<float>::infinity()) ? "n/a" : ss.str();
            DrawText(scoreStr.c_str(), (node.x - 15) * scale, (node.y + 25) * scale, 18 * scale, GRAY);

        }
    }
};