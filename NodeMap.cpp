#include "NodeMap.h"
#include "Agent.h"
#include <iostream>
#include <algorithm>

void NodeMap::Intialise(std::vector<std::string> asciiMap, int cellSize)
{
	m_cellSize = cellSize;
	const char emptySquare = '0';

	m_height = asciiMap.size();
	m_width = asciiMap[0].size();

	std::cout << "\n[[MAP STATS]]\n";
	std::cout << "Height: " << m_height << '\n';
	std::cout << "Width: " << m_width << '\n';

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

				Node* nodeSouthWest = (x == 0 || y == 0) ? nullptr : GetNode(x - 1, y - 1);
				if (nodeSouthWest)
				{
					node->ConnectTo(nodeSouthWest, 1.414f);
					nodeSouthWest->ConnectTo(node, 1.414f);
				}

				Node* nodeSouthEast = (x == m_width - 1 || y == 0) ? nullptr : GetNode(x + 1, y - 1);
				if (nodeSouthEast)
				{
					node->ConnectTo(nodeSouthEast, 1.414f);
					nodeSouthEast->ConnectTo(node, 1.414f);
				}
			}
		}
	}
}

void NodeMap::Draw()
{
	Color borderColour{ 60, 0, 255, 255 };
	Color cellColour{ 127, 200, 60, 255 };
	Color lineColour{ 0, 0, 0, 255 };

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			std::string coords = std::to_string(x).c_str() + std::string(",") + std::to_string(y).c_str();
			Node* node = GetNode(x, y);
			if (node == nullptr)
			{
				DrawRectangle(x * m_cellSize, y * m_cellSize, m_cellSize - 1, m_cellSize - 1, borderColour);
			}
			else
			{
				DrawRectangle(x * m_cellSize, y * m_cellSize, m_cellSize - 1, m_cellSize - 1, cellColour);
				//DrawCircle((x * m_cellSize) + m_cellSize / 2, (y * m_cellSize) + m_cellSize / 2, m_cellSize / 8, lineColour);
				for (int i = 0; i < node->connections.size(); i++)
				{
					Node* other = node->connections[i].target;
					//DrawLine((x + 0.5f) * m_cellSize, (y + 0.5f) * m_cellSize, (int)other->position.x, (int)other->position.y, lineColour);
				}
			}
			DrawText(coords.c_str(), x * m_cellSize, y * m_cellSize, 5, BLACK);
		}
	}
}

void NodeMap::DrawPath(Agent* agent, Color lineColour)
{
	std::vector<Node*> path = agent->GetPathAgent().GetPath();
	if (path.empty()) return;
	for (int i = 1; i < path.size(); i++)
	{
		Node* node = *(path.begin() + i);
		Node* prevNode = *(path.begin() + (i - 1));
		DrawLine(prevNode->position.x, prevNode->position.y, node->position.x, node->position.y, lineColour);
	}
}

Node* NodeMap::GetClosestNode(glm::vec2 worldPos)
{
	int i = (int)(worldPos.x / m_cellSize);
	if (i < 0 || i >= m_width) return nullptr;

	int j = (int)(worldPos.y / m_cellSize);
	if (j < 0 || j >= m_height) return nullptr;
	return GetNode(i, j);
}

Node* NodeMap::GetRandomNode()
{
	Node* node = nullptr;
	while (node == nullptr)
	{
		int x = rand() % m_width;
		int y = rand() % m_height;
		node = GetNode(x, y);
	}
	return node;
}

float NodeMap::GetCellSize()
{
	return m_cellSize;
}

bool NodeMap::IsVisibleFrom(Node* start, Node* end)
{
	glm::vec2 delta = end->position - start->position;
	float distance = glm::distance(end->position, start->position);
	delta = delta * (m_cellSize / distance);

	for (float cells = 1.0f; cells < distance / m_cellSize; cells += 1.0f)
	{
		glm::vec2 testPosition = start->position + (delta * cells);

		if (GetClosestNode(testPosition) == nullptr) return false;
	}

	return true;
}

std::vector<Node*> NodeMap::SmoothPath(std::vector<Node*> path)
{
	if(path.empty()) return std::vector<Node*>();

	std::vector<Node*> newPath;
	
	Node* start = *path.begin();
	newPath.push_back(start);

	for (int i = 0; i < path.size(); i++)
	{
		while (i < path.size() - 1 && IsVisibleFrom(start, path[i + 1])) i++;

		start = path[i];
		newPath.push_back(start);
	}

	return newPath;
}
