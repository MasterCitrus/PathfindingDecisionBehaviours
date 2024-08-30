#pragma once
#include "Behaviour.h"

class SelectorBehaviour : public Behaviour
{
public:
	SelectorBehaviour(Behaviour* b1, Behaviour* b2) : m_b1{ b1 }, m_b2{ b2 } {}
	~SelectorBehaviour() { delete m_b1; delete m_b2; }
	virtual void Update(Agent* agent, float deltaTime) override;

	void SetBehaviour(Behaviour* b, Agent* agent);

private:
	Behaviour* m_b1;
	Behaviour* m_b2;
	Behaviour* m_selected = nullptr;
};

