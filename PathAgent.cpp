#include "PathAgent.h"
#include "NavMesh.h"


void PathAgent::Update(float deltaTime)
{
	if (usingNavMesh)
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
			//m_currentNode = *(m_path.begin() + m_currentIndex);
			m_currentIndex++;
			if (m_currentIndex >= m_smoothPath.size())
			{
				m_position = m_currentNodePosition;
				m_smoothPath.clear();
				m_currentNode = m_path.back();
			}
			else
			{
				m_currentNodePosition = m_smoothPath[m_currentIndex];
				//m_currentNode = *(m_path.begin() + m_currentIndex);
			}
		}
	}
	else
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
			else
			{
				m_path.clear();
				m_currentNode = nextNode;
				m_position = m_currentNode->position;
			}
		}
	}
}

void PathAgent::GoToNode(Node* node)
{
	m_path = NavMesh::AStarSearch(m_currentNode, node);
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
