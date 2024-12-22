#include <iostream> /*std::ostream, std::cout*/
#include <vector>   /*std::vector*/
#include <memory>   /*std::shared_ptr*/
#include <numeric>  /*std::accumulate*/


struct Node {
	enum class NodeType {
		Leaf,
		Operation
	} type;

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
		case '+': //Допустимая операция
		case '-': //Допустимая операция
		case '*': //Допустимая операция
		case '/': //Допустимая операция
			return;
		default: //Недопустимая операция
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
		case '+': //Сложение всех элементов
			return std::accumulate(operands.begin(), operands.end(), 0.0);
		case '-': //Вычитание из первого элемента всех остальных (их суммы)
			return operands[0] - std::accumulate(operands.begin() + 1, operands.end(), 0.0);
		case '*': //Перемножение каждого элемента
			return std::accumulate(operands.begin(), operands.end(), 1.0, std::multiplies<double>());
		case '/': //Деление первого элемента на остальные (их произведение)
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
		Node root('+'); // Корень с операцией сложения
		
		// Добавляем листья к корню
		root.children.push_back(std::make_shared<Node>(5)); // Лист с числом 5
		root.children.push_back(std::make_shared<Node>(3)); // Лист с числом 3
		root.children.push_back(std::make_shared<Node>(2)); // Лист с числом 2
		
		// Вычисляем значение дерева
		std::cout << "Result: " << root << std::endl; // Ожидаемое значение: 10

		return EXIT_SUCCESS;
	}
	catch (std::invalid_argument arg) {
		std::cerr << arg.what() << std::endl;
		return EXIT_FAILURE;
	}
}
