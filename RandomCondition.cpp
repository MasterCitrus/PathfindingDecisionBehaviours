#include "RandomCondition.h"
#include "Agent.h"

bool RandomCondition::IsTrue(Agent* agent)
{
	std::uniform_int_distribution<> randomNumber(0, 100);
	return randomNumber(gen) > breakpoint;
}
