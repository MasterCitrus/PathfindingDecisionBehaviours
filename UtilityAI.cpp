#include "UtilityAI.h"

UtilityAI::~UtilityAI()
{
	for (Behaviour* b : m_behaviours) delete b;
}

void UtilityAI::Update(Agent* agent, float deltaTime)
{
	float bestEval = 0;
	Behaviour* newBehaviour = nullptr;
	for (Behaviour* b : m_behaviours)
	{
		float eval = b->Evaluate(agent);
		if (eval > bestEval)
		{
			bestEval = eval;
			newBehaviour = b;
		}
	}

	if (newBehaviour != nullptr && newBehaviour != currentBehaviour)
	{
		if (currentBehaviour) currentBehaviour->Exit(agent);
		currentBehaviour = newBehaviour;
		currentBehaviour->Enter(agent);
	}

	currentBehaviour->Update(agent, deltaTime);
}

void UtilityAI::AddBehaviour(Behaviour* behaviour)
{
	m_behaviours.push_back(behaviour);
}
