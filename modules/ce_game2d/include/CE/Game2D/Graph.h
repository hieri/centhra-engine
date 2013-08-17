#ifndef _CE_GAME2D_GRAPH_H_
#define _CE_GAME2D_GRAPH_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Vector2.h>
#include <CE/Game2D/PhysicalGroup.h>

#define CE_GRAPHNODE_CACHESIZE 2

namespace ce
{
	namespace game2d
	{
		class Graph : public Entity
		{
		public:
			class Node : public Entity
			{
				static std::vector<Node *> ms_cacheVectors[CE_GRAPHNODE_CACHESIZE];
				static void ClearCache(unsigned int idx);

				std::vector<Node *> m_neighbors;
				std::vector<float> m_neighborDistances;
				Vector2<float> m_position;
				bool m_cache[CE_GRAPHNODE_CACHESIZE];
				bool Cache(unsigned int idx);

			public:
				Node(Vector2<float> position);

				void AddNeighbor(Node *node);
				void RemoveNeighbor(Node *node);
				bool IsNeighbor(Node *node) const;
				std::vector<Node *> GetNeighbors() const;
				void ClearNeighbors();
				Vector2<float> GetPosition() const;

				friend class Graph;
			};

			void Add(Node *node);
			void Remove(Node *node);
			bool IsMember(Node *node) const;
			void ClearNodes();
			std::vector<Node *> FindPath(Node *nodeA, Node *nodeB);
			std::vector<Vector2<float> > FindPath(Vector2<float> posA, Vector2<float> posB, unsigned int mask, PhysicalGroup *group, PhysicalObject *ignore = 0);
			void GenerateNeighbors(PhysicalGroup *group, unsigned int mask);

		private:
			std::vector<Node *> m_nodes;
		};
	}
}

#endif
