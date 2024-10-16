#include <iostream>
#include <vector>
#include <limits>
#include <stack>

using namespace std;

// Структура для представления рёбер графа
struct Edge {
    int source;      // Исходная вершина (нейрон)
    int destination; // Конечная вершина (нейрон)
    float weight;    // Вес ребра (сила связи)
};

// Класс для представления графа
class Graph {
public:
    Graph(int vertices);
    void addEdge(int source, int destination, float weight);
    vector<float> bellmanFord(int start, vector<int>& predecessors);

private:
    int vertices; // Количество нейронов (вершин)
    vector<Edge> edges; // Список рёбер
};

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
            cout << "Граф содержит отрицательный цикл." << endl;
            return {};
        }
        }
    }

    return distance; // Возвращаем массив расстояний
 }




// Функция для вывода пути
void printPath(int start, int end, const vector<int>& predecessors) {
    stack<int> path;
    for (int v = end; v != -1; v = predecessors[v]) {
        path.push(v);
    }

    cout << "Кратчайший путь от нейрона " << start << " до нейрона " << end << ": ";
    while (!path.empty()) {
        cout << path.top() << " ";
        path.pop();
    }
    cout << endl;
}

int main() {
    int numNeurons = 5; // Количество нейронов
    Graph graph(numNeurons);

    // Добавляем рёбра (нейрон1, нейрон2, вес)
    graph.addEdge(0, 1, 0.5); // Сила связи между нейронами 0 и 1
    graph.addEdge(0, 2, 2.0); // Сила связи между нейронами 0 и 2 graph.addEdge(1, 2, 1.0); // Сила связи между нейронами 1 и 2
    graph.addEdge(1, 3, 0.1); // Сила связи между нейронами 1 и 3 graph.addEdge(2, 1, 0.5); // Сила связи между нейронами 2 и 1
    graph.addEdge(2, 3, -2.5); // Сила связи между нейронами 2 и 3 graph.addEdge(2, 4, 1.0); // Сила связи между нейронами 2 и 4
    graph.addEdge(3, 4, 0.0); // Сила связи между нейронами 3 и 4 int startNeuron, endNeuron;

    cout << "Введите начальный нейрон (0-" << numNeurons - 1 << "): ";
    int startNeuron;
    cin >> startNeuron;
    cout << "Введите конечный нейрон (0-" << numNeurons - 1 << "): ";
    int endNeuron;
    cin >> endNeuron;

    vector<int> predecessors;
    vector<float> distances = graph.bellmanFord(startNeuron, predecessors);

    // Выводим результаты if (!distances.empty()) {
        cout << "Расстояние от нейрона " << startNeuron << " до нейрона " << endNeuron << ": ";
        if (distances[endNeuron] == numeric_limits<float>::max()) {
            cout << "Бесконечность" << endl;
        } else {
            cout << distances[endNeuron] << endl;
            printPath(startNeuron, endNeuron, predecessors); // Выводим путь
        }

   return 0;
    }






