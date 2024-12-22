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
	Node(char op) : type(NodeType::Operation), operation(op) {
		switch (op)
		{
		case '+':
		case '-':
		case '*':
		case '/':
			return;
		default:
			throw std::invalid_argument(std::string{ "Unknown operation: " } + op);
		}
	}

	// Функция для вычисления значения дерева
	double evaluate() const {
		if (this->type == NodeType::Leaf) {
			return this->value; // Если узел - лист, возвращаем его значение
		}

		// Если узел - операция, вычисляем значения всех детей
		std::vector<double> operands;
		for (const auto& child : this->children) {
			operands.push_back(child->evaluate());
		}

		// Выполняем операцию
		switch (this->operation) {
		case '+':
			return std::accumulate(operands.begin(), operands.end(), 0.0);
		case '-':
			return operands[0] - std::accumulate(operands.begin() + 1, operands.end(), 0.0);
		case '*':
			return std::accumulate(operands.begin(), operands.end(), 1.0, std::multiplies<double>());
		case '/':
			return operands[0] / std::accumulate(operands.begin() + 1, operands.end(), 1.0, std::multiplies<double>());
		}
	}

	friend std::ostream & operator<<(std::ostream & ost, Node const& tree) 
	{
		ost << tree.evaluate();
		return ost;
	}
};


int main() {
	try {
		// Пример создания дерева
		Node root('+'); // Корень с операцией сложения
		auto leaf1 = std::make_shared<Node>(5);   // Лист с числом 5
		auto leaf2 = std::make_shared<Node>(3);   // Лист с числом 3
		auto leaf3 = std::make_shared<Node>(2);   // Лист с числом 2
		// Добавляем листья к корню
		root.children.push_back(leaf1);
		root.children.push_back(leaf2);
		root.children.push_back(leaf3);

		// Вычисляем значение дерева
		std::cout << "Result: " << root << std::endl; // Ожидаемое значение: 10

		return 0;
	}
	catch (std::invalid_argument arg) {
		std::cerr << arg.what();
		return EXIT_FAILURE;
	}
}
