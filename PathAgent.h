#pragma once

#include "Pathfinding.h"

class NodeMap;

class PathAgent
{
public:
	void Update(float deltaTime);
	void GoToNode(Node* node, NodeMap* map);

	void SetNode(Node* node);
	void SetSpeed(float speed);
	void SetSmoothPath(std::vector<glm::vec2> smoothPath);
	void SetNodeMap(NodeMap* nodeMap) { m_nodeMap = nodeMap; }

	Node* GetNode() { return m_currentNode; }
	std::vector<Node*> GetPath();
	std::vector<glm::vec2> GetSmoothPath();
	glm::vec2 GetPosition() const;

private:

	std::vector<Node*> m_path;
	std::vector<glm::vec2> m_smoothPath;
	NodeMap* m_nodeMap;
	glm::vec2 m_position;
	glm::vec2 m_currentNodePosition;
	int m_currentIndex;
	Node* m_currentNode;

	float m_speed;
};

