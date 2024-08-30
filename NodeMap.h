#pragma once

#include "raylib.h"
#include "Pathfinding.h"
#include "glm/vec2.hpp"
#include <string>

class NodeMap
{
	int m_width, m_height;
	float m_cellSize;

	Node** m_nodes;

public:
	void Intialise(std::vector<std::string> asciiMap, int cellSize);

	//Node functions
	Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
	Node* GetClosestNode(glm::vec2 worldPos);
	Node* GetRandomNode();
	float GetCellSize();

	void Draw(bool draw);

	void DrawPath(std::vector<Node*> path, Color lineColour);

	//Pathfinding functions
	static std::vector<Node*> DijkstraSearch(Node* startNode, Node* endNode);
	static std::vector<Node*> AStarSearch(Node* startNode, Node* endNode);

};

