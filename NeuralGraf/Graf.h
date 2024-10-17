#ifndef GRAF_H
#define GRAF_H
#include <iostream>
#include <vector>
#include <limits>
#include <stack>

struct Edge {
    int source;      // Исходная вершина (нейрон)
    int destination; // Конечная вершина (нейрон)
    float weight;    // Вес ребра (сила связи)
};


class Graph {
public:
    Graph(int vertices);
    void addEdge(int source, int destination, float weight);
    std::vector<float> bellmanFord(int start, std::vector<int>& predecessors);
private:
    int vertices;
    std::vector<Edge> edges;
};
#endif // GRAF_H