#pragma once
#include "glm/vec2.hpp"
#include <vector>

struct Node;

class INavigatable
{
public:
	virtual void Draw() = 0;
	virtual Node* GetClosestNode(glm::vec2 worldPos) = 0;

	virtual float GetCellSize() { return 32; }
	virtual Node* GetRandomNode() = 0;

	//virtual std::vector<glm::vec2> SmoothPath(const std::vector<Node*>& path) = 0;
	virtual std::vector<glm::vec2> SmoothPath(const std::vector<Node*>& path) { return std::vector<glm::vec2>(); }

	
};