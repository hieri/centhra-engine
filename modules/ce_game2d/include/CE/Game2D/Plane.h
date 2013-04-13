#ifndef _CE_GAME2D_PLANE_H_
#define _CE_GAME2D_PLANE_H_

//- Centhra Engine -
#include <CE/Game2D/Zone.h>

namespace ce
{
	namespace game2d
	{
		class Plane : public Entity
		{
			float m_zoneSize;
			unsigned int m_width, m_height;
			Zone ***m_zones;

		public:
			Plane(unsigned int width, unsigned int height, float zoneSize);
			~Plane();

			Zone *GetZone(unsigned int x, unsigned int y) const;
			void Render(float minX, float minY, float maxX, float maxY);
			void Place(ZoneEntity *entity);
			void MoveEntity(ZoneEntity *entity, Vector2<float> movement);
			void ProcessPhysics(float dt);
			void RemoveDead();
		};
	}
}

#endif
