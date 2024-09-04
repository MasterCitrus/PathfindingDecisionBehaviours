#include "INavigatable.h"
#include "Pathfinding.h"
#include <algorithm>
#include <iostream>

std::vector<Node*> INavigatable::DijkstraSearch(Node* startNode, Node* endNode)
{
	if (startNode == nullptr || endNode == nullptr)
	{
		if (startNode == nullptr && endNode == nullptr) std::cout << "Nodes do not exist.\n";
		else if (startNode == nullptr) std::cout << "startNode does not exist.\n";
		else if (endNode == nullptr) std::cout << "endNode do not exist.\n";
		return std::vector<Node*>();
	}

	if (startNode == endNode) return std::vector<Node*>();

	startNode->gScore = 0;
	startNode->previous = nullptr;

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	openList.push_back(startNode);

	while (!openList.empty())
	{
		if (openList.size() > 1) std::sort(openList.begin(), openList.end(), Node::CompareG);

		Node* currentNode = *openList.begin();

		if (currentNode == endNode) break;

		openList.erase(openList.begin());
		closedList.push_back(currentNode);

		for (auto& c : currentNode->connections)
		{
			if (std::find(closedList.begin(), closedList.end(), c.target) == closedList.end())
			{
				int gScore = currentNode->gScore + c.cost;

				if (std::find(openList.begin(), openList.end(), c.target) == openList.end())
				{
					c.target->gScore = gScore;
					c.target->previous = currentNode;
					openList.push_back(c.target);
				}
				else if (gScore < c.target->gScore)
				{
					c.target->gScore = gScore;
					c.target->previous = currentNode;
				}
			}
		}
	}

	std::vector<Node*> path;
	Node* currentNode = endNode;

	while (currentNode)
	{
		path.push_back(currentNode);
		currentNode = currentNode->previous;
	}

	std::reverse(path.begin(), path.end());

	return path;
}

std::vector<Node*> INavigatable::AStarSearch(Node* startNode, Node* endNode)
{
	if (startNode == nullptr || endNode == nullptr)
	{
		if (startNode == nullptr && endNode == nullptr) std::cout << "Nodes do not exist.\n";
		else if (startNode == nullptr) std::cout << "startNode does not exist.\n";
		else if (endNode == nullptr) std::cout << "endNode do not exist.\n";
		return std::vector<Node*>();
	}

	if (startNode == endNode) return std::vector<Node*>();

	startNode->gScore = 0;
	startNode->hScore = 0;
	startNode->fScore = 0;
	startNode->previous = nullptr;

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	openList.push_back(startNode);

	while (!openList.empty())
	{
		if (openList.size() > 1) std::sort(openList.begin(), openList.end(), Node::CompareF);

		Node* currentNode = *openList.begin();

		if (currentNode == endNode) break;

		openList.erase(openList.begin());
		closedList.push_back(currentNode);

		for (auto& c : currentNode->connections)
		{
			if (std::find(closedList.begin(), closedList.end(), c.target) == closedList.end())
			{
				int gScore = currentNode->gScore + c.cost;
				int hScore = glm::distance(c.target->position, endNode->position);
				int fScore = gScore + hScore;

				if (std::find(openList.begin(), openList.end(), c.target) == openList.end())
				{
					c.target->gScore = gScore;
					c.target->fScore = fScore;
					c.target->previous = currentNode;
					openList.push_back(c.target);
				}
				else if (gScore < c.target->gScore)
				{
					c.target->gScore = gScore;
					c.target->fScore = fScore;
					c.target->previous = currentNode;
				}
			}
		}
	}

	std::vector<Node*> path;
	Node* currentNode = endNode;

	while (currentNode)
	{
		path.push_back(currentNode);
		currentNode = currentNode->previous;
	}

	std::reverse(path.begin(), path.end());

	return path;
}
