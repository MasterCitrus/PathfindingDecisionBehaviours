#pragma once

#include "raylib.h"
#include "Pathfinding.h"
#include "INavigatable.h"
#include "glm/vec2.hpp"
#include <string>

class NodeMap : public INavigatable
{
	int m_width, m_height;
	float m_cellSize;

	Node** m_nodes;

public:
	void Intialise(std::vector<std::string> asciiMap, int cellSize);

	//Node functions
	Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
	virtual Node* GetClosestNode(glm::vec2 worldPos) override;
	virtual Node* GetRandomNode() override;
	virtual float GetCellSize() override;

	int SmoothPath(const std::vector<Node*>& path, std::vector<glm::vec2>& smoothPath) override { return smoothPath.size(); }

	void Draw() override;

	void DrawPath(std::vector<Node*> path, Color lineColour);

	//Pathfinding functions
	static std::vector<Node*> DijkstraSearch(Node* startNode, Node* endNode);
	static std::vector<Node*> AStarSearch(Node* startNode, Node* endNode);

};

