#include "PathAgent.h"
#include "NodeMap.h"
#include "Pathfinding.h"


void PathAgent::Update(float deltaTime)
{
	if (m_path.empty()) return;

	Node* node = m_nodeMap->GetClosestNode(GetPosition());
	if (node) m_currentNode = node;

	Node* nextNode = m_path.begin() + m_currentIndex == m_path.end() ? m_path.back() : *(m_path.begin() + m_currentIndex);
	float distance = glm::distance(m_position, nextNode->position);
	glm::vec2 normal = glm::normalize(nextNode->position - m_position);
	distance -= m_speed * deltaTime;
	if (distance > 0)
	{
		m_position += m_speed * deltaTime * normal;
	}
	else
	{
		m_currentNode = nextNode;
		if (m_currentIndex >= m_path.size())
		{
			m_path.clear();
			m_position = m_currentNode->position;
		}
		else
		{
			m_currentIndex++;
				
		}
	}
}

void PathAgent::GoToNode(Node* node, NodeMap* map)
{
	m_path = AStarSearch(m_currentNode, node);
	m_path = map->SmoothPath(m_path);
	m_currentIndex = 0;
}

void PathAgent::SetNode(Node* node)
{
	m_currentNode = node;
	m_currentNodePosition = node->position;
	m_position = node->position;
}

void PathAgent::SetSpeed(float speed)
{
	m_speed = speed;
}

void PathAgent::SetSmoothPath(std::vector<glm::vec2> smoothPath)
{
	m_smoothPath = smoothPath;
}

std::vector<Node*> PathAgent::GetPath()
{
	return m_path;
}

std::vector<glm::vec2> PathAgent::GetSmoothPath()
{
	return m_smoothPath;
}

glm::vec2 PathAgent::GetPosition() const
{
	return m_position;
}
