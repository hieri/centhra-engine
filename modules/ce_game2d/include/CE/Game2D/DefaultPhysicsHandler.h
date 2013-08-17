#ifndef _CE_GAME2D_DEFAULTPHYSICSHANDLER_H_
#define _CE_GAME2D_DEFAULTPHYSICSHANDLER_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector2.h>
#include <CE/Game2D/PhysicsHandler.h>

#define CE_ZONEENTITY_CACHESIZE 8

namespace ce
{
	namespace game2d
	{
		class DefaultPhysicsHandler : public PhysicsHandler
		{
			class Zone;
			class Plane;
			class ZoneEntity : public Entity
			{
				static unsigned int ms_lastID;

				Vector2<float> m_position, m_extent, m_velocity, m_moveBoxMin, m_moveBoxMax, m_movement, m_movePadding;
				bool m_canMove[2], m_startedPhysics, m_finishedPhysics, m_cache[CE_ZONEENTITY_CACHESIZE];
				Color m_color;
				unsigned int m_id;

				static std::vector<ZoneEntity *> ms_cacheVectors[CE_ZONEENTITY_CACHESIZE];
				static void ClearCache(unsigned int idx);
				bool Cache(unsigned int idx);

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

				void SetPlane(Plane *plane);
				Plane *GetPlane() const;

				void MoveEntity(ZoneEntity *entity, Vector2<float> movement);
				void ProcessPhysics(float dt);

				void PhysicsPhase1(float dt);
				void PhysicsPhase2(float dt);
				void PhysicsPhase3(float dt);

				std::vector<ZoneEntity *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, ZoneEntity *ignore = 0);
				std::vector<ZoneEntity *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, ZoneEntity *ignore = 0);

				friend class Plane;
			};

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

				std::vector<ZoneEntity *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, ZoneEntity *ignore = 0);
				std::vector<ZoneEntity *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, ZoneEntity *ignore = 0);
			};
		};
	}
}

#endif
