#pragma once
#include "raylib.h"
#include "INavigatable.h"
#include "PathAgent.h"
#include "Behaviour.h"
#include <vector>

class Agent
{
public:
	Agent() {}
	Agent(INavigatable* nodeMap, Behaviour* behaviour);
	~Agent() {}//delete m_current; }

	void Update(float deltaTime);
	void Draw();

	void Reset();

	void GoTo(glm::vec2 point);

	void SetTarget(Agent* agent);
	void SetColour(Color colour);

	void TakeDamage(float damage) { m_hp -= damage; };
	void SetDamage(float damage) { m_damage = damage; }

	Agent* GetTarget();
	PathAgent& GetPathAgent();
	INavigatable* GetNodeMap();
	glm::vec2 GetPosition();
	Color GetColor() const { return m_colour; }
	float GetHP() { return m_hp; }
	float GetDamage() { return m_damage; }

	bool PathComplete();

private:
	PathAgent m_pathAgent;
	Behaviour* m_current;
	INavigatable* m_nodeMap;
	Color m_colour;
	Agent* m_target;
	int m_hp;
	int m_damage;
};

