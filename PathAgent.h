#pragma once

#include "Pathfinding.h"

class PathAgent
{
public:
	void Update(float deltaTime);
	void GoToNode(Node* node);

	void SetNode(Node* node);
	void SetSpeed(float speed);

	std::vector<Node*> GetPath();
	glm::vec2 GetPosition() const;

private:
	glm::vec2 m_position;

	std::vector<Node*> m_path;
	int m_currentIndex;
	Node* m_currentNode;

	float m_speed;
};

