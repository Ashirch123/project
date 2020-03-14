#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
using namespace std;

struct Node
{
	int x, y, pathCost, prevX, prevY, depth;
	char obstacle;
	bool visited;
	bool explored;


	Node()
	{
		x = -1;
		y = -1;
		prevX = -1;
		prevY = -1;
		visited = false;
		explored = false;
		pathCost = 0;
		depth = INT_MAX;
	}

};
int row, col;
int startRow, startCol;
int goalRow, goalCol;
Node** grid = NULL;

struct Queue
{
	Node* node[140];
	int front = -1;
	int rear = -1;
	int numItems;

	Queue()
	{
		for (int i = 0; i < 140; i++)
		{
			node[i] = NULL;
		}

		front = -1;
		rear = -1;
		numItems = 0;
	}

	bool IsFull()
	{
		if (numItems < 140)
			return false;

		else
			return true;
	}

	bool IsEmpty()
	{
		if (numItems)
			return false;

		else
			return true;
	}

	void Enqueue(Node* n)
	{
		if (IsFull())
		{
			cout << "The queue is full.\n";
			return;
		}
		else
		{
			rear = (rear + 1) % 140;
			node[rear] = n;
			numItems++;
		}
	}

	Node* Dequeue()
	{
		if (IsEmpty())
		{
			cout << "The queue is empty.\n";
			return NULL;
		}

		else
		{
			front = (front + 1) % 140;
			Node* n = node[front];
			numItems--;
			return n;
		}
	}
};

struct PriorityQueue
{
	Node* node[140];
	int front = -1;

	PriorityQueue()
	{
		for (int i = 0; i < 140; i++)
		{
			node[i] = NULL;
		}

		front = -1;
	}

	void Enqueue(Node* n)
	{
		front = (front + 1) % 140;
		node[front] = n;
	}

	Node* Dequeue()
	{
		Node* temp = node[0];
		int minIndex = 0;
		for (int i = 0; i < 140; i++)
		{
			if (node[i] == NULL)
				break;

			if (temp->pathCost > node[i]->pathCost)
			{
				temp = node[i];
				minIndex = i;
			}
		}

		for (int i = minIndex; i < 139; i++)
		{
			node[i] = node[i + 1];
		}
		front--;
		return temp;
	}
};

struct Stack
{
	Node* node[140];
	int current = -1;

	Stack()
	{
		for (int i = 0; i < 140; i++)
		{
			node[i] = NULL;
		}
	}

	bool IsEmpty()
	{
		if (current < 0)
			return true;

		else
			return false;
	}

	void Push(Node* n)
	{
		current++;
		node[current] = n;
	}

	Node* Pop()
	{
		int index = current;
		current--;
		return node[index];
	}

};

void showMenu()
{
	cout << "Press:\n";
	cout << "0: Exit\n";
	cout << "1: Breadth First Search\n";
	cout << "2: Uniform Cost Search\n";
	cout << "3: Depth First Search\n";
	cout << "4: Iterative Deepening Search\n";
	cout << "5: Depth Limited Search\n";
	cout << "6: Bidirectional Search\n= ";
}

bool readFile()
{
	ifstream in;
	in.open("grid.txt");
	if (!in)
	{
		return false;
	}
	in >> col;
	in >> row;
	in >> startCol;
	in >> startRow;
	in >> goalCol;
	in >> goalRow;

	startRow = row - startRow;
	goalRow = row - goalRow - 1;

	grid = new Node*[row];
	for (int i = 0; i < row; i++)
	{
		grid[i] = new Node[col];
	}

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			in >> grid[i][j].obstacle;
			grid[i][j].x = i;
			grid[i][j].y = j;
		}
	}

	return true;
}

void bfsAlgorithm()
{
	Node** bfsGrid = new Node*[row];

	for (int i = 0; i < row; i++)
		bfsGrid[i] = new Node[col];

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			bfsGrid[i][j] = grid[i][j];
	}

	cout << "\t\t\t\t\tBreadth First Search\n";
	Queue queue;
	queue.Enqueue(&bfsGrid[startRow][startCol]);
	int x;
	int y;
	while (true)
	{
		Node* current = queue.Dequeue();
		current->visited = true;
		current->explored = true;
		x = current->x;
		y = current->y;

		if (x == goalRow && y == goalCol)
		{
			cout << "Goal Found" << endl;
			break;
		}

		if (x - 1 >= 0 && bfsGrid[x - 1][y].obstacle != '1' && bfsGrid[x - 1][y].visited == false) //UP
		{
			bfsGrid[x - 1][y].visited = true;
			bfsGrid[x - 1][y].prevX = x;
			bfsGrid[x - 1][y].prevY = y;
			bfsGrid[x - 1][y].pathCost = current->pathCost + 1;
			queue.Enqueue(&bfsGrid[x - 1][y]);
		}


		if (x - 1 >= 0 && y + 1 < col && bfsGrid[x - 1][y + 1].obstacle != '1' && bfsGrid[x - 1][y + 1].visited == false) //Diagonal
		{
			bfsGrid[x - 1][y + 1].visited = true;
			bfsGrid[x - 1][y + 1].prevX = x;
			bfsGrid[x - 1][y + 1].prevY = y;
			bfsGrid[x - 1][y + 1].pathCost = current->pathCost + 2;
			queue.Enqueue(&bfsGrid[x - 1][y + 1]);
		}

		if (y + 1 < col && bfsGrid[x][y + 1].obstacle != '1' && bfsGrid[x][y + 1].visited == false) //Right
		{
			bfsGrid[x][y + 1].visited = true;
			bfsGrid[x][y + 1].prevX = x;
			bfsGrid[x][y + 1].prevY = y;
			bfsGrid[x][y + 1].pathCost = current->pathCost + 3;
			queue.Enqueue(&bfsGrid[x][y + 1]);
		}

	}

	Node* goal;

	while (true)	//Backtracking from goal to start
	{
		goal = &bfsGrid[x][y];
		goal->obstacle = '#';
		if (x == startRow && y == startCol)
			break;
		x = goal->prevX;
		y = goal->prevY;
	}

	bfsGrid[startRow][startCol].obstacle = 'S';
	bfsGrid[goalRow][goalCol].obstacle = 'G';

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (bfsGrid[i][j].obstacle == '#' || bfsGrid[i][j].obstacle == 'G' || bfsGrid[i][j].obstacle == 'S')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			if (bfsGrid[i][j].obstacle == 'O')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

			cout << bfsGrid[i][j].obstacle << "\t";
		}
		cout << endl;
	}

	cout << "Path Cost = " << bfsGrid[goalRow][goalCol].pathCost << endl;
	system("pause>0");
	system("cls");
}

void dfsAlgorithm()
{
	Node** dfsGrid = new Node*[row];
	for (int i = 0; i < row; i++)
		dfsGrid[i] = new Node[col];

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			dfsGrid[i][j] = grid[i][j];
	}


	cout << "\t\t\t\t\tDepth First Search\n";
	Stack stack;
	stack.Push(&dfsGrid[startRow][startCol]);
	int x;
	int y;
	while (true)
	{
		Node* current = stack.Pop();
		current->visited = true;
		current->explored = true;
		x = current->x;
		y = current->y;

		if (x == goalRow && y == goalCol)
		{
			cout << "Goal Found" << endl;
			break;
		}

		//Up
		if (x - 1 >= 0 && dfsGrid[x - 1][y].obstacle != '1' && dfsGrid[x - 1][y].visited == false)
		{
			dfsGrid[x - 1][y].visited = true;
			dfsGrid[x - 1][y].prevX = x;
			dfsGrid[x - 1][y].prevY = y;
			dfsGrid[x - 1][y].pathCost = current->pathCost + 1;
			stack.Push(&dfsGrid[x - 1][y]);
		}

		//Diagonal
		if (x - 1 >= 0 && y + 1 < col && dfsGrid[x - 1][y + 1].obstacle != '1' && dfsGrid[x - 1][y + 1].visited == false)
		{
			dfsGrid[x - 1][y + 1].visited = true;
			dfsGrid[x - 1][y + 1].prevX = x;
			dfsGrid[x - 1][y + 1].prevY = y;
			dfsGrid[x - 1][y + 1].pathCost = current->pathCost + 2;
			stack.Push(&dfsGrid[x - 1][y + 1]);
		}
		//Right
		if (y + 1 < col && dfsGrid[x][y + 1].obstacle != '1' && dfsGrid[x][y + 1].visited == false)
		{
			dfsGrid[x][y + 1].visited = true;
			dfsGrid[x][y + 1].prevX = x;
			dfsGrid[x][y + 1].prevY = y;
			dfsGrid[x][y + 1].pathCost = current->pathCost + 3;
			stack.Push(&dfsGrid[x][y + 1]);
		}
	}

	Node* goal;

	while (true)	//Backtracking from goal to start
	{
		goal = &dfsGrid[x][y];
		goal->obstacle = '#';
		if (x == startRow && y == startCol)
			break;
		x = goal->prevX;
		y = goal->prevY;
	}

	dfsGrid[startRow][startCol].obstacle = 'S';
	dfsGrid[goalRow][goalCol].obstacle = 'G';
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (dfsGrid[i][j].obstacle == '#' || dfsGrid[i][j].obstacle == 'G' || dfsGrid[i][j].obstacle == 'S')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			if (dfsGrid[i][j].obstacle == 'O')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

			cout << dfsGrid[i][j].obstacle << "\t";
		}
		cout << endl;
	}
	cout << "Path Cost = " << dfsGrid[goalRow][goalCol].pathCost << endl;
	system("pause>0");
	system("cls");

}

void dlsAlgorithm()
{
	Node** dlsGrid = new Node*[row];
	for (int i = 0; i < row; i++)
		dlsGrid[i] = new Node[col];

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			dlsGrid[i][j] = grid[i][j];
	}


	system("cls");
	Stack stack;
	dlsGrid[startRow][startCol].depth = 0;
	stack.Push(&dlsGrid[startRow][startCol]);
	int x;
	int y;
	int limit;
	cout << "Enter depth limit: ";
	cin >> limit;
	cout << "\t\t\t\t\tDepth Limited Search\n";
	cout << "Limit=" << limit << endl;
	while (true)
	{
		Node* current = stack.Pop();
		current->visited = true;
		current->explored = true;
		x = current->x;
		y = current->y;

		if (x == goalRow && y == goalCol)
		{
			cout << "Goal Found" << endl;
			break;
		}

		if (current->depth < limit)
		{

			if (y + 1 < col && dlsGrid[x][y + 1].obstacle != '1'/* && dlsGrid[x][y + 1].visited == false*/)
			{
				if (current->depth + 1 < dlsGrid[x][y + 1].depth)
				{
					dlsGrid[x][y + 1].visited = true;
					dlsGrid[x][y + 1].prevX = x;
					dlsGrid[x][y + 1].prevY = y;
					dlsGrid[x][y + 1].pathCost = current->pathCost + 3;
					dlsGrid[x][y + 1].depth = current->depth + 1;
				}
				stack.Push(&dlsGrid[x][y + 1]);
			}

			if (x - 1 >= 0 && y + 1 < col && dlsGrid[x - 1][y + 1].obstacle != '1' /*&& dlsGrid[x - 1][y + 1].visited == false*/)
			{
				if (current->depth + 1 < dlsGrid[x - 1][y + 1].depth)
				{

					dlsGrid[x - 1][y + 1].visited = true;
					dlsGrid[x - 1][y + 1].prevX = x;
					dlsGrid[x - 1][y + 1].prevY = y;
					dlsGrid[x - 1][y + 1].pathCost = current->pathCost + 2;
					dlsGrid[x - 1][y + 1].depth = current->depth + 1;
				}
				stack.Push(&dlsGrid[x - 1][y + 1]);
			}

			if (x - 1 >= 0 && dlsGrid[x - 1][y].obstacle != '1' /*&& dlsGrid[x - 1][y].visited == false*/)
			{
				if (current->depth + 1 < dlsGrid[x - 1][y].depth)
				{
					dlsGrid[x - 1][y].visited = true;
					dlsGrid[x - 1][y].prevX = x;
					dlsGrid[x - 1][y].prevY = y;
					dlsGrid[x - 1][y].pathCost = current->pathCost + 1;
					dlsGrid[x - 1][y].depth = current->depth + 1;
				}
				stack.Push(&dlsGrid[x - 1][y]);
			}
		}

		if (stack.IsEmpty())
		{
			cout << "\nSolution does not exist in the current limit\n";
			system("pause>0");
			system("cls");
			return;
		}

	}

	Node* goal;

	while (true)	//Backtracking from goal to start
	{
		goal = &dlsGrid[x][y];
		goal->obstacle = '#';
		if (x == startRow && y == startCol)
			break;
		x = goal->prevX;
		y = goal->prevY;
	}

	dlsGrid[startRow][startCol].obstacle = 'S';
	dlsGrid[goalRow][goalCol].obstacle = 'G';

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (dlsGrid[i][j].obstacle == '#' || dlsGrid[i][j].obstacle == 'G' || dlsGrid[i][j].obstacle == 'S')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			if (dlsGrid[i][j].obstacle == 'O')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

			cout << dlsGrid[i][j].obstacle << "\t";
		}
		cout << endl;
	}
	cout << "Path Cost = " << dlsGrid[goalRow][goalCol].pathCost << endl;
	system("pause>0");
	system("cls");
}

void idsAlgorithm()
{
	Node** idsGrid = new Node*[row];
	for (int i = 0; i < row; i++)
		idsGrid[i] = new Node[col];

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			idsGrid[i][j] = grid[i][j];
	}

	cout << "\t\t\t\t\tIterative Deepening Search\n";

	int limit = 0;
	bool limitExist = true;
	int x = 0;
	int y = 0;
	while (limitExist)
	{
		Stack stack;
		idsGrid[startRow][startCol].depth = 0;
		stack.Push(&idsGrid[startRow][startCol]);
		bool pathFound = false;

		while (!pathFound)		//Find the path to goal
		{
			Node* current = stack.Pop();
			current->visited = true;
			current->explored = true;
			x = current->x;
			y = current->y;

			if (x == goalRow && y == goalCol)
			{
				cout << "Goal Found" << endl;
				pathFound = true;
				limitExist = false;
			}

			if (current->depth < limit)
			{

				if (y + 1 < col && idsGrid[x][y + 1].obstacle != '1'/* && idsGrid[x][y + 1].visited == false*/)
				{
					if (current->depth + 1 < idsGrid[x][y + 1].depth)
					{
						idsGrid[x][y + 1].visited = true;
						idsGrid[x][y + 1].prevX = x;
						idsGrid[x][y + 1].prevY = y;
						idsGrid[x][y + 1].pathCost = current->pathCost + 3;
						idsGrid[x][y + 1].depth = current->depth + 1;
					}
					stack.Push(&idsGrid[x][y + 1]);

				}

				if (x - 1 >= 0 && y + 1 < col && idsGrid[x - 1][y + 1].obstacle != '1' /*&& idsGrid[x - 1][y + 1].visited == false*/)
				{
					if (current->depth + 1 < idsGrid[x - 1][y + 1].depth)
					{

						idsGrid[x - 1][y + 1].visited = true;
						idsGrid[x - 1][y + 1].prevX = x;
						idsGrid[x - 1][y + 1].prevY = y;
						idsGrid[x - 1][y + 1].pathCost = current->pathCost + 2;
						idsGrid[x - 1][y + 1].depth = current->depth + 1;
					}
					stack.Push(&idsGrid[x - 1][y + 1]);
				}

				if (x - 1 >= 0 && idsGrid[x - 1][y].obstacle != '1' /*&& idsGrid[x - 1][y].visited == false*/)
				{
					if (current->depth + 1 < idsGrid[x - 1][y].depth)
					{
						idsGrid[x - 1][y].visited = true;
						idsGrid[x - 1][y].prevX = x;
						idsGrid[x - 1][y].prevY = y;
						idsGrid[x - 1][y].pathCost = current->pathCost + 1;
						idsGrid[x - 1][y].depth = current->depth + 1;
					}
					stack.Push(&idsGrid[x - 1][y]);
				}
			}

			if (stack.IsEmpty())
			{
				limit++;
				break;
			}

		}
	}

	Node* goal;

	while (true)	//Backtracking from goal to start
	{
		goal = &idsGrid[x][y];
		goal->obstacle = '#';
		if (x == startRow && y == startCol)
			break;
		x = goal->prevX;
		y = goal->prevY;
	}

	idsGrid[startRow][startCol].obstacle = 'S';
	idsGrid[goalRow][goalCol].obstacle = 'G';

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (idsGrid[i][j].obstacle == '#' || idsGrid[i][j].obstacle == 'G' || idsGrid[i][j].obstacle == 'S')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			if (idsGrid[i][j].obstacle == 'O')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

			cout << idsGrid[i][j].obstacle << "\t";
		}
		cout << endl;
	}
	cout << "Path Cost = " << idsGrid[goalRow][goalCol].pathCost << endl;
	system("pause>0");
	system("cls");

}

void ucsAlgorithm()
{
	Node** ucsGrid = new Node*[row];

	for (int i = 0; i < row; i++)
		ucsGrid[i] = new Node[col];

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			ucsGrid[i][j] = grid[i][j];
	}

	cout << "\t\t\t\t\tUniform Cost Search\n";

	PriorityQueue queue;
	queue.Enqueue(&ucsGrid[startRow][startCol]);
	int x;
	int y;

	while (true)
	{
		Node* current = queue.Dequeue();
		current->visited = true;
		current->explored = true;
		x = current->x;
		y = current->y;

		if (x == goalRow && y == goalCol)
		{
			cout << "Goal Found" << endl;
			break;
		}

		if (y + 1 < col && ucsGrid[x][y + 1].obstacle != '1' && ucsGrid[x][y + 1].visited == false)
		{
			ucsGrid[x][y + 1].visited = true;
			ucsGrid[x][y + 1].prevX = x;
			ucsGrid[x][y + 1].prevY = y;
			ucsGrid[x][y + 1].pathCost = current->pathCost + 3;
			queue.Enqueue(&ucsGrid[x][y + 1]);
		}

		if (x - 1 >= 0 && y + 1 < col && ucsGrid[x - 1][y + 1].obstacle != '1' && ucsGrid[x - 1][y + 1].visited == false)
		{
			ucsGrid[x - 1][y + 1].visited = true;
			ucsGrid[x - 1][y + 1].prevX = x;
			ucsGrid[x - 1][y + 1].prevY = y;
			ucsGrid[x - 1][y + 1].pathCost = current->pathCost + 2;
			queue.Enqueue(&ucsGrid[x - 1][y + 1]);
		}

		if (x - 1 >= 0 && ucsGrid[x - 1][y].obstacle != '1' && ucsGrid[x - 1][y].visited == false)
		{
			ucsGrid[x - 1][y].visited = true;
			ucsGrid[x - 1][y].prevX = x;
			ucsGrid[x - 1][y].prevY = y;
			ucsGrid[x - 1][y].pathCost = current->pathCost + 1;
			queue.Enqueue(&ucsGrid[x - 1][y]);
		}
	}

	Node* goal;

	while (true)	//Backtracking from goal to start
	{
		goal = &ucsGrid[x][y];
		goal->obstacle = '#';
		if (x == startRow && y == startCol)
			break;
		x = goal->prevX;
		y = goal->prevY;
	}

	ucsGrid[startRow][startCol].obstacle = 'S';
	ucsGrid[goalRow][goalCol].obstacle = 'G';
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (ucsGrid[i][j].obstacle == '#' || ucsGrid[i][j].obstacle == 'G' || ucsGrid[i][j].obstacle == 'S')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			if (ucsGrid[i][j].obstacle == 'O')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

			cout << ucsGrid[i][j].obstacle << "\t";
		}
		cout << endl;
	}
	cout << "Path Cost = " << ucsGrid[goalRow][goalCol].pathCost << endl;
	system("pause>0");
	system("cls");
}

void bdsAlgorithm()
{
	Node** bdsGrid = new Node*[row];
	for (int i = 0; i < row; i++)
		bdsGrid[i] = new Node[col];

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			bdsGrid[i][j] = grid[i][j];
	}

	cout << "\t\t\t\t\tBidirectional Search\n";

	Queue queue;
	Queue goalQueue;
	queue.Enqueue(&bdsGrid[startRow][startCol]);
	goalQueue.Enqueue(&bdsGrid[goalRow][goalCol]);
	int x = 0;
	int y = 0;
	int goalX;
	int goalY;
	int centerX;
	int centerY;

	while (true)
	{
		Node* current = queue.Dequeue();
		current->explored = true;
		x = current->x;
		y = current->y;

		if (x - 1 >= 0 && bdsGrid[x - 1][y].obstacle != '1' && bdsGrid[x - 1][y].explored == false)
		{
			queue.Enqueue(&bdsGrid[x - 1][y]);
		}

		if (x - 1 >= 0 && y + 1 < col && bdsGrid[x - 1][y + 1].obstacle != '1' && bdsGrid[x - 1][y + 1].explored == false)
		{
			queue.Enqueue(&bdsGrid[x - 1][y + 1]);
		}

		if (y + 1 < col && bdsGrid[x][y + 1].obstacle != '1' && bdsGrid[x][y + 1].explored == false)
		{
			queue.Enqueue(&bdsGrid[x][y + 1]);
		}

		Node* goalCurrent = goalQueue.Dequeue();
		if (current == goalCurrent)
		{
			cout << "Goal Found\n";
			centerX = current->x;
			centerY = current->y;
			break;
		}

		else
		{
			goalCurrent->explored = true;
			goalX = goalCurrent->x;
			goalY = goalCurrent->y;

			if (goalX + 1 < row && bdsGrid[goalX + 1][goalY].obstacle != '1' && bdsGrid[goalX + 1][goalY].explored == false)
			{
				goalQueue.Enqueue(&bdsGrid[goalX + 1][goalY]);
			}

			if (goalX + 1 < row && goalY - 1 >= 0 && bdsGrid[goalX + 1][goalY - 1].obstacle != '1' && bdsGrid[goalX + 1][goalY - 1].explored == false)
			{
				goalQueue.Enqueue(&bdsGrid[goalX + 1][goalY - 1]);
			}

			if (goalY - 1 >= 0 && bdsGrid[goalX][goalY - 1].obstacle != '1' && bdsGrid[goalX][goalY - 1].explored == false)
			{
				goalQueue.Enqueue(&bdsGrid[goalX][goalY - 1]);
			}

		}
	}

	Queue startQueue;
	startQueue.Enqueue(&bdsGrid[startRow][startCol]);

	while (true)
	{
		Node* current = startQueue.Dequeue();
		current->visited = true;
		current->explored = true;
		x = current->x;
		y = current->y;

		if (x == centerX && y == centerY)
		{
			break;
		}

		if (x - 1 >= 0 && bdsGrid[x - 1][y].obstacle != '1' && bdsGrid[x - 1][y].visited == false)
		{
			bdsGrid[x - 1][y].visited = true;
			bdsGrid[x - 1][y].prevX = x;
			bdsGrid[x - 1][y].prevY = y;
			bdsGrid[x - 1][y].pathCost = current->pathCost + 1;
			startQueue.Enqueue(&bdsGrid[x - 1][y]);
		}

		if (x - 1 >= 0 && y + 1 < col && bdsGrid[x - 1][y + 1].obstacle != '1' && bdsGrid[x - 1][y + 1].visited == false)
		{
			bdsGrid[x - 1][y + 1].visited = true;
			bdsGrid[x - 1][y + 1].prevX = x;
			bdsGrid[x - 1][y + 1].prevY = y;
			bdsGrid[x - 1][y + 1].pathCost = current->pathCost + 2;
			startQueue.Enqueue(&bdsGrid[x - 1][y + 1]);
		}

		if (y + 1 < col && bdsGrid[x][y + 1].obstacle != '1' && bdsGrid[x][y + 1].visited == false)
		{
			bdsGrid[x][y + 1].visited = true;
			bdsGrid[x][y + 1].prevX = x;
			bdsGrid[x][y + 1].prevY = y;
			bdsGrid[x][y + 1].pathCost = current->pathCost + 3;
			startQueue.Enqueue(&bdsGrid[x][y + 1]);
		}
	}

	Node* goal;

	while (true)	//Backtracking from goal to start
	{
		goal = &bdsGrid[x][y];
		goal->obstacle = '#';
		if (x == startRow && y == startCol)
			break;
		x = goal->prevX;
		y = goal->prevY;
	}

	int cost1 = bdsGrid[centerX][centerY].pathCost;

	for (int i = 0; i < row; i++)
	for (int j = 0; j < col; j++)
		bdsGrid[i][j].visited = false;

	Queue endQueue;
	endQueue.Enqueue(&bdsGrid[goalRow][goalCol]);

	while (true)
	{
		Node* goalCurrent = endQueue.Dequeue();
		goalCurrent->visited = true;
		goalCurrent->explored = true;
		goalX = goalCurrent->x;
		goalY = goalCurrent->y;

		if (centerX == goalX && centerY == goalY)
		{
			break;
		}

		if (goalX + 1 < row && bdsGrid[goalX + 1][goalY].obstacle != '1' && bdsGrid[goalX + 1][goalY].visited == false)
		{
			bdsGrid[goalX + 1][goalY].visited = true;
			bdsGrid[goalX + 1][goalY].prevX = goalX;
			bdsGrid[goalX + 1][goalY].prevY = goalY;
			bdsGrid[goalX + 1][goalY].pathCost = goalCurrent->pathCost + 1;
			endQueue.Enqueue(&bdsGrid[goalX + 1][goalY]);
		}

		if (goalX + 1 < row && goalY - 1 >= 0 && bdsGrid[goalX + 1][goalY - 1].obstacle != '1' && bdsGrid[goalX + 1][goalY - 1].visited == false)
		{
			bdsGrid[goalX + 1][goalY - 1].visited = true;
			bdsGrid[goalX + 1][goalY - 1].prevX = goalX;
			bdsGrid[goalX + 1][goalY - 1].prevY = goalY;
			bdsGrid[goalX + 1][goalY - 1].pathCost = goalCurrent->pathCost + 2;
			endQueue.Enqueue(&bdsGrid[goalX + 1][goalY - 1]);
		}

		if (goalY - 1 >= 0 && bdsGrid[goalX][goalY - 1].obstacle != '1' && bdsGrid[goalX][goalY - 1].visited == false)
		{
			bdsGrid[goalX][goalY - 1].visited = true;
			bdsGrid[goalX][goalY - 1].prevX = goalX;
			bdsGrid[goalX][goalY - 1].prevY = goalY;
			bdsGrid[goalX][goalY - 1].pathCost = goalCurrent->pathCost + 3;
			endQueue.Enqueue(&bdsGrid[goalX][goalY - 1]);
		}

	}

	while (true)	//Backtracking from goal to start
	{
		goal = &bdsGrid[goalX][goalY];
		goal->obstacle = '#';
		if (goalX == goalRow && goalY == goalCol)
			break;
		goalX = goal->prevX;
		goalY = goal->prevY;
	}

	bdsGrid[centerX][centerY].obstacle = 'O';

	cost1 += bdsGrid[centerX][centerY].pathCost;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			if (bdsGrid[i][j].obstacle == '#' || bdsGrid[i][j].obstacle == 'G' || bdsGrid[i][j].obstacle == 'S')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			if (bdsGrid[i][j].obstacle == 'O')
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);

			cout << bdsGrid[i][j].obstacle << "\t";
		}
		cout << endl;
	}
	cout << "Path Cost = " << cost1 << endl;
	system("pause>0");
	system("cls");
}



int main()
{
	if (!readFile())
	{
		cout << "File cannot be opened\n";
		system("pause>0");
		return -1;
	}

	while (true)
	{
		showMenu();
		int opt;
		cin >> opt;
		system("cls");
		if (opt == 1) //BFS
		{
			bfsAlgorithm();
		}
		else if (opt == 2) //UCS
		{
			ucsAlgorithm();
		}
		else if (opt == 3) //DFS
		{
			dfsAlgorithm();
		}
		else if (opt == 4) //IDS
		{
			idsAlgorithm();
		}
		else if (opt == 5) //DLS
		{
			dlsAlgorithm();
		}
		else if (opt == 6) //BDS
		{
			bdsAlgorithm();
		}

		else //Exit
		{
			return -1;
		}
	}

	system("pause>0");
	return 0;
}