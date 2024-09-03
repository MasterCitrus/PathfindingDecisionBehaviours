#include "PathAgent.h"
#include "NodeMap.h"


void PathAgent::Update(float deltaTime)
{
	if (m_smoothPath.empty()) return;

	float distance = glm::distance(m_position, m_currentNodePosition);
	glm::vec2 normal = glm::normalize(m_currentNodePosition - m_position);
	distance -= m_speed * deltaTime;
	if (distance > 0)
	{
		m_position += m_speed * deltaTime * normal;
	}
	else
	{
		m_currentIndex++;
		if (m_currentIndex >= m_smoothPath.size())
		{
			m_position = m_currentNodePosition;
			m_smoothPath.clear();
		}
		else
		{
			glm::vec2 oldNode = m_currentNodePosition;
			m_currentNodePosition = m_smoothPath[m_currentIndex];
		}
	}
}

void PathAgent::GoToNode(Node* node)
{
	m_path = NodeMap::AStarSearch(m_currentNode, node);
	m_currentIndex = 0;
}

void PathAgent::SetNode(Node* node)
{
	m_currentNode = node;
	m_position = m_currentNode->position;
}

void PathAgent::SetSpeed(float speed)
{
	m_speed = speed;
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
