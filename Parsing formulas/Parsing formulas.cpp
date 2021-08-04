#include <iostream>
#include <string>

using namespace std;

//a function that checks if a character is a Latin letter
bool isLetter(char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
		return 1;
	return 0;
};
//a function that checks if a character is a digit
bool isNumeral(char c) {
	bool flag = true;
	if (c >= '0' && c <= '9')
		if (c == '.' && flag)
			flag = false;
		else
			return 1;
	return 0;
};
//a function that checks if a character is signed
int isSign(char c) {
	if ((c == '+') || (c == '-') || (c == '*'))
		return 1;
	return 0;
}
//expression validator (except for unknown characters and parenthesis mismatch)
bool isExpression(string expression) {
	for (int i = 1; i < expression.length(); i++) {
		if (
			isSign(expression[i]) && (isSign(expression[i - 1]) || expression[i - 1] == '(')
			||
			expression[i] == ')' && (isSign(expression[i - 1]) || expression[i - 1] == '(')
			||
			expression[i] == '(' && (!isSign(expression[i - 1]) && expression[i - 1] != '(')
			||
			(isNumeral(expression[i]) || isLetter(expression[i])) && expression[i - 1] == ')'
			)
			return 0;
	}
	if (isSign(expression[expression.length() - 1]) || isSign(expression[0]))
		return 0;
	return 1;
}

//stack
template <typename T>
class Stack {
	//structure defining a stack member
	template <typename T>
	struct StackElement {
		T Element;
		StackElement<T>* NextElement;
	};
	//stack head
	StackElement<T>* Head;
	//element for working with a stack
	StackElement<T>* CurrentElement;
	//stack size
	int StackSize;
	//recursive function designed to copy the elements of one stack to another
	void Copy() {
		if (CurrentElement != NULL) {
			T temp = CurrentElement->Element;
			CurrentElement = CurrentElement->NextElement;
			Copy();
			Add(temp);
		}
	}

public:
	//default stack constructor
	Stack() {
		Head = NULL;
		CurrentElement = NULL;
		StackSize = 0;
	}
	//stack copy constructor
	Stack(const Stack& s2) {
		Head = NULL;
		StackSize = 0;
		CurrentElement = s2.Head;
		Copy();
	}
	//stack destructor
	~Stack() {
		while (Head != NULL) {
			CurrentElement = Head;
			Head = Head->NextElement;
			delete CurrentElement;
			StackSize--;
		}
	}
	//adding an item to the stack
	bool Add(T element) {
		CurrentElement = new StackElement<T>;
		CurrentElement->Element = element;
		CurrentElement->NextElement = Head;
		Head = CurrentElement;
		StackSize++;
		return true;
	}
	//removing an item from to the stack
	bool Remove() {
		if (Head == NULL)
			return false;
		CurrentElement = Head;
		Head = Head->NextElement;
		delete CurrentElement;
		StackSize--;
		return true;
	}
	//searching for an item on the stack
	bool Find(T element) {
		CurrentElement = Head;
		while (CurrentElement != NULL) {
			if (CurrentElement->Element == element)
				return true;
			CurrentElement = CurrentElement->NextElement;
		}
		return false;
	}
	//outputting stack items to the console
	void Print() {
		CurrentElement = Head;
		while (CurrentElement != NULL) {
			cout << CurrentElement->Element << " ";
			CurrentElement = CurrentElement->NextElement;
		}
		cout << endl;
	}
	//function that returns the size of the stack
	int Size() {
		return StackSize;
	}
	//function that returns the top element of the stack
	T GetHead() {
		if (Head != NULL)
			return Head->Element;
		return 0;
	}
	//overloading the assignment operator for the stack
	Stack<T> operator = (Stack<T> s2)
	{
		while (Head != NULL)
			Remove();
		CurrentElement = s2.Head;
		Copy();
		return *this;
	}
};
//binary tree
template <typename T>
class BinaryTree 
{
protected:
	//structure defining a tree element
	struct TreeNode 
	{
		//tree node constructor
		TreeNode(T item, TreeNode* l = NULL, TreeNode* r = NULL) {
			Field = item;
			Left = l;
			Right = r;
			Status = 0;
		}
		T Field;
		TreeNode* Left;
		TreeNode* Right;
		int Status;
	};
	//tree root
	TreeNode* Root;
	//element for working with wood
	TreeNode* CurrentNode;
	int NumberOfNodes;
	//recursive function for console output
	void PrintTree() 
	{
		TreeNode* node = CurrentNode;
		if (node != NULL) 
		{
			CurrentNode = node->Left;
			PrintTree();
			cout << node->Field << " ";
			CurrentNode = node->Right;
			PrintTree();
		}
	}
	//recursive function to delete
	void DeleteTree() 
	{
		TreeNode* node = CurrentNode;
		if (node != NULL) 
		{
			CurrentNode = node->Left;
			DeleteTree();
			CurrentNode = node->Right;
			DeleteTree();
			delete node;
			NumberOfNodes--;
		}
	}
public:
	//default constructor
	BinaryTree() 
	{
		Root = NULL;
		CurrentNode = NULL;
		NumberOfNodes = 0;
	}
	//destructor
	~BinaryTree() 
	{
		CurrentNode = Root;
		DeleteTree();
	}
	//console output
	void Print() 
	{
		CurrentNode = Root;
		PrintTree();
		cout << endl;
	}
	//function that returns the number of nodes
	int Size() 
	{
		return NumberOfNodes;
	}
};
//tree-formula
class TreeFormula : public BinaryTree<string> 
{
	//function that generates a tree-formula from the reverse Polish notation
	TreeNode* Add(string item)
	{
		TreeNode* node = CurrentNode;
		//create a node, exit recursion
		if (node == NULL) 
		{
			node = new TreeNode(item);
			if (Root == NULL)
				Root = node;
			if (!isSign(item[0]))
				node->Status = 2;
			return node;
		}
		//if there is no right child or the element is not a number and an element can be added
		//to the branch of the right child, then we add it to the right, otherwise to the left
		if (node->Right == NULL || isSign(node->Right->Field[0]) && node->Right->Status < 2)
		{
			CurrentNode = node->Right;
			node->Right = Add(item);
		}
		else
		{
			CurrentNode = node->Left;
			node->Left = Add(item);
		}
		//change the status of the current item
		if (node->Left != NULL && node->Right != NULL && node->Left->Status == 2 && node->Right->Status == 2)
			node->Status = 2;
		else
			node->Status = 1;
		return node;
	}
	int CalcTree() 
	{
		int right, left;
		TreeNode* node = CurrentNode;
		if (node->Left == NULL)
			return stoi(node->Field);
		CurrentNode = node->Left;
		left = CalcTree();
		CurrentNode = node->Right;
		right = CalcTree();
		switch (node->Field[0])
		{
		case '+':
			return left + right;
		case '-':
			return left - right;
		case '*':
			return left * right;
		}

	}
	int stoi(string s)
	{
		int number = 0;
		for (auto symbol : s)
			if (symbol >= '0' && symbol <= '9')
				number = number * 10 + (symbol - '0');
			else
				throw "The tree is not a numeric expression, no calculations can be made.";
		return number;
	}
	
public:
	void Create(Stack<string> expression) {
		CurrentNode = Root;
		DeleteTree();
		Root = 0;
		while (expression.Size() != 0) {
			CurrentNode = Root;
			Add(expression.GetHead());
			expression.Remove();
		}
	}
	int Calculate()
	{
		CurrentNode = Root;
		return CalcTree();
	}
};

//a function that converts from standard expression notation to Polish reverse notation
Stack<string> ExpressionToRPN(string expression) {
	//string for storing the operands of the expression
	string operand = "";
	//stack in which the operations are stored
	Stack<char> operations;
	//stack to which the reverse polish write is written
	Stack<string> result;
	for (int i = 0; i < expression.length(); i++)
	{
		if (expression[i] == ' ') {
			expression.erase(i, 1);
			i--;
		}
	}
	if (!isExpression(expression))
		throw "the formula is typed incorrectly! Extra signs are encountered.";
	for (auto symbol : expression) {
		if (isLetter(symbol) || isNumeral(symbol))
			operand += symbol;
		else {
			if (operand != "") 
			{
				result.Add(operand);
				operand = "";
			}
			switch (symbol)
			{
			case '+':
			case '-':
				while (operations.Size() != 0 && operations.GetHead() != '(') 
				{
					result.Add(string(1, operations.GetHead()));
					operations.Remove();

				}
				break;
			case '*':
				while (operations.Size() != 0 && operations.GetHead() == '*') 
				{
					result.Add(string(1, operations.GetHead()));
					operations.Remove();
				}
				break;
			case ')':
				while (operations.Size() != 0 && operations.GetHead() != '(') 
				{
					result.Add(string(1, operations.GetHead()));
					operations.Remove();
				}
				if (operations.Size() == 0)
					throw "The formula is typed incorrectly! Open and close parentheses mismatch.";
				else
					operations.Remove();
				break;
			case '(':
				break;
			default:
				if (symbol != ' ')
					throw "The formula is typed incorrectly! An unknown symbol was encountered.";
				break;
			}
			if (symbol != ')')
				operations.Add(symbol);
		}
	}
	if (operand != "")
		result.Add(operand);
	while (operations.Size() != 0) {
		if (operations.GetHead() == '(')
			throw "The formula is typed incorrectly! Open and close parentheses mismatch.";
		result.Add(string(1, operations.GetHead()));
		operations.Remove();
	}
	return result;
}

int main() 
{
	Stack<string> b;
	string expression;
	TreeFormula a;

	while (true) 
	{
		cout << "Enter a formula or \"exit\" to exit." << endl;
		getline(cin, expression);
		if (expression == "exit")
			break;
		try
		{
			b = ExpressionToRPN(expression);
		}
		catch (const char* a)
		{
			cout << a << endl;
			continue;
		}
		a.Create(b);
		b.Print();
		a.Print();
		try 
		{
			cout << a.Calculate() << endl;
		}
		catch (const char* a)
		{
			cout << a << endl;
			continue;
		}
	}
	return 0;
}

/*
Имеем формулу:
(a+b)*c-d
Переводим в вид удобный для обработки - обратная польская запись
1. (
2. (	a
3. (+	a
4. (+	ab
5.		ab+
6. *	ab+
7. *	ab+c
8. -	ab+c*
9. -	db+c*d
10.		ab+c*d-
Рекурсивно сторим дерево
	   -
	  / \
	 *  d
	/ \
   +   c
  / \
 a   b

 Имеем формулу:
a+(b-c)*d-e
Переводим в вид удобный для обработки - обратная польская запись
1.		a
2. +	a
3. +(	ab
4. +(-	ab
5. +(-) abc-
6. +*	abc-
7. +	abc-d*+
8. -	abc-d*+e-

*/