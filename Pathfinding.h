#pragma once

#include "glm/glm.hpp"
#include <vector>

struct Node;

struct Edge
{
	Edge() { target = nullptr; cost = 0; }
	Edge(Node* other, float cost) : target(other), cost(cost) {}

	Node* target;
	float cost;
};

struct Node
{
	glm::vec2 position;
	std::vector<Edge> connections;

	float gScore;
	float hScore;
	float fScore;
	Node* previous;

	Node(float x, float y);

	void ConnectTo(Node* other, float cost);

	static bool CompareG(Node* a, Node* b);

	static bool CompareF(Node* a, Node* b);
};

