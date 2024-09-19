#pragma once
#include "MultiCondition.h"

class ANDMultiCondition : public MultiCondition
{
public:
	ANDMultiCondition(bool result) : result{ result } {}
	virtual bool IsTrue(Agent* agent) override;
private:
	bool result;
};

