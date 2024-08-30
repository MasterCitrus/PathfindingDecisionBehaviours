#pragma once
#include "raylib.h"
#include "NodeMap.h"
#include "PathAgent.h"
#include "Behaviour.h"
#include <vector>

class Agent
{
public:
	Agent() {}
	Agent(NodeMap* nodeMap, Behaviour* behaviour);
	~Agent() { delete m_current; }

	void Update(float deltaTime);
	void Draw();

	void Reset();

	void GoTo(glm::vec2 point);

	void SetTarget(Agent* agent);
	void SetColour(Color colour);

	Agent* GetTarget();
	PathAgent& GetPathAgent();
	NodeMap* GetNodeMap();
	glm::vec2 GetPosition();

	bool PathComplete();

private:
	PathAgent m_pathAgent;
	Behaviour* m_current;
	NodeMap* m_nodeMap;
	Color m_colour;
	Agent* m_target;
};

