#include "NodeMap.h"
#include <iostream>
#include <algorithm>

void NodeMap::Intialise(std::vector<std::string> asciiMap, int cellSize)
{
	m_cellSize = cellSize;
	const char emptySquare = '0';

	m_height = asciiMap.size();
	m_width = asciiMap[0].size();

	m_nodes = new Node * [m_width * m_height];

	for (int y = 0; y < m_height; y++)
	{
		std::string& line = asciiMap[y];

		if (line.size() != m_width)
		{
			std::cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")\n";
		}

		for (int x = 0; x < m_width; x++)
		{
			char tile = x < line.size() ? line[x] : emptySquare;

			m_nodes[x + m_width * y] = tile == emptySquare ? nullptr : new Node(((float)x + 0.5f) * m_cellSize, ((float)y + 0.5f) * m_cellSize);
		}
	}

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			Node* node = GetNode(x, y);
			if (node)
			{
				Node* nodeWest = x == 0 ? nullptr : GetNode(x - 1, y);
				if (nodeWest)
				{
					node->ConnectTo(nodeWest, 1);
					nodeWest->ConnectTo(node, 1);
				}

				Node* nodeSouth = y == 0 ? nullptr : GetNode(x, y - 1);
				if (nodeSouth)
				{
					node->ConnectTo(nodeSouth, 1);
					nodeSouth->ConnectTo(node, 1);
				}
			}
		}
	}
}

void NodeMap::Draw(bool draw)
{
	Color cellColour{ 255, 0, 0, 255 };
	Color lineColour{ 127, 127, 127, 255 };

	if (draw)
	{
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				Node* node = GetNode(x, y);
				if (node == nullptr)
				{
					DrawRectangle(x * m_cellSize, y * m_cellSize, m_cellSize - 1, m_cellSize - 1, cellColour);
				}
				else
				{
					for (int i = 0; i < node->connections.size(); i++)
					{
						Node* other = node->connections[i].target;
						DrawLine((x + 0.5f) * m_cellSize, (y + 0.5f) * m_cellSize, (int)other->position.x, (int)other->position.y, lineColour);
					}
				}
			}
		}
	}
}

void NodeMap::DrawPath(std::vector<Node*> path, Color lineColour)
{
	for (int i = 1; i < path.size(); i++)
	{
		Node* node = *(path.begin() + i);
		Node* prevNode = *(path.begin() + (i - 1));
		DrawLine(prevNode->position.x, prevNode->position.y, (int)node->position.x, (int)node->position.y, lineColour);
	}
}

std::vector<Node*> NodeMap::DijkstraSearch(Node* startNode, Node* endNode)
{
	if (startNode == nullptr || endNode == nullptr) 
	{ 
		if(startNode == nullptr && endNode == nullptr) std::cout << "Nodes do not exist.\n";
		else if(startNode == nullptr) std::cout << "startNode does not exist.\n";
		else if(endNode == nullptr) std::cout << "endNode do not exist.\n";
		return std::vector<Node*>(); 
	}

	if(startNode == endNode) return std::vector<Node*>();
	
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

std::vector<Node*> NodeMap::AStarSearch(Node* startNode, Node* endNode)
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

Node* NodeMap::GetClosestNode(glm::vec2 worldPos)
{
	int i = (int)(worldPos.x / m_cellSize);
	if (i < 0 || i >= m_width) return nullptr;

	int j = (int)(worldPos.y / m_cellSize);
	if (j < 0 || j >= m_height) return nullptr;
	return GetNode(i, j);
}
