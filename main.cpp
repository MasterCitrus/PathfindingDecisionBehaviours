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

int main()
{
	int screenWidth = 800;
	int screenHeight = 450;

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
	srand(22);
	navigation.AddObstacles(12, 60, 60);
	navigation.Build();
	Node* start = navigation.GetNodes()[0];

	//Agent setup
	Agent agent(&navigation, new GotoBehaviour());
	agent.GetPathAgent().SetNode(start);
	agent.GetPathAgent().SetSpeed(64);
	agent.SetColour(Color{ 0,255,0,255 });

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

	//Get path
	//std::vector<Node*> nodeMapPath = NodeMap::DijkstraSearch(start, end);
	Color lineColour = { 255, 255, 0, 255 };

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		BeginDrawing();

		ClearBackground(BLACK);

		navigation.Draw();
		
		//Draw all connections
		//map.Draw(true);
		////Draw selected path
		map.DrawPath(agent.GetPathAgent().GetPath(), lineColour);

		////Agent update/draw
		agent.Update(deltaTime);
		agent.Draw();

		agent2.Update(deltaTime);
		agent2.Draw();

		agent3.Update(deltaTime);
		agent3.Draw();

		EndDrawing();
	}

	CloseWindow();
}