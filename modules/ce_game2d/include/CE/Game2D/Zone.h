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
		class Zone
		{
			float m_minX, m_minY, m_maxX, m_maxY;
			Plane *m_plane;
			std::vector<ZoneEntity *> m_children;

		public:
			Zone(float minX, float minY, float maxX, float maxY);
			~Zone();

			void Add(ZoneEntity *entity);
			void Remove(ZoneEntity *entity);
			bool IsMember(ZoneEntity *entity) const;

			void Render();
			void FinishRender();

			void SetPlane(Plane *plane);
			Plane *GetPlane() const;
		};
	}
}

#endif
