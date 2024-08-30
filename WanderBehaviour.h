#pragma once
#include "Behaviour.h"

class WanderBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime) override;
	virtual void Enter(Agent* agent) override;

	virtual float Evaluate(Agent* agent) override;
};

