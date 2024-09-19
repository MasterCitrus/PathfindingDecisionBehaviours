#pragma once
#include "MultiCondition.h"

class XORMultiCondition : public MultiCondition
{
public:
	XORMultiCondition(Condition* condition, Condition* condition2) : condition{ condition }, condition2{ condition2 } {}
	virtual bool IsTrue(Agent* agent);

private:
	Condition* condition;
	Condition* condition2;
};

