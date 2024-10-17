/*
#include "Graf.h"
#include "../Mediator.h"

// Конструктор
Graph::Graph(int vertices) : vertices(vertices) {}

// Метод для добавления рёбер
void Graph::addEdge(int source, int destination, float weight) {
    edges.push_back({source, destination, weight});
}

// Метод алгоритма Беллмана-Форда
vector<float> Graph::bellmanFord(int start, vector<int>& predecessors) {
    vector<float> distance(vertices, numeric_limits<float>::max());
    distance[start] = 0; // Расстояние до стартовой вершины равно 0
    predecessors.assign(vertices, -1); // Инициализация предшественников

    // Основной цикл алгоритма
    for (int i = 1; i < vertices; ++i) {
        for (const auto& edge : edges) {
            if (distance[edge.source] != numeric_limits<float>::max() &&
                distance[edge.source] + edge.weight < distance[edge.destination]) {
                distance[edge.destination] = distance[edge.source] + edge.weight;
                predecessors[edge.destination] = edge.source; // Запоминаем предшественника
            }
        // Проверка на наличие отрицательных циклов for (const auto& edge : edges) {
        if (distance[edge.source] != numeric_limits<float>::max() &&
            distance[edge.source] + edge.weight < distance[edge.destination]) {
            std::cout << "Граф содержит отрицательный цикл." << endl;
            return {};
        }
        }
    }
    return distance; // Возвращаем массив расстояний
 }*/