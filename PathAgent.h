#pragma once

#include "Pathfinding.h"

class PathAgent
{
public:
	void Update(float deltaTime);
	void GoToNode(Node* node);

	void SetNode(Node* node);
	void SetSpeed(float speed);
	void SetSmoothPath(std::vector<glm::vec2> smoothPath);

	Node* GetNode() { return m_currentNode; }
	std::vector<Node*> GetPath();
	std::vector<glm::vec2> GetSmoothPath();
	glm::vec2 GetPosition() const;

private:
	glm::vec2 m_position;

	std::vector<Node*> m_path;
	std::vector<glm::vec2> m_smoothPath;
	int m_currentIndex;
	Node* m_currentNode;
	glm::vec2 m_currentNodePosition;

	float m_speed;
};

