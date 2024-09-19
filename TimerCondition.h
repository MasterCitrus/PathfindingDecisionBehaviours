#pragma once
#include "Condition.h"

class TimerCondition : public Condition
{
public:
	TimerCondition(float time) : timer{ time }, startTime{ time } {}
	virtual bool IsTrue(Agent* agent) override;

private:
	float timer;
	float startTime;
};

