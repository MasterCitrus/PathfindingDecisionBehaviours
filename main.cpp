#include "raylib.h"
#include "NodeMap.h"
#include "Agent.h"
#include "GotoBehaviour.h"
#include "WanderBehaviour.h"
#include "FollowBehaviour.h"
#include "SelectorBehaviour.h"
#include "DistanceCondition.h"
#include "FiniteStateMachine.h"
#include <string>
#include <vector>

int main()
{
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "A* Path");

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

	//Start/End nodes
	Node* start = map.GetNode(1, 1);
	Node* end = map.GetNode(10, 2);

	//Agent setup
	Agent agent(&map, new GotoBehaviour());
	agent.GetPathAgent().SetNode(start);
	agent.GetPathAgent().SetSpeed(64);
	agent.SetColour(Color{ 0,255,0,255 });

	Agent agent2(&map, new WanderBehaviour());
	agent2.GetPathAgent().SetNode(map.GetRandomNode());
	agent2.GetPathAgent().SetSpeed(64);

	DistanceCondition* closerThan5 = new DistanceCondition(5.0f * map.GetCellSize(), true);
	DistanceCondition* furtherThan7 = new DistanceCondition(7.0f * map.GetCellSize(), false);

	State* wanderState = new State(new WanderBehaviour());
	State* followState = new State(new FollowBehaviour());
	wanderState->AddTransition(closerThan5, followState);
	followState->AddTransition(furtherThan7, wanderState);

	FiniteStateMachine* fsm = new FiniteStateMachine(wanderState);
	fsm->AddState(wanderState);
	fsm->AddState(followState);

	Agent agent3(&map, fsm);
	agent3.GetPathAgent().SetNode(map.GetRandomNode());
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
		
		//Draw all connections
		map.Draw(true);
		//Draw selected path
		map.DrawPath(agent.GetPathAgent().GetPath(), lineColour);

		//Agent update/draw
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