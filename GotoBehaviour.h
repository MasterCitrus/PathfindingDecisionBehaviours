#pragma once
#include "Behaviour.h"

class GotoBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime) override;
	virtual void Enter(Agent* agent) override;
	virtual void Exit(Agent* agent) override;
};

