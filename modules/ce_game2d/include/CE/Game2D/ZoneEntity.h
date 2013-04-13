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
		class Plane;
		class ZoneEntity : public Entity
		{
			static unsigned int ms_lastID;

			Vector2<float> m_position, m_extent, m_velocity, m_moveBoxMin, m_moveBoxMax, m_movement, m_movePadding;
			bool m_canMove[2], m_isRendered, m_startedPhysics, m_finishedPhysics;
			Color m_color;
			unsigned int m_id;

		protected:
			std::vector<Zone *> m_zones;
			unsigned int m_collisionMask;

			virtual void DoRender();

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
			Vector2<float> GetVelocity() const;
			void SetExtent(Vector2<float> extent);
			void SetPosition(Vector2<float> position);
			void SetVelocity(Vector2<float> velocity);
			void Move(Vector2<float> movement);
			bool CollidesWith(ZoneEntity *entity, Vector2<float> offsetA = Vector2<float>(0.f, 0.f), Vector2<float> offsetB = Vector2<float>(0.f, 0.f));
			bool CollidesWith(Vector2<float> boxMin, Vector2<float> boxMax, Vector2<float> offset = Vector2<float>(0.f, 0.f));
			std::vector<Zone *> &GetZones();

			unsigned int GetCollisionMask() const;
			void SetCollisionMask(unsigned int mask);

			virtual bool OnCollision(ZoneEntity *collider);

			static void Cleanup();

			friend class Zone;
			friend class Plane;
		};
	}
}

#endif
