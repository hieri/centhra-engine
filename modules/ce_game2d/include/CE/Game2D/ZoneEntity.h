#ifndef _CE_GAME2D_ZONEENTITY_H_
#define _CE_GAME2D_ZONEENTITY_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector2.h>
#include <CE/Game2D/Entity.h>

namespace ce
{
	namespace game2d
	{
		class Zone;
		class ZoneEntity : public Entity
		{
			Vector2<float> m_position, m_extent;
			Color m_color;
			bool m_isRendered;
			std::vector<Zone *> m_zones;

		protected:
			void DoRender();

		public:
			ZoneEntity(Vector2<float> position, Vector2<float> extent);
			~ZoneEntity();

			void AddZone(Zone *zone);
			bool ContainsZone(Zone *zone) const;
			void RemoveZone(Zone *zone);
			void Render();
			void FinishRender();
			Vector2<float> GetExtent() const;
			Vector2<float> GetPosition() const;
			void SetExtent(Vector2<float> extent);
			void SetPosition(Vector2<float> position);
			void Move(Vector2<float> movement);
			std::vector<Zone *> &GetZones();
		};
	}
}

#endif
