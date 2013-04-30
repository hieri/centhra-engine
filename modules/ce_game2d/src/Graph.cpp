//- Standard Library -
#include <map>

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

						bool added = false;
						vector<Node *>::iterator it;
						for(it = queue.begin(); it != queue.end(); it++)
							if(distMap[*it] > dist)
								break;
						queue.insert(it, neighbor);
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
			print("%f\n", distMap[nodeB]);

			return path;
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
			for(vector<Node *>::iterator it = m_neighbors.begin(); it != m_neighbors.end(); it++, a++)
				if(node == *it)
				{
					m_neighbors.erase(it);
					m_neighborDistances.erase(m_neighborDistances.begin() + a);
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
	}
}
