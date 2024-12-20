#include "FiniteStateMachine.h"
#include "Agent.h"
#include "Condition.h"

FiniteStateMachine::~FiniteStateMachine()
{
	//for (State* s : m_states) delete s;
}

void FiniteStateMachine::Update(Agent* agent, float deltaTime)
{
	State* newState = nullptr;

	for (State::Transition t : m_currentState->GetTransitions())
	{
		if (t.condition->IsTrue(agent)) newState = t.targetState;
	}

	if (newState != nullptr && newState != m_currentState)
	{
		m_currentState->Exit(agent);
		m_currentState = newState;
		m_currentState->Enter(agent);
	}
	
	m_currentState->Update(agent, deltaTime);
}

void FiniteStateMachine::AddState(State* state)
{
	m_states.push_back(state);
}

void FiniteStateMachine::Enter(Agent* agent)
{
	m_currentState->Enter(agent);
}
