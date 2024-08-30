#pragma once

#include <vector>

class Agent;
class Behaviour;
class Condition;

class State
{
public:
	State(Behaviour* behaviour);
	~State();

	struct Transition
	{
		Condition* condition;
		State* targetState;
	};

	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
	virtual void Exit(Agent* agent);

	void AddTransition(Condition* condition, State* state);

	std::vector<Transition> GetTransitions();

private:
	std::vector<Behaviour*> m_behaviours;
	std::vector<Transition> m_transitions;
};

