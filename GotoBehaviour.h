#pragma once
#include "Behaviour.h"

class GotoBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime) override;
};

