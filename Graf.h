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
    vector<float> bellmanFord(int start, vector<int>& predecessors);

private:
    int vertices;
    vector<Edge> edges;
};

#endif // GRAF_H