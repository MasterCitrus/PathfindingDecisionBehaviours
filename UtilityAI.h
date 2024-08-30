#pragma once

#include "Behaviour.h"
#include <vector>

class UtilityAI : public Behaviour
{
public:
	~UtilityAI();

	virtual void Update(Agent* agent, float deltaTime);

	void AddBehaviour(Behaviour* behaviour);

private:
	std::vector<Behaviour*> m_behaviours;
	Behaviour* currentBehaviour;
};

