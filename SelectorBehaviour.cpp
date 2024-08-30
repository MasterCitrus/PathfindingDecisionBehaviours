#include "SelectorBehaviour.h"
#include "Agent.h"

void SelectorBehaviour::Update(Agent* agent, float deltaTime)
{
	float distance = glm::distance(agent->GetPosition(), agent->GetTarget()->GetPosition());
	if (distance < agent->GetNodeMap()->GetCellSize() * 2.5)
	{
		SetBehaviour(m_b1, agent);
		agent->SetColour({ 255, 0, 0, 255 });
	}
	else
	{
		SetBehaviour(m_b2, agent);
		agent->SetColour({ 0, 255, 255, 255 });
	}
	m_selected->Update(agent, deltaTime);
}

void SelectorBehaviour::SetBehaviour(Behaviour* b, Agent* agent)
{
	if (m_selected != b)
	{
		m_selected = b;
		agent->Reset();
	}
}
