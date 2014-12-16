//- Standard Library -
#include <map>
#include <algorithm>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/Graph.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		void Graph::Add(Node *node)
		{
			if(!IsMember(node))
				m_nodes.push_back(node);
		}
		void Graph::Remove(Node *node)
		{
			vector<Node *>::iterator it = find(m_nodes.begin(), m_nodes.end(), node);
			if(it != m_nodes.end())
				m_nodes.erase(it);
		}
		bool Graph::IsMember(Node *node) const
		{
			return find(m_nodes.begin(), m_nodes.end(), node) != m_nodes.end();
		}
		void Graph::ClearNodes()
		{
			m_nodes.clear();
		}
		vector<Graph::Node *> Graph::FindPath(Node *nodeA, Node *nodeB)
		{
			vector<Node *> path, queue;
			map<Node *, Node *> pathMap;
			map<Node *, float> distMap;
			pathMap[nodeA] = 0;
			distMap[nodeA] = 0;
			queue.push_back(nodeA);

			while(queue.size())
			{
				Node *node = queue[0];
				queue.erase(queue.begin());
				if(node == nodeB)
					break;

				int count = node->m_neighbors.size();
				for(int a = 0; a < count; a++)
				{
					Node *neighbor = node->m_neighbors[a];
					float dist = node->m_neighborDistances[a] + distMap[node];

					if(pathMap.count(neighbor))
					{
						if(distMap[neighbor] > dist)
						{
							distMap[neighbor] = dist;
							pathMap[neighbor] = node;
						}
					}
					else
					{
						pathMap[neighbor] = node;
						distMap[neighbor] = dist;

						if(queue.empty() == false)
						{
							Node **markNodes = &queue.front();
							Node **endNodes = markNodes + queue.size();
							while(markNodes != endNodes)
							{
								if(distMap[*markNodes] > dist)
								{
									queue.insert(queue.begin() + (markNodes - &queue.front()), neighbor);
									break;
								}
								markNodes++;
							}
							if(markNodes == endNodes)
								queue.push_back(neighbor);
						}
						else
							queue.push_back(neighbor);
					}
				}
			}

			if(pathMap.count(nodeB))
			{
				Node *node = nodeB;
				while(node)
				{
					path.insert(path.begin(), node);
					node = pathMap[node];
				}
			}

			return path;
		}
		vector<Vector2<float> > Graph::FindPath(Vector2<float> posA, Vector2<float> posB, unsigned int mask, PhysicalGroup *group, PhysicalObject *ignore)
		{
			vector<Vector2<float> > path;
			vector<Node *> queue;
			map<Node *, Node *> pathMap;
			map<Node *, float> distMap, endDistMap;

			vector<Node *> neighborsA, neighborsB;
			if(m_nodes.empty() == false)
			{
				Node **markNodes = &m_nodes.front();
				Node **endNodes = markNodes + m_nodes.size();
				while(markNodes != endNodes)
				{
					Node *node = *markNodes++;
					if(!group->SegmentSearch(posA[0], posA[1], node->m_position[0], node->m_position[1], mask, ignore).size())
					{
						float dist = (posA - node->m_position).GetLength();
						neighborsA.push_back(node);
						pathMap[node] = 0;
						distMap[node] = dist;

						if(queue.empty() == false)
						{
							Node **markNodes = &queue.front();
							Node **endNodes = markNodes + queue.size();
							while(markNodes != endNodes)
							{
								if(distMap[*markNodes] > dist)
								{
									queue.insert(queue.begin() + (markNodes - &queue.front()), node);
									break;
								}
								markNodes++;
							}
							if(markNodes == endNodes)
								queue.push_back(node);
						}
						else
							queue.push_back(node);
					}
					if(!group->SegmentSearch(posB[0], posB[1], node->m_position[0], node->m_position[1], mask, ignore).size())
					{
						neighborsB.push_back(node);
						endDistMap[node] = (posB - node->m_position).GetLength();
					}
				}
			}

			int n = 0;
			while(queue.size())
			{
				Node *node = queue[0];
				queue.erase(queue.begin());
				if(endDistMap.count(node))
					n++;
				if(n == endDistMap.size())
					break;

				int count = node->m_neighbors.size();
				for(int a = 0; a < count; a++)
				{
					Node *neighbor = node->m_neighbors[a];
					float dist = node->m_neighborDistances[a] + distMap[node];
					if(endDistMap.count(neighbor))
						dist += endDistMap[neighbor];

					if(pathMap.count(neighbor))
					{
						if(distMap[neighbor] > dist)
						{
							distMap[neighbor] = dist;
							pathMap[neighbor] = node;
						}
					}
					else
					{
						pathMap[neighbor] = node;
						distMap[neighbor] = dist;

						if(queue.empty() == false)
						{
							Node **markNodes = &queue.front();
							Node **endNodes = markNodes + queue.size();
							while(markNodes != endNodes)
							{
								if(distMap[*markNodes] > dist)
								{
									queue.insert(queue.begin() + (markNodes - &queue.front()), neighbor);
									break;
								}
								markNodes++;
							}
							if(markNodes == endNodes)
								queue.push_back(neighbor);
						}
						else
							queue.push_back(neighbor);
					}
				}
			}

			Node *shortest = 0;
			float shortestDist = 0.f;
			if(neighborsB.empty() == false)
			{
				Node **markNodes = &neighborsB.front();
				Node **endNodes = markNodes + neighborsB.size();
				while(markNodes != endNodes)
				{
					Node *node = *markNodes++;
					if(shortest)
					{
						if(shortestDist > distMap[node])
						{
							shortest = node;
							shortestDist = distMap[node];
						}
					}
					else
					{
						shortest = node;
						shortestDist = distMap[node];
					}
				}
			}

			path.insert(path.begin(), posB);
			if(pathMap.count(shortest))
			{
				Node *node = shortest;
				while(node)
				{
					path.insert(path.begin(), node->m_position);
					node = pathMap[node];
				}
			}
			path.insert(path.begin(), posA);

			return path;
		}
		void Graph::GenerateNeighbors(PhysicalGroup *group, unsigned int mask)
		{
			if(m_nodes.empty() == false)
			{
				Node **markNodesA = &m_nodes.front(), **markNodesB;
				Node **endNodes = markNodesA + m_nodes.size();
				while(markNodesA != endNodes)
					(*markNodesA++)->ClearNeighbors();

				markNodesA = &m_nodes.front();
				while(markNodesA != endNodes)
				{
					Node *nodeA = *markNodesA++;
					markNodesB = &m_nodes.front();
					while(markNodesB != endNodes)
					{
						Node *nodeB = *markNodesB++;
						if(nodeA != nodeB)
							if(!nodeA->IsNeighbor(nodeB))
								if(!group->SegmentSearch(nodeA->m_position[0], nodeA->m_position[1], nodeB->m_position[0], nodeB->m_position[1], mask).size())
								{
									nodeA->AddNeighbor(nodeB);
									nodeB->AddNeighbor(nodeA);
								}
					}
				}
			}
		}

		vector<Graph::Node *> Graph::Node::ms_cacheVectors[CE_GRAPHNODE_CACHESIZE];
		void Graph::Node::ClearCache(unsigned int idx)
		{
			ms_cacheVectors[idx].clear();
		}
		bool Graph::Node::Cache(unsigned int idx)
		{
			if(m_cache[idx])
				return false;

			m_cache[idx] = true;
			ms_cacheVectors[idx].push_back(this);
			return true;
		}
		Graph::Node::Node(Vector2<float> position)
		{
			m_position = position;
		}
		void Graph::Node::AddNeighbor(Node *node)
		{
			if(!IsNeighbor(node))
			{
				m_neighbors.push_back(node);
				m_neighborDistances.push_back((m_position - node->m_position).GetLength());
			}
		}
		void Graph::Node::RemoveNeighbor(Node *node)
		{
			int a = 0;
			if(m_neighbors.empty() == false)
			{
				Node **markNodes = &m_neighbors.front();
				Node **endNodes = markNodes + m_neighbors.size();
				while(markNodes != endNodes)
				{
					Node *nodeB = *markNodes++;
					if(node == nodeB)
					{
						m_neighbors.erase(m_neighbors.begin() + a);
						m_neighborDistances.erase(m_neighborDistances.begin() + a);
					}
					a++;
				}
			}
		}
		bool Graph::Node::IsNeighbor(Node *node) const
		{
			return find(m_neighbors.begin(), m_neighbors.end(), node) != m_neighbors.end();
		}
		void Graph::Node::ClearNeighbors()
		{
			m_neighbors.clear();
			m_neighborDistances.clear();
		}
		Vector2<float> Graph::Node::GetPosition() const
		{
			return m_position;
		}
		vector<Graph::Node *> Graph::Node::GetNeighbors() const
		{
			return m_neighbors;
		}
	}
}
