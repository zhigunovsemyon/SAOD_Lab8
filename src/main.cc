﻿#include <iostream> /*std::ostream, std::cout*/
#include <memory>   /*std::shared_ptr*/
#include <numeric>  /*std::accumulate*/
#include <vector>   /*std::vector*/

struct Node {
	enum class NodeType {
		Leaf,
		Operation
	} type;

	union {
		double value;	// Для листа
		char operation; // Для операции
	};

	std::vector<std::shared_ptr<Node>> children; // Дети узла

	// Конструктор для листа
	Node(double val) : type(NodeType::Leaf), value(val) {}

	Node(int val) : type(NodeType::Leaf), value(static_cast<double>(val)) {}

	// Конструктор для операции
	Node(char op) : type(NodeType::Operation), operation(op)
	{
		switch (op) {
		// Допустимая операция
		case '+':
		case '-':
		case '*':
		case '/':
			return;
		default: // Недопустимая операция
			throw std::invalid_argument(
				std::string{"Unknown operation: "} + op);
		}
	}

	// Функция для вычисления значения дерева
	double evaluate() const;

	friend std::ostream & operator<<(std::ostream & ost, Node const & tree)
	{
		return ost << tree.evaluate(); // Возвращает ost
	}
};

// Функция для вычисления значения дерева
double Node::evaluate() const
{
	// Если узел - лист, возвращаем его значение
	if (this->type == NodeType::Leaf) {
		return this->value;
	}

	// Если узел - операция, вычисляем значения всех детей
	std::vector<double> operands;
	for (auto const & child : this->children) {
		operands.push_back(child->evaluate());
	}

	// Выполняем операцию
	switch (this->operation) {
	case '+': // Сложение всех элементов
		return std::accumulate(operands.begin(), operands.end(), 0.0);
	case '-': /*Вычитание из первого элемента всех остальных
		(их суммы) */
		return operands[0] - std::accumulate(operands.begin() + 1,
						     operands.end(), 0.0);
	case '*': // Перемножение каждого элемента
		return std::accumulate(operands.begin(), operands.end(), 1.0,
				       std::multiplies<double>());
	case '/': /*Деление первого элемента на остальные
		(их произведение) */
		return operands[0] / std::accumulate(operands.begin() + 1,
						     operands.end(), 1.0,
						     std::multiplies<double>());
	default:
		throw std::invalid_argument(std::string{"Unknown operation: "} +
					    operation);
	}
}

int main()
{
	try {
		Node root('+'); // Корень с операцией сложения

		// Добавляем листья к корню
		root.children.push_back(
			std::make_shared<Node>(5)); // Лист с числом 5
		root.children.push_back(
			std::make_shared<Node>(3)); // Лист с числом 3
		root.children.push_back(
			std::make_shared<Node>('*')); // Лист с числом 2

		root.children.back()->children.push_back(
			std::make_shared<Node>(2));
		root.children.back()->children.push_back(
			std::make_shared<Node>(2));
		root.children.back()->children.push_back(
			std::make_shared<Node>(2));
		root.children.back()->children.push_back(
			std::make_shared<Node>(2));

		// Вычисляем значение дерева
		std::cout << "Result: " << root
			  << std::endl; // Ожидаемое значение: 10

		return EXIT_SUCCESS;
	} catch (std::invalid_argument arg) {
		std::cerr << arg.what() << std::endl;
		return EXIT_FAILURE;
	}
}
