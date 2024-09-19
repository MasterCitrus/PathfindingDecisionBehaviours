#pragma once
#include "Behaviour.h"
class AttackBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime) override;
	virtual void Enter(Agent* agent) override;
	virtual void Exit(Agent* agent) override;

	virtual float Evaluate(Agent* agent) override;
private:
	float timer = 0.5f;
};

