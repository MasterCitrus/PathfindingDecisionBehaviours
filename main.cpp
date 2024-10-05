#include "raylib.h"
#include "NodeMap.h"
#include "Agent.h"
#include "GotoBehaviour.h"
#include "WanderBehaviour.h"
#include "FollowBehaviour.h"
#include "AttackBehaviour.h"
#include "IdleBehaviour.h"
#include "DeadBehaviour.h"
#include "DistanceCondition.h"
#include "HealthCondition.h"
#include "TimerCondition.h"
#include "RandomCondition.h"
#include "ANDMultiCondition.h"
#include "ORMultiCondition.h"
#include "XORMultiCondition.h"
#include "FiniteStateMachine.h"
#include "UtilityAI.h"
#include <string>
#include <vector>
#include <time.h>
#include <fstream>
#include <filesystem>
#include <random>
#include <iostream>


int main()
{
	{
		int screenWidth = 1280;
		int screenHeight = 720;

		//Random settings
		srand(time(nullptr));
		std::random_device rd;
		std::mt19937 gen(rd());

		//MAP LOADING
		std::vector<std::string> mapFileNames;

		const std::filesystem::path maps{ "maps" };

		for (auto const& dir_entry : std::filesystem::directory_iterator{ maps })
		{
			mapFileNames.push_back(dir_entry.path().filename().string());
		}

		std::uniform_int_distribution<> randomMapNumber(0, mapFileNames.size() - 1);

		std::ifstream mapFile("maps/" + *(mapFileNames.begin() + randomMapNumber(gen)));

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
		//MAP LOADING END

		bool drawPaths = false;

		InitWindow(screenWidth, screenHeight, "Pathfinding");
		SetTargetFPS(60);

		//NodeMap setup
		NodeMap map;
		map.Intialise(asciiMap, 50);

		std::cout << "\nPress 'Space' to show path lines.\n";

		Node* start = map.GetNode(1, 1);

		//Conditions
		DistanceCondition* closerThan2Half = new DistanceCondition(2.5f * map.GetCellSize(), true);
		DistanceCondition* furtherThan4 = new DistanceCondition(4.0f * map.GetCellSize(), false);
		DistanceCondition* closerThanOne = new DistanceCondition(1.0f * map.GetCellSize(), true);
		DistanceCondition* furtherThanOne = new DistanceCondition(1.0f * map.GetCellSize(), false);
		HealthCondition* targetDead = new HealthCondition(0.0f, false);
		HealthCondition* dead = new HealthCondition(0.0f, true);
		ORMultiCondition* targetDeadOrFurtherThan4 = new ORMultiCondition(true);
		targetDeadOrFurtherThan4->AddCondition(furtherThan4);
		targetDeadOrFurtherThan4->AddCondition(targetDead);
		ORMultiCondition* furtherThanOneOrTargetDead = new ORMultiCondition(true);
		furtherThanOneOrTargetDead->AddCondition(furtherThanOne);
		furtherThanOneOrTargetDead->AddCondition(targetDead);
		XORMultiCondition* targetDeadXorFurtherThanOne = new XORMultiCondition(targetDead, furtherThanOne);
		XORMultiCondition* closerThanOneXorTargetDead = new XORMultiCondition(closerThanOne, targetDead);
		XORMultiCondition* cT2HXTD = new XORMultiCondition(closerThan2Half, targetDead);
		//TimerCondition* twoSec = new TimerCondition(2.0f);
		//RandomCondition* random = new RandomCondition(gen, 95);

		//States
		State* wanderState = new State(new WanderBehaviour());
		State* followState = new State(new FollowBehaviour());
		State* attackState = new State(new AttackBehaviour());
		State* deadState = new State(new DeadBehaviour());
		//State* idleState = new State(new IdleBehaviour());
		State* goToState = new State(new GotoBehaviour());
		wanderState->AddTransition(cT2HXTD, followState);
		wanderState->AddTransition(dead, deadState);
		//wanderState->AddTransition(random, idleState);
		followState->AddTransition(targetDeadOrFurtherThan4, wanderState);
		followState->AddTransition(closerThanOneXorTargetDead, attackState);
		followState->AddTransition(dead, deadState);
		attackState->AddTransition(targetDeadXorFurtherThanOne, wanderState);
		attackState->AddTransition(furtherThanOneOrTargetDead, followState);
		attackState->AddTransition(dead, deadState);
		//idleState->AddTransition(twoSec, wanderState);
		goToState->AddTransition(dead, deadState);

		//Player State Machine
		FiniteStateMachine* playerFSM = new FiniteStateMachine(goToState);
		playerFSM->AddState(goToState);
		playerFSM->AddState(deadState);

		//AI Finite State Machines
		FiniteStateMachine* fsm = new FiniteStateMachine(wanderState);
		fsm->AddState(wanderState);
		fsm->AddState(followState);
		fsm->AddState(attackState);
		fsm->AddState(deadState);

		FiniteStateMachine* fsm2 = new FiniteStateMachine(wanderState);
		fsm2->AddState(wanderState);
		fsm2->AddState(followState);
		fsm2->AddState(attackState);
		fsm2->AddState(deadState);

		FiniteStateMachine* fsm3 = new FiniteStateMachine(wanderState);
		fsm3->AddState(wanderState);
		fsm3->AddState(followState);
		fsm3->AddState(attackState);
		fsm3->AddState(deadState);

		//Utility AI
		UtilityAI* utilityAI = new UtilityAI();
		utilityAI->AddBehaviour(new WanderBehaviour());
		utilityAI->AddBehaviour(new FollowBehaviour());
		utilityAI->AddBehaviour(new AttackBehaviour());
		utilityAI->AddBehaviour(new DeadBehaviour());

		//Agent setup

		//Player
		Agent agent(&map, playerFSM);
		agent.GetPathAgent().SetNode(start);
		agent.GetPathAgent().SetSpeed(128);
		agent.SetDamage(25);

		//AI Agents
		Agent agent2(&map, utilityAI);
		agent2.GetPathAgent().SetNode(map.GetRandomNode());
		agent2.GetPathAgent().SetSpeed(64);
		agent2.SetTarget(&agent);
		agent2.SetDamage(10);

		Agent agent3(&map, fsm);
		agent3.GetPathAgent().SetNode(map.GetRandomNode());
		agent3.SetTarget(&agent);
		agent3.GetPathAgent().SetSpeed(32);
		agent3.SetDamage(10);

		Agent agent4(&map, fsm2);
		agent4.GetPathAgent().SetNode(map.GetRandomNode());
		agent4.SetTarget(&agent);
		agent4.GetPathAgent().SetSpeed(32);
		agent4.SetDamage(10);

		Agent agent5(&map, fsm3);
		agent5.GetPathAgent().SetNode(map.GetRandomNode());
		agent5.SetTarget(&agent);
		agent5.GetPathAgent().SetSpeed(32);
		agent5.SetDamage(10);

		std::vector<Agent*> agents;

		agents.push_back(&agent);
		//agents.push_back(&agent2);
		agents.push_back(&agent3);
		agents.push_back(&agent4);
		agents.push_back(&agent5);

		Color lineColour = { 255, 255, 255, 255 };

		while (!WindowShouldClose())
		{
			float deltaTime = GetFrameTime();

			BeginDrawing();

			ClearBackground(GRAY);

			map.Draw();


			if (IsKeyPressed(KEY_SPACE)) drawPaths = !drawPaths;

			for (auto& a : agents)
			{
				a->Update(deltaTime);
				a->Draw();
			}


			//Draw all connections
			if (drawPaths)
			{
				map.DrawPath(&agent, agent.GetColor());
				map.DrawPath(&agent2, agent2.GetColor());
				map.DrawPath(&agent3, agent3.GetColor());
				map.DrawPath(&agent4, agent4.GetColor());
				map.DrawPath(&agent5, agent5.GetColor());
			}

			std::string text = "HP: " + std::to_string(agent.GetHP());
			DrawText(text.c_str(), 10, 10, 20, WHITE);

			if (agent.GetHP() <= 0) DrawText("YOU DEAD MON?", (screenWidth / 2) - 450, (screenHeight / 2) - 75, 100, BLACK);

			EndDrawing();
		}

		CloseWindow();

		delete utilityAI;
	}
}