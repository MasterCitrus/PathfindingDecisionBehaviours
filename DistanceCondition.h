#pragma once
#include "Condition.h"

class DistanceCondition : public Condition
{
public:
	DistanceCondition(float d, bool lt) : m_distance{ d }, m_lessThan{ lt } {}

	virtual bool IsTrue(Agent* agent);

private:
	float m_distance;
	bool m_lessThan;
};

