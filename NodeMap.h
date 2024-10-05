#pragma once

#include "raylib.h"
#include "Pathfinding.h"
#include "INavigatable.h"
#include "glm/vec2.hpp"
#include <string>

class Agent;

class NodeMap : public INavigatable
{
	int m_width, m_height;
	int currentIndex = 0;
	float m_cellSize;

	Node** m_nodes;

public:
	~NodeMap();

	void Intialise(std::vector<std::string> asciiMap, int cellSize);

	//Node functions
	Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }
	virtual Node* GetClosestNode(glm::vec2 worldPos) override;
	virtual Node* GetRandomNode() override;
	virtual float GetCellSize() override;

	bool IsVisibleFrom(Node* start, Node* end);

	std::vector<Node*> SmoothPath(std::vector<Node*> path);

	//std::vector<glm::vec2> SmoothPath(const std::vector<Node*>& path) override { return std::vector<glm::vec2>(); }

	void Draw() override;

	void DrawPath(Agent* agent, Color lineColour);

	

};

