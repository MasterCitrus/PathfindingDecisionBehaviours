#include "PathAgent.h"
#include "NodeMap.h"

void PathAgent::Update(float deltaTime)
{
	if (m_path.empty()) return;

	Node* nextNode = m_path[m_currentIndex + 1];
	float distance = glm::distance(m_position, nextNode->position);
	glm::vec2 normal = glm::normalize(nextNode->position - m_position);
	distance -= m_speed * deltaTime;
	if (distance > 0)
	{
		m_position += m_speed * deltaTime * normal;
	}
	else
	{
		if (m_currentIndex < m_path.size() - 2)
		{
			m_currentIndex++;
			m_currentNode = nextNode;
		}
		else m_path.clear(); m_currentNode = nextNode;
	}
}

void PathAgent::GoToNode(Node* node)
{
	m_path = NodeMap::AStarSearch(m_currentNode, node);
	m_currentIndex = 0;
}

void PathAgent::Draw()
{
	DrawCircle((int)m_position.x, (int)m_position.y, 8, { 255,255,0,255 });
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
