#include "State.h"
#include "Agent.h"
#include "Behaviour.h"


State::State(Behaviour* behaviour)
{
	m_behaviours.push_back(behaviour);
}

State::~State()
{
	for (Behaviour* b : m_behaviours) delete b;

	for (Transition t : m_transitions) delete t.condition;
}

void State::Update(Agent* agent, float deltaTime)
{
	for (Behaviour* b : m_behaviours) b->Update(agent, deltaTime);
}

void State::AddTransition(Condition* condition, State* state)
{
	Transition transition;
	transition.condition = condition;
	transition.targetState = state;

	m_transitions.push_back(transition);
}

std::vector<State::Transition> State::GetTransitions()
{
	return m_transitions;
}
