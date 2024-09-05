#pragma once
#include "poly2tri.h"
#include "INavigatable.h"
#include "glm/vec2.hpp"
#include "Pathfinding.h"
#include <vector>

struct Color;

class NavMesh : public INavigatable
{
public:
	NavMesh(float width, float height);
	~NavMesh();

	class NavMeshNode : public Node
	{
	public:
		NavMeshNode() {}
		virtual ~NavMeshNode() {}

		int GetAdjacentVertices(NavMeshNode* other, glm::vec2* adjacent);

		std::vector<glm::vec2> vertices;
	};

	struct Obstacle
	{
		float x, y, w, h, padding;
	};

	NavMeshNode* GetRandomNavMeshNode() const;
	virtual Node* GetRandomNode() override { return GetRandomNavMeshNode(); }
	NavMeshNode* FindClosest(float x, float y) const;
	virtual Node* GetClosestNode(glm::vec2 worldPos) { return FindClosest(worldPos.x, worldPos.y); }

	std::vector<NavMeshNode*> GetNodes() { return m_nodes; }

	bool AddObstacle(float x, float y, float width, float height, float padding);

	virtual std::vector<glm::vec2> SmoothPath(const std::vector<Node*>& path) override;

	std::vector<glm::vec2> StringPull(const std::vector<glm::vec2> portals, const int numPortals, const int maxPts);
	float TriArea2(const glm::vec2 a, const glm::vec2 b, const glm::vec2 c);
	bool VEqual(const glm::vec2 a, const glm::vec2 b);

	void AddObstacles(int num, int width, int height);
	void Build();
	void Draw();

	void DrawPath(std::vector<glm::vec2> path, Color lineColour);

protected:
	std::vector<Obstacle> m_obstacles;
	std::vector<NavMeshNode*> m_nodes;

	p2t::CDT* m_cdt;
	std::vector<std::vector<p2t::Point*>> m_polygons;

	glm::vec2 extents;
};

