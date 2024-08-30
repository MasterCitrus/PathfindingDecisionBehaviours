#pragma once

#include "Behaviour.h"
#include "State.h"

class FiniteStateMachine : public Behaviour
{
public:
	FiniteStateMachine(State* s) : m_currentState{ s }, m_newState{ nullptr } {}
	virtual ~FiniteStateMachine();

	void Update(Agent* agent, float deltaTime);

	void AddState(State* state);
	virtual void Enter(Agent* agent);

private:
	std::vector<State*> m_states;

	State* m_currentState;
	State* m_newState;
};

