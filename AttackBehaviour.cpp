#include "AttackBehaviour.h"
#include "Agent.h"

void AttackBehaviour::Update(Agent* agent, float deltaTime)
{
	timer -= deltaTime;

	if (timer <= 0.0f)
	{
		timer = 0.5f;
		agent->GetTarget()->TakeDamage(agent->GetDamage());
	}
}

void AttackBehaviour::Enter(Agent* agent)
{
	agent->SetColour({ 255, 0, 0, 255 });
	agent->Reset();
}

void AttackBehaviour::Exit(Agent* agent)
{

}

float AttackBehaviour::Evaluate(Agent* agent)
{
	Agent* target = agent->GetTarget();
	float distance = glm::distance(target->GetPosition(), agent->GetPosition());

	float eval = 0;
	if (target->GetHP() > 0)
	{
		if (distance < agent->GetNodeMap()->GetCellSize()) eval = 1000;
	}
	else eval = 0;
	return eval;
}