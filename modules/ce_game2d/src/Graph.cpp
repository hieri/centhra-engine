//- Centhra Engine -
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
