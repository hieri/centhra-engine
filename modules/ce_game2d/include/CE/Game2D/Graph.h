#ifndef _CE_GAME2D_GRAPH_H_
#define _CE_GAME2D_GRAPH_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Vector2.h>
#include <CE/Game2D/Entity.h>

namespace ce
{
	namespace game2d
	{
		class Graph : public Entity
		{
		public:
			class Node : public Entity
			{
				std::vector<Node *> m_neighbors;
				std::vector<float> m_neighborDistances;
				Vector2<float> m_position;

			public:
				Node(Vector2<float> position);

				void AddNeighbor(Node *node);
				void RemoveNeighbor(Node *node);
				bool IsNeighbor(Node *node) const;
				void ClearNeighbors();
			};

			void Add(Node *node);
			void Remove(Node *node);
			bool IsMember(Node *node) const;
			void ClearNodes();

		private:
			std::vector<Node *> m_nodes;
		};
	}
}

#endif
