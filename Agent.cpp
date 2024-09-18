#include "Agent.h"
#include "Behaviour.h"

Agent::Agent(INavigatable* nodeMap, Behaviour* behaviour) : m_current{ behaviour }, m_nodeMap{ nodeMap }, m_colour{ 255, 255, 0, 255 }, m_target{ nullptr }
{
	m_current->Enter(this);
}

void Agent::Update(float deltaTime)
{
	if (m_current) m_current->Update(this, deltaTime);
	m_pathAgent.Update(deltaTime);
}

void Agent::Draw()
{
	glm::vec2 position = m_pathAgent.GetPosition();
	DrawCircle((int)position.x, (int)position.y, 8, m_colour);
}

void Agent::Reset()
{
	m_pathAgent.GetPath().clear();
	m_pathAgent.GetSmoothPath().clear();
}

void Agent::GoTo(glm::vec2 point)
{
	Node* end = m_nodeMap->GetClosestNode(point);
	m_pathAgent.GoToNode(end);
	m_pathAgent.SetSmoothPath(m_nodeMap->SmoothPath(m_pathAgent.GetPath()));
}

void Agent::SetTarget(Agent* agent)
{
	m_target = agent;
}

void Agent::SetColour(Color colour)
{
	m_colour = colour;
}

Agent* Agent::GetTarget()
{
	return m_target;
}

PathAgent& Agent::GetPathAgent()
{
	return m_pathAgent;
}

INavigatable* Agent::GetNodeMap()
{
	return m_nodeMap;
}

glm::vec2 Agent::GetPosition()
{
	return m_pathAgent.GetPosition();
}

bool Agent::PathComplete()
{
	return m_pathAgent.GetPath().empty();
}
