#include "NavMesh.h"
#include "raylib.h"

NavMesh::NavMesh(float width, float height)
{
	m_polygons.push_back({});
	m_polygons[0].push_back(new p2t::Point(0, 0));
	m_polygons[0].push_back(new p2t::Point(0, height));
	m_polygons[0].push_back(new p2t::Point(width, height));
	m_polygons[0].push_back(new p2t::Point(width, 0));

	m_cdt = new p2t::CDT(m_polygons[0]);

	extents = glm::vec2(width, height);
}

NavMesh::~NavMesh()
{

}

NavMesh::NavMeshNode* NavMesh::GetRandomNavMeshNode() const
{
	int index = rand() % m_nodes.size();
	return m_nodes[index];
}

NavMesh::NavMeshNode* NavMesh::FindClosest(float x, float y) const
{
	NavMesh::NavMeshNode* closest = nullptr;
	float closestDist = 2000 * 2000;

	for (auto node : m_nodes)
	{
		float dist = (node->position.x - x) * (node->position.x - x) + (node->position.y - y) * (node->position.y - y);

		if (dist < closestDist)
		{
			closest = node;
			closestDist = dist;
		}
	}
	return closest;
}

bool NavMesh::AddObstacle(float x, float y, float width, float height, float padding)
{
	for (auto& ob : m_obstacles)
	{
		if (((ob.x + ob.w + ob.padding) < x - padding
			|| (ob.y + ob.h + ob.padding) < y - padding
			|| (x + width + padding) < ob.x - ob.padding
			|| (y + height + padding) < ob.y - ob.padding) == false) return false;
	}

	Obstacle o = { x, y, width, height, padding };
	m_obstacles.push_back(o);

	m_polygons.push_back({});
	m_polygons.back().push_back(new p2t::Point(o.x - padding, o.y - padding));
	m_polygons.back().push_back(new p2t::Point(o.x - padding, o.y + o.h + padding));
	m_polygons.back().push_back(new p2t::Point(o.x + o.w + padding, o.y + o.h + padding));
	m_polygons.back().push_back(new p2t::Point(o.x + o.w + padding, o.y - padding));
	m_cdt->AddHole(m_polygons.back());

	return true;
}

int NavMesh::SmoothPath(const std::vector<Node*>& path, std::vector<glm::vec2>& smoothPath)
{
	if(path.size() == 0) return 0;
	
	smoothPath.clear();
	glm::vec2* portals = new glm::vec2[(path.size() + 1) * 2];
	int index = 0;

	portals[index++] = ((NavMesh::NavMeshNode*)path.front())->position;
	portals[index++] = ((NavMesh::NavMeshNode*)path.front())->position;

	NavMesh::NavMeshNode* prev = nullptr;
	for (auto n : path)
	{
		NavMesh::NavMeshNode* node = (NavMesh::NavMeshNode*)n;
		if (prev != nullptr)
		{
		glm::vec2 adj[2];
			prev->GetAdjacentVertices(node, adj);
			glm::vec2 fromPrev = { node->position.x - prev->position.x, node->position.y - prev->position.y };
			glm::vec2 toAdj0 = { adj[0].x - prev->position.x, adj[0].y - prev->position.y };

			if ((fromPrev.x * toAdj0.y - toAdj0.x * fromPrev.y) > 0)
			{
				portals[index++] = adj[0];
				portals[index++] = adj[1];
			}
			else
			{
				portals[index++] = adj[1];
				portals[index++] = adj[0];
			}
		}
		prev = node;
	}

	portals[index++] = ((NavMesh::NavMeshNode*)path.back())->position;
	portals[index++] = ((NavMesh::NavMeshNode*)path.back())->position;

	glm::vec2 out[100];
	int count = StringPull(portals, index / 2, out, 100);

	for (int i = 0; i < count; ++i)
	{
		smoothPath.push_back(out[i]);
	}

	delete[] portals;

	return smoothPath.size();	
}

int NavMesh::StringPull(const glm::vec2* portals, const int numPortals, glm::vec2* pts, const int maxPts)
{
	int npts = 0;

	glm::vec2 portalApex = portals[0];
	glm::vec2 portalLeft = portals[0];
	glm::vec2 portalRight = portals[1];
	int apexIndex = 0, leftIndex = 0, rightIndex = 0;
	
	pts[npts++] = portalApex;

	for (int i = 1; i < numPortals && npts < maxPts; ++i)
	{
		const glm::vec2& left = portals[i * 2];
		const glm::vec2& right = portals[i * 2 + 1];

		if (TriArea2(portalApex, portalRight, right) <= 0.0f)
		{
			if (VEqual(portalApex, portalRight) || TriArea2(portalApex, portalLeft, right) > 0.0f)
			{
				portalRight = right;
				rightIndex = i;
			}
			else
			{
				pts[npts++] = portalLeft;
				portalApex = portalLeft;
				apexIndex = leftIndex;
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;
				i = apexIndex;
				continue;
			}
		}

		if (TriArea2(portalApex, portalLeft, left))
		{
			if (VEqual(portalApex, portalLeft) || TriArea2(portalApex, portalLeft, left) < 0.0f)
			{
				portalLeft = left;
				leftIndex = i;
			}
			else
			{
				pts[npts++] = portalRight;
				portalApex = portalRight;
				apexIndex = rightIndex;
				portalLeft = portalApex;
				portalRight = portalApex;
				leftIndex = apexIndex;
				rightIndex = apexIndex;
				i = apexIndex;
				continue;
			}
		}

		if (npts < maxPts)
		{
			pts[npts++] = portals[(numPortals - 1) * 2];
		}

		return npts;
	}
}

float NavMesh::TriArea2(const glm::vec2 a, const glm::vec2 b, const glm::vec2 c)
{
	glm::vec2 ab = b - a;
	glm::vec2 ac = c - a;
	return ac.x * ab.y - ab.x - ac.y;
}

bool NavMesh::VEqual(const glm::vec2 a, const glm::vec2 b)
{
	static const float epsilon = 0.001f * 0.001f;
	return glm::distance(a, b) < epsilon;
}

void NavMesh::AddObstacles(int num, int width, int height)
{
	for (int i = 0; i < num; i++)
	{
		bool safe = false;
		do
		{
			safe = AddObstacle((rand() / float(RAND_MAX) * 0.75f + 0.125f) * extents.x,
							   (rand() / float(RAND_MAX) * 0.75f + 0.125f) * extents.y, width, height, 10);
		}
		while (safe == false);
	}
}

void NavMesh::Build()
{
	m_cdt->Triangulate();

	std::vector<p2t::Triangle*> triangles = m_cdt->GetTriangles();
	for (auto tri : triangles)
	{
		auto node = new NavMesh::NavMeshNode();

		node->vertices.push_back({ (float)tri->GetPoint(0)->x, (float)tri->GetPoint(0)->y });
		node->vertices.push_back({ (float)tri->GetPoint(1)->x, (float)tri->GetPoint(1)->y });
		node->vertices.push_back({ (float)tri->GetPoint(2)->x, (float)tri->GetPoint(2)->y });

		node->position.x = (node->vertices[0].x + node->vertices[1].x + node->vertices[2].x) / 3;
		node->position.y = (node->vertices[0].y + node->vertices[1].y + node->vertices[2].y) / 3;

		m_nodes.push_back(node);
	}

	for (auto n : m_nodes)
	{
		for (auto n2 : m_nodes)
		{
			if (n == n2) continue;

			glm::vec2 v[2];
			if (n->GetAdjacentVertices(n2, v) == 2)
			{
				float mag = (n->position.x - n2->position.x) *
					(n->position.x - n2->position.x) *
					(n->position.y - n2->position.y) *
					(n->position.y - n2->position.y);

				float distance = glm::length(n->position - n2->position);
				n->connections.push_back(Edge(n2, distance));
				n2->connections.push_back(Edge(n, distance));
			}
		}
	}

	for (auto& p : m_polygons)
	{
		for (auto ptr : p)
		{
			delete ptr;
		}
	}
	m_polygons.clear();

	delete m_cdt;
	m_cdt = nullptr;
}

void NavMesh::Draw()
{
	Color m_lineColour = { 0, 128, 255, 255 };
	Color m_obstacleColour = { 255, 0, 0, 255 };

	for (auto node : m_nodes)
	{
		DrawLine((int)node->vertices[0].x, (int)node->vertices[0].y, (int)node->vertices[1].x, (int)node->vertices[1].y, m_lineColour);
		DrawLine((int)node->vertices[1].x, (int)node->vertices[1].y, (int)node->vertices[2].x, (int)node->vertices[2].y, m_lineColour);
		DrawLine((int)node->vertices[2].x, (int)node->vertices[2].y, (int)node->vertices[0].x, (int)node->vertices[0].y, m_lineColour);
	}

	for (auto& o : m_obstacles)
	{
		DrawRectangle((int)o.x, (int)o.y, (int)o.w, (int)o.h, m_obstacleColour);
	}
}

void NavMesh::DrawPath(std::vector<glm::vec2> path, Color lineColour)
{

}

int NavMesh::NavMeshNode::GetAdjacentVertices(NavMesh::NavMeshNode* other, glm::vec2* adjacent)
{
	int count = 0;
	for (auto v : vertices)
	{
		for (auto v2 : other->vertices)
		{
			if (v.x == v2.x && v.y == v2.y)
			{
				adjacent[count++] = v;
				break;
			}
		}
	}
	return count;
}
