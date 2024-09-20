#pragma once

class Agent;

class Condition
{
public:
	virtual ~Condition() {}
	virtual bool IsTrue(Agent* agent) = 0;
};