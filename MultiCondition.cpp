#include "MultiCondition.h"

MultiCondition::~MultiCondition()
{
	for (auto& c : conditions) delete c;
}

void MultiCondition::AddCondition(Condition* condition)
{
	conditions.push_back(condition);
}
