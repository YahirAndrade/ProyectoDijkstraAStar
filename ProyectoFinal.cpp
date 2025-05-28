#include "Node.h"
#include "Edge.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "AStar.h"
#include "Visualizer.h"

int main() {
    Graph graph;
    graph.generateRandomGraph(10, 800, 600);

    Dijkstra dijkstra;
    AStar astar;

    dijkstra.run(graph, 0, 9);
    astar.run(graph, 0, 9);

    Visualizer vis(graph, dijkstra, astar, 800, 600);
    vis.run();

    return 0;
}
/*
*     Práctica Final  -> 29/05/25
 1. Visualización de un grafo
    -No manejaremos pesos negativos           *Crear grafo aleatoriamente
 2. Algoritmos de búsqueda                     min 10 nodos (0...N-1)
    - Dijkstra
    - A*                                      *Todo debe ir paso por paso
 3. Reporte -> 28/05/25
                11:59pm
*/
