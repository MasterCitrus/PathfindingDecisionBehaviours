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
#include <fstream>
#include <filesystem>
#include <random>
#include <iostream>

int main()
{
	int screenWidth = 1280;
	int screenHeight = 720;

	std::vector<std::string> mapFileNames;

	const std::filesystem::path maps{ "maps" };

	for (auto const& dir_entry : std::filesystem::directory_iterator{ maps })
	{
		mapFileNames.push_back(dir_entry.path().filename().string());
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> randomMapNumber(0, mapFileNames.size() - 1);


	std::ifstream mapFile("maps/" + *(mapFileNames.begin() + randomMapNumber(gen)));

	bool drawPaths = false;

	std::vector<Agent*> agents;

	InitWindow(screenWidth, screenHeight, "Pathfinding");

	SetTargetFPS(60);

	std::vector<std::string> asciiMap;

	for (std::string line; std::getline(mapFile, line);)
	{
		asciiMap.push_back(line);
	}

	int emptyLines = 0;
	for (auto& string : asciiMap)
	{
		if (string.find('1') != string.npos) break;
		else emptyLines++;
	}

	if (emptyLines == asciiMap.size())
	{
		std::cout << "\n---{{{Invalid Map - No valid nodes}}}---\n";
		return 1;
	}

	//NodeMap setup
	NodeMap map;
	//asciiMap.push_back("000000000000");
	//asciiMap.push_back("010111011100");
	//asciiMap.push_back("011101110110");
	//asciiMap.push_back("010100000010");
	//asciiMap.push_back("010111111110");
	//asciiMap.push_back("010000001000");
	//asciiMap.push_back("011111111110");
	//asciiMap.push_back("000000000000");
	map.Intialise(asciiMap, 50);

	std::cout << "\nPress 'Space' to show path lines.\n";

	Node* start = map.GetNode(1, 1);
	//Node* end = map.GetNode(10, 2);

	//NavMesh setup
	//NavMesh navigation(screenWidth, screenHeight);
	srand(time(nullptr));
	//navigation.AddObstacles(250, 10, 10);
	//navigation.Build();
	//Node* start = navigation.GetNodes()[0];

	//Agent setup
	Agent agent(&map, new GotoBehaviour());
	agent.GetPathAgent().SetNode(start);
	agent.GetPathAgent().SetSpeed(128);


	Agent agent2(&map, new WanderBehaviour());
	agent2.GetPathAgent().SetNode(map.GetRandomNode());
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

	FiniteStateMachine* fsm2 = new FiniteStateMachine(wanderState);
	fsm2->AddState(wanderState);
	fsm2->AddState(followState);

	FiniteStateMachine* fsm3 = new FiniteStateMachine(wanderState);
	fsm3->AddState(wanderState);
	fsm3->AddState(followState);

	//Utility AI for Agent 3
	UtilityAI* utilityAI = new UtilityAI();
	utilityAI->AddBehaviour(new WanderBehaviour());
	utilityAI->AddBehaviour(new FollowBehaviour());

	Agent agent3(&map, fsm);
	agent3.GetPathAgent().SetNode(map.GetRandomNode());
	agent3.SetTarget(&agent);
	agent3.GetPathAgent().SetSpeed(32);

	Agent agent4(&map, fsm2);
	agent4.GetPathAgent().SetNode(map.GetRandomNode());
	agent4.SetTarget(&agent);
	agent4.GetPathAgent().SetSpeed(32);

	Agent agent5(&map, fsm3);
	agent5.GetPathAgent().SetNode(map.GetRandomNode());
	agent5.SetTarget(&agent);
	agent5.GetPathAgent().SetSpeed(32);

	agents.push_back(&agent);
	agents.push_back(&agent2);
	agents.push_back(&agent3);
	agents.push_back(&agent4);
	agents.push_back(&agent5);

	//for (int i = 0; i < 10; i++)
	//{
	//	Agent newAgent(&map, new WanderBehaviour());
	//	newAgent.GetPathAgent().SetNode(map.GetRandomNode());
	//	newAgent.GetPathAgent().SetSpeed(64);

	//	agents.push_back(newAgent);
	//}

	//Get path
	//std::vector<Node*> nodeMapPath = NodeMap::DijkstraSearch(start, end);
	Color lineColour = { 255, 255, 255, 255 };

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		BeginDrawing();

		ClearBackground(GRAY);

		map.Draw();
		

		if (IsKeyPressed(KEY_SPACE)) drawPaths = !drawPaths;
		//Draw all connections
		//map.Draw(true);
		////Draw selected path
		if (drawPaths)
		{
			//map.DrawPath(agent.GetPathAgent().GetPath(), agent.GetColor());
			//map.DrawPath(agent2.GetPathAgent().GetPath(), agent.GetColor());
			//map.DrawPath(agent3.GetPathAgent().GetPath(), agent.GetColor());
			map.DrawPath(&agent, agent.GetColor());
			map.DrawPath(&agent2, agent2.GetColor());
			map.DrawPath(&agent3, agent3.GetColor());
			map.DrawPath(&agent4, agent4.GetColor());
			map.DrawPath(&agent5, agent5.GetColor());
		}
		
		////Agent update/draw
		//agent.Update(deltaTime);
		//agent.Draw();

		//agent2.Update(deltaTime);
		//agent2.Draw();

		//agent3.Update(deltaTime);
		//agent3.Draw();

		//agent4.Update(deltaTime);
		//agent4.Draw();


		for (auto& a : agents)
		{
			a->Update(deltaTime);
			a->Draw();
		}

		EndDrawing();
	}

	CloseWindow();
}