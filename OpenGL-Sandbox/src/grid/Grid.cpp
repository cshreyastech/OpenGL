#include "Grid.h"

Grid* Grid::grid_ = nullptr;

Grid* Grid::Instance(const float quad_size, const float lowestISOvalue, const float highestISOvalue)
{
    if (grid_ == nullptr) {
        grid_ = new Grid(quad_size, lowestISOvalue, highestISOvalue);
    }
    return grid_;
}

Grid::Grid(const float quad_size, const float lowestISOvalue, const float highestISOvalue)
    : quad_size_(quad_size), lowestISOvalue_(lowestISOvalue), highestISOvalue_(highestISOvalue)
{
	ConstructGrid();
}

void Grid::ConstructGrid()
{
	Node* aboveNode = nullptr;
	Node* belowNode = nullptr;
	Node* leftNode = nullptr;
	Node* rightNode = nullptr;

	Node* rowHead = nullptr;

	nRows = 0; nCols = 0;
	
	for (float y = -rows; y < rows; y += quad_size_)
	{
		nCols = 0;
		for (float x = -cols; x < cols; x += quad_size_)
		{
			const glm::vec3 A = {  		       x,			   y, 0.0f };
			const glm::vec3 B = { x + quad_size_,			   y, 0.0f };
			const glm::vec3 C = { x + quad_size_, y + quad_size_, 0.0f };
			const glm::vec3 D = {			   x, y + quad_size_, 0.0f };

			Node* newNode = new Node();

			newNode->edgePositions[0] = A;
			newNode->edgePositions[1] = B;
			newNode->edgePositions[2] = C;
			newNode->edgePositions[3] = D;

			newNode->cellID = nCells;

			// head
			if (y == -rows && x == -cols)
			{
				head_ = newNode;
				rowHead = head_;
				leftNode = rowHead;
			}
			
			// Bottom most Row right of head
			else if (y == -rows)
			{
				newNode->left = leftNode;
				leftNode->right = newNode;
				
				leftNode = leftNode->right;
			}

			// Left most Column top of head
			else if (x == -cols)
			{
				belowNode = rowHead;
				rowHead = newNode;
				leftNode = rowHead;

				rowHead->below = belowNode;
				belowNode->above = rowHead;
				belowNode = belowNode->right;
			}
			
			// All other Nodes
			else
			{
				newNode->left = leftNode;
				leftNode->right = newNode;

				newNode->below = belowNode;
				belowNode->above = newNode;

				leftNode = leftNode->right;
				belowNode = belowNode->right;
			}

			tail_ = newNode;
			nCols++;
			nCells++;
		}
		nRows++;
	}
	
	std::cout << std::endl;
}

void Grid::GenerateIsoValues(const float lowestISOvalue, const float highestISOvalue)
{
	Node* rowRunner = head_;
	Node* colRunner = head_;
	Node* temp = nullptr;

	srand((unsigned)time(NULL));
	while (colRunner != nullptr)
	{
		rowRunner = colRunner;

		while (rowRunner != nullptr)
		{
			// head
			if (rowRunner == head_)
			{
				rowRunner->isoSurfaceVal[0] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
				rowRunner->isoSurfaceVal[1] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
				rowRunner->isoSurfaceVal[2] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
				rowRunner->isoSurfaceVal[3] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
			}
			// Bottom most Row right of head
			else if (rowRunner->below == nullptr && rowRunner->above != nullptr &&
				rowRunner->left != nullptr)
			{
				rowRunner->isoSurfaceVal[0] = rowRunner->left->isoSurfaceVal[1];
				rowRunner->isoSurfaceVal[1] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
				rowRunner->isoSurfaceVal[2] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
				rowRunner->isoSurfaceVal[3] = rowRunner->left->isoSurfaceVal[2];
			}
			// Left most Column top of head
			else if (rowRunner->below != nullptr && rowRunner->right != nullptr &&
				rowRunner->left == nullptr)
			{
				rowRunner->isoSurfaceVal[0] = rowRunner->below->isoSurfaceVal[3];
				rowRunner->isoSurfaceVal[1] = rowRunner->below->isoSurfaceVal[2];
				rowRunner->isoSurfaceVal[2] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
				rowRunner->isoSurfaceVal[3] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
			}
			//All other nodes
			else
			{
				rowRunner->isoSurfaceVal[0] = rowRunner->left->isoSurfaceVal[1];
				rowRunner->isoSurfaceVal[1] = rowRunner->below->isoSurfaceVal[2];
				rowRunner->isoSurfaceVal[2] = GetRandomIsoValueInRange(lowestISOvalue, highestISOvalue);
				rowRunner->isoSurfaceVal[3] = rowRunner->left->isoSurfaceVal[2];
			}
			std::cout << rowRunner->cellID << ", ";
			temp = rowRunner;
			rowRunner = rowRunner->right;
		}
		std::cout << std::endl;

		colRunner = colRunner->above;
	}
}

void Grid::PrintFromHead()
{
	std::cout
		<< "nCells" << nCells << ", "
		<< "nRows" << nRows << ", "
		<< "nCols" << nCols << std::endl;

	Node* temp = head_;
	Node* rowRunner = head_;
	Node* colRunner = head_;

	rowRunner = head_;
	colRunner = head_;

	std::cout << std::endl << "Iteration from Head" << std::endl;

	while (colRunner != nullptr)
	{
		rowRunner = colRunner;
		while (rowRunner != nullptr)
		{
			std::cout << rowRunner->cellID << ", ";

			temp = rowRunner;
			rowRunner = rowRunner->right;
		}
		std::cout << std::endl;

		colRunner = colRunner->above;
	}

	Node* node5 = head_->right->above;
	Node* node8 = head_->above->above;
	Node* node9 = head_->right->right->above->above->left;
	Node* node10 = head_->above->above->above->right->right->below;
}


void Grid::PrintFromTail()
{
	std::cout
		<< "nCells" << nCells << ", "
		<< "nRows" << nRows << ", "
		<< "nCols" << nCols << std::endl;

	std::cout << std::endl << "Iteration from Tail" << std::endl;
	Node* temp = tail_;
	Node* rowRunner = tail_;
	Node* colRunner = tail_;

	while (colRunner != nullptr)
	{
		rowRunner = colRunner;

		while (rowRunner != nullptr)
		{
			std::cout << rowRunner->cellID << ", ";
			temp = rowRunner;
			rowRunner = rowRunner->left;
		}
		std::cout << std::endl;

		colRunner = colRunner->below;
	}
}


void Grid::CleanUp()
{
	Node* temp = head_;
	Node* rowRunner = head_;
	Node* colRunner = head_;

	std::cout << std::endl << "Delete grid up from Head" << std::endl;

	while (colRunner != nullptr)
	{
		rowRunner = colRunner;
		colRunner = colRunner->above;

		while (rowRunner != nullptr)
		{
			//std::cout << rowRunner->cellID << ", ";
			temp = rowRunner;
			rowRunner = rowRunner->right;
			
			delete temp;
		}
		//std::cout << std::endl;
	}
}

float Grid::GetRandomIsoValueInRange(const float low, const float high)
{
	//srand((unsigned)time(NULL));

	return (low + static_cast <float> (rand()) /
		(static_cast <float> (RAND_MAX / (high - (low)))));
}