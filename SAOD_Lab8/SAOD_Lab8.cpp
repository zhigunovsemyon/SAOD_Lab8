#include <iostream>
#include <vector>
#include <memory>
#include <numeric>

enum class NodeType {
    Leaf,
    Operation
};

struct Node {
    NodeType type;
    union {
        double value; // Для листа
        char operation; // Для операции
    };
    std::vector<std::shared_ptr<Node>> children; // Дети узла

    // Конструктор для листа
    Node(double val) : type(NodeType::Leaf), value(val) {}
    Node(int val) : type(NodeType::Leaf), value(static_cast<double>(val)) {}

    // Конструктор для операции
    Node(char op) : type(NodeType::Operation), operation(op) {}
};

// Функция для вычисления значения дерева
double evaluate(const std::shared_ptr<Node>& node) {
    if (node->type == NodeType::Leaf) {
        return node->value; // Если узел - лист, возвращаем его значение
    }

    // Если узел - операция, вычисляем значения всех детей
    std::vector<double> operands;
    for (const auto& child : node->children) {
        operands.push_back(evaluate(child));
    }

    // Выполняем операцию
    switch (node->operation) {
        case '+':
            return std::accumulate(operands.begin(), operands.end(), 0.0);
        case '-':
            return operands[0] - std::accumulate(operands.begin() + 1, operands.end(), 0.0);
        case '*':
            return std::accumulate(operands.begin(), operands.end(), 1.0, std::multiplies<double>());
        case '/':
            return operands[0] / std::accumulate(operands.begin() + 1, operands.end(), 1.0, std::multiplies<double>());
        default:
            throw std::invalid_argument("Unknown operation");
    }
}

int main() {
    // Пример создания дерева
    auto root = std::make_shared<Node>('+'); // Корень с операцией сложения
    auto leaf1 = std::make_shared<Node>(5);   // Лист с числом 5
    auto leaf2 = std::make_shared<Node>(3);   // Лист с числом 3
    auto leaf3 = std::make_shared<Node>(2);   // Лист с числом 2

    // Добавляем листья к корню
    root->children.push_back(leaf1);
    root->children.push_back(leaf2);
    root->children.push_back(leaf3);

    // Вычисляем значение дерева
    double result = evaluate(root);
    std::cout << "Result: " << result << std::endl; // Ожидаемое значение: 10

    return 0;
}
