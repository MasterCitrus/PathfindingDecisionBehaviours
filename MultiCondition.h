#pragma once
#include "Condition.h"
#include <vector>

class MultiCondition : public Condition
{
public:
	virtual ~MultiCondition();
	virtual bool IsTrue(Agent* agent) = 0;
	virtual void AddCondition(Condition* condition);
protected:
	std::vector<Condition*> conditions;
};

