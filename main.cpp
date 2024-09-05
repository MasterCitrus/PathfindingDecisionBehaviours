#include "raylib.h"
#include "NodeMap.h"
#include "Agent.h"
#include "GotoBehaviour.h"
#include "WanderBehaviour.h"
#include "FollowBehaviour.h"
#include "SelectorBehaviour.h"
#include "DistanceCondition.h"
#include "FiniteStateMachine.h"
#include "UtilityAI.h"
#include "NavMesh.h"
#include <string>
#include <vector>
#include <time.h>

int main()
{
	int screenWidth = 1280;
	int screenHeight = 720;

	bool drawPaths = false;

	std::vector<Agent> agents;

	InitWindow(screenWidth, screenHeight, "Pathfinding");

	SetTargetFPS(60);

	//NodeMap setup
	NodeMap map;
	std::vector<std::string> asciiMap;
	asciiMap.push_back("000000000000");
	asciiMap.push_back("010111011100");
	asciiMap.push_back("011101110110");
	asciiMap.push_back("010100000010");
	asciiMap.push_back("010111111110");
	asciiMap.push_back("010000001000");
	asciiMap.push_back("011111111110");
	asciiMap.push_back("000000000000");
	map.Intialise(asciiMap, 50);

	//Node* start = map.GetNode(1, 1);
	//Node* end = map.GetNode(10, 2);

	//NavMesh setup
	NavMesh navigation(screenWidth, screenHeight);
	srand(time(nullptr));
	navigation.AddObstacles(250, 10, 10);
	navigation.Build();
	Node* start = navigation.GetNodes()[0];

	//Agent setup
	Agent agent(&navigation, new GotoBehaviour());
	agent.GetPathAgent().SetNode(start);
	agent.GetPathAgent().SetSpeed(64);

	Agent agent2(&navigation, new WanderBehaviour());
	agent2.GetPathAgent().SetNode(navigation.GetRandomNode());
	agent2.GetPathAgent().SetSpeed(64);

	//Finite State Machine for Agent 3
	DistanceCondition* closerThan2half = new DistanceCondition(2.5f * map.GetCellSize(), true);
	DistanceCondition* furtherThan4 = new DistanceCondition(4.0f * map.GetCellSize(), false);

	State* wanderState = new State(new WanderBehaviour());
	State* followState = new State(new FollowBehaviour());
	wanderState->AddTransition(closerThan2half, followState);
	followState->AddTransition(furtherThan4, wanderState);

	FiniteStateMachine* fsm = new FiniteStateMachine(wanderState);
	fsm->AddState(wanderState);
	fsm->AddState(followState);

	//Utility AI for Agent 3
	UtilityAI* utilityAI = new UtilityAI();
	utilityAI->AddBehaviour(new WanderBehaviour());
	utilityAI->AddBehaviour(new FollowBehaviour());

	Agent agent3(&navigation, utilityAI);
	agent3.GetPathAgent().SetNode(navigation.GetRandomNode());
	agent3.SetTarget(&agent);
	agent3.GetPathAgent().SetSpeed(32);

	for (int i = 0; i < 100; i++)
	{
		Agent newAgent(&navigation, new WanderBehaviour());
		newAgent.GetPathAgent().SetNode(navigation.GetRandomNode());
		newAgent.GetPathAgent().SetSpeed(64);

		agents.push_back(newAgent);
	}

	//Get path
	//std::vector<Node*> nodeMapPath = NodeMap::DijkstraSearch(start, end);
	Color lineColour = { 255, 255, 255, 255 };

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		BeginDrawing();

		ClearBackground(BLACK);

		navigation.Draw();
		

		if (IsKeyPressed(KEY_SPACE)) drawPaths = !drawPaths;
		//Draw all connections
		//map.Draw(true);
		////Draw selected path
		if (drawPaths)
		{
			//map.DrawPath(agent.GetPathAgent().GetPath(), agent.GetColor());
			//map.DrawPath(agent2.GetPathAgent().GetPath(), agent.GetColor());
			//map.DrawPath(agent3.GetPathAgent().GetPath(), agent.GetColor());
			navigation.DrawPath(agent.GetPathAgent().GetSmoothPath(), agent.GetColor());
			//navigation.DrawPath(agent2.GetPathAgent().GetSmoothPath(), lineColour);
			navigation.DrawPath(agent3.GetPathAgent().GetSmoothPath(), agent.GetColor());
		}
		
		////Agent update/draw
		agent.Update(deltaTime);
		agent.Draw();

		agent2.Update(deltaTime);
		agent2.Draw();

		agent3.Update(deltaTime);
		agent3.Draw();

		for (auto& a : agents)
		{
			a.Update(deltaTime);
			a.Draw();
		}

		EndDrawing();
	}

	CloseWindow();
}