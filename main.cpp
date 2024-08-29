#include "raylib.h"
#include "NodeMap.h"
#include "PathAgent.h"
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
	PathAgent agent;
	agent.SetNode(start);
	agent.SetSpeed(64);

	//Get path
	//std::vector<Node*> nodeMapPath = NodeMap::DijkstraSearch(start, end);
	Color lineColour = { 255, 255, 0, 255 };

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		BeginDrawing();

		ClearBackground(BLACK);
		
		map.Draw(true);
		map.DrawPath(agent.GetPath(), lineColour);

		if (IsMouseButtonPressed(0))
		{
			Vector2 mousePos = GetMousePosition();
			end = map.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
			agent.GoToNode(end);
		}

		/*if (IsMouseButtonPressed(1))
		{
			Vector2 mousePos = GetMousePosition();
			end = map.GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
			nodeMapPath = NodeMap::DijkstraSearch(start, end);
		}*/
		agent.Update(deltaTime);
		agent.Draw();


		EndDrawing();
	}

	CloseWindow();
}