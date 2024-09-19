#pragma once
#include "MultiCondition.h"

class ORMultiCondition : public MultiCondition
{
public:
	ORMultiCondition(bool result) : result{ result } {}
	virtual bool IsTrue(Agent* agent) override;

private:
	bool result;
};

