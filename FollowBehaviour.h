#pragma once
#include "Behaviour.h"
#include "glm/vec2.hpp"

class FollowBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime) override;
	virtual void Enter(Agent* agent) override;
	virtual void Exit(Agent* agent) override;

	virtual float Evaluate(Agent* agent) override;

private:
	glm::vec2 lastTargetPosition;
};

