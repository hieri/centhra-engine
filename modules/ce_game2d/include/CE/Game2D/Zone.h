#ifndef _CE_GAME2D_ZONE_H_
#define _CE_GAME2D_ZONE_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Game2D/ZoneEntity.h>

namespace ce
{
	namespace game2d
	{
		class Plane;
		class Zone : public Entity
		{
			float m_minX, m_minY, m_maxX, m_maxY;
			Plane *m_plane;
			std::vector<ZoneEntity *> m_children;

		protected:
			virtual void DoRender();

		public:
			Zone(float minX, float minY, float maxX, float maxY);
			~Zone();

			void Add(ZoneEntity *entity);
			void Remove(ZoneEntity *entity);
			bool IsMember(ZoneEntity *entity) const;
			void RemoveDead();

			void FinishRender();

			void SetPlane(Plane *plane);
			Plane *GetPlane() const;

			void MoveEntity(ZoneEntity *entity, Vector2<float> movement);
			void ProcessPhysics(float dt);

			void PhysicsPhase1(float dt);
			void PhysicsPhase2(float dt);
			void PhysicsPhase3(float dt);

			std::vector<ZoneEntity *> BoxSearch(float minX, float minY, float maxX, float maxY);

			friend class Plane;
		};
	}
}

#endif
