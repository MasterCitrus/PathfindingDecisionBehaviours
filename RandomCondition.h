#pragma once
#include "Condition.h"

#include <random>

class RandomCondition : public Condition
{
public:
	RandomCondition(std::mt19937 gen, int breakpoint) : gen{ gen }, breakpoint{ breakpoint } {}
	virtual bool IsTrue(Agent* agent);

private:
	std::mt19937 gen;
	int breakpoint;
};

