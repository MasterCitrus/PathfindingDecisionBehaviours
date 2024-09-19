#include "IdleBehaviour.h"
#include "Agent.h"

void IdleBehaviour::Update(Agent* agent, float deltaTime)
{
	if (!agent->PathComplete()) agent->Reset();
}

void IdleBehaviour::Enter(Agent* agent)
{
	agent->SetColour({ 0, 0, 255, 255 });
	agent->Reset();
}

void IdleBehaviour::Exit(Agent* agent)
{

}

float IdleBehaviour::Evaluate(Agent* agent)
{
	return 0.0f;
}
