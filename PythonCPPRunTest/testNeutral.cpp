#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Структура для представления ребер графа
struct Edge {
    int src, dest;
    float weight;
};

// Класс графа
class Graph {
public:
    int V; // Количество вершин vector<Edge> edges; // Ребра графа

    Graph(int V);
    void addEdge(int src, int dest, float weight);
    void bellmanFord(int src, vector<float>& distances);
};

// Конструктор графа
Graph::Graph(int V) {
    this->V = V;
}

// Метод для добавления ребра в граф
void Graph::addEdge(int src, int dest, float weight) {
    edges.push_back({src, dest, weight});
}

// Алгоритм Беллмана-Форда
void Graph::bellmanFord(int src, vector<float>& distances) {
    distances.assign(V, numeric_limits<float>::infinity());
    distances[src] = 0;

    for (int i = 1; i <= V - 1; ++i) {
        for (const auto& edge : edges) {
            if (distances[edge.src] != numeric_limits<float>::infinity() &&
                distances[edge.src] + edge.weight < distances[edge.dest]) {
                distances[edge.dest] = distances[edge.src] + edge.weight;
            }
        }
    }
}

// Нейросеть
class NeuralNetwork {
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize);
    void train(const vector<vector<float>>& inputs, const vector<float>& outputs, int epochs, float learningRate);
    vector<float> predict(const vector<float>& input);

private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    vector<vector<float>> weightsInputHidden;
    vector<vector<float>> weightsHiddenOutput;

    float sigmoid(float x);
    float sigmoidDerivative(float x);
};

// Конструктор нейросети
NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize)
    : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize) {
    weightsInputHidden.resize(inputSize, vector<float>(hiddenSize));
    weightsHiddenOutput.resize(hiddenSize, vector<float>(outputSize));

    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < inputSize; ++i) {
        for (int j = 0; j < hiddenSize; ++j) {
            weightsInputHidden[i][j] = static_cast<float>(rand()) / RAND_MAX; // Инициализация случайными значениями
        }
    }
    for (int i = 0; i < hiddenSize; ++i) {
        for (int j = 0; j < outputSize; ++j) {
            weightsHiddenOutput[i][j] = static_cast<float>(rand()) / RAND_MAX; // Инициализация случайными значениями
        }
    }
}

// Сигмоидальная функция
float NeuralNetwork::sigmoid(float x) {
    return 1.0f / (1.0f + exp(-x));
}

// Производная сигмоиды
float NeuralNetwork::sigmoidDerivative(float x) {
    return x * (1.0f - x);
}

// Обучение нейросети
void NeuralNetwork::train(const vector<vector<float>>& inputs, const vector<float>& outputs, int epochs, float learningRate) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        for (size_t i = 0; i < inputs.size(); ++i) {
            // Прямое распространение vector<float> inputLayer = inputs[i];
            vector<float> hiddenLayer(hiddenSize);
            vector<float> outputLayer(outputSize);

            // Входной слой в скрытый for (int j = 0; j < hiddenSize; ++j) {
                hiddenLayer[j] = 0.0f;
                for (int k = 0; k < inputSize; ++k) {
                    hiddenLayer[j] += inputLayer[k] * weightsInputHidden[k][j];
                }
                hiddenLayer[j] = sigmoid(hiddenLayer[j]);
            }

            // Скрытый слой в выходной for (int j = 0; j < outputSize; ++j) {
                outputLayer[j] = 0.0f;
                for (int k = 0; k < hiddenSize; ++k) {
                    outputLayer[j] += hiddenLayer[k] * weightsHiddenOutput[k][j];
                }
                outputLayer[j] = sigmoid(outputLayer[j]);
            }

            // Обратное распространение vector<float> outputErrors(outputSize);
            for (int j = 0; j < outputSize; ++j) {
                outputErrors[j] = outputs[i] - outputLayer[j];
            }

            // Обновление весов скрытого-выходного слоя
            for (int j = 0; j < hiddenSize; ++j) {
                for (int k = 0; k < outputSize; ++k) {
                    weightsHiddenOutput[j][k] += learningRate * outputErrors[k] * sigmoidDerivative(outputLayer[k]) * hiddenLayer[j];
                }
            }

            // Ошибки скрытого слоя
            vector<float> hiddenErrors(hiddenSize);
            for (int j = 0; j < hiddenSize; ++j) {
                hiddenErrors[j] = 0.0f;
                for (int k = 0; k < outputSize; ++k) {
                    hiddenErrors[j] += outputErrors[k] * weightsHiddenOutput[j][k];
                }
            }

            // Обновление весов входного-скрытого слоя
            for (int j = 0; j < inputSize; ++j) {
                for (int k = 0; k < hiddenSize; ++k) {
                    weightsInputHidden[j][k] += learningRate * hiddenErrors[k] * sigmoidDerivative(hiddenLayer[k]) * inputLayer[j];
                }
            }
        }
    }
}

// Предсказание выходного значения
vector<float> NeuralNetwork::predict(const vector<float>& input) {
    vector<float> hiddenLayer(hiddenSize);
    vector<float> outputLayer(outputSize);

    // Входной слой в скрытый for (int j = 0; j < hiddenSize; ++j) {
        hiddenLayer[j] = 0.0f;
        for (int k = 0; k < inputSize; ++k) {
            hiddenLayer[j] += input[k] * weightsInputHidden[k][j];
        }
        hiddenLayer[j] = sigmoid(hiddenLayer[j]);
    }

    // Скрытый слой в выходной for (int j = 0; j < outputSize; ++j) {
        outputLayer[j] = 0.0f;
        for (int k = 0; k < hiddenSize; ++k) {
            outputLayer[j] += hiddenLayer[k] * weightsHiddenOutput[k][j];
        }
        outputLayer[j] = sigmoid(outputLayer[j]);
    }

    return outputLayer;
}

int main() {
    // Создание графа
    Graph g(5);
    g.addEdge(0, 1, 10);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 2, 1);
    g.addEdge(1, 3, 2);
    g.addEdge(2, 1, 4);
    g.addEdge(2, 3, 8);
    g.addEdge(2, 4, 2);
    g.addEdge(3, 4, 7);
    g.addEdge(4, 3, 9);

    // Нахождение кратчайших расстояний
    vector<float> distances;
    g.bellmanFord(0, distances); // Начальная точка 0

    cout << "Кратчайшие расстояния от начальной точки 0:" << endl;
    for (int i = 0; i < g.V; ++i) {
        cout << "До " << i << ": " << distances[i] << endl;
    }

    // Подготовка данных для нейросети
    vector<vector<float>> inputs = {
        {0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}, {2, 4}, {3, 4}
    };
    vector<float> outputs = {
        10, 3, 1, 2, 8, 2, 7 // Соответствующие веса };

    // Создание и обучение нейросети NeuralNetwork nn(2, 3, 1); // 2 входа, 3 скрытых нейрона, 1 выход
    nn.train(inputs, outputs, 10000, 0.01f);

    // Тестирование нейросети
    vector<float> testInput = {0, 1};
    vector<float> prediction = nn.predict(testInput);
    cout << "Предсказанное расстояние от 0 до 1: " << prediction[0] << endl;

    return 0;
}
