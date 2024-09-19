#pragma once
#include "Condition.h"

class HealthCondition : public Condition
{
public:
	HealthCondition(float hp, bool isSelf) : hpLimit{ hp }, self{ isSelf } {}
	virtual bool IsTrue(Agent* agent);
private:
	float hpLimit;
	bool self;
};

