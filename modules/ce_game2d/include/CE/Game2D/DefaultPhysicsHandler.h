#ifndef _CE_GAME2D_DEFAULTPHYSICSHANDLER_H_
#define _CE_GAME2D_DEFAULTPHYSICSHANDLER_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector2.h>
#include <CE/Game2D/PhysicsHandler.h>

#define CE_OBJECTHANDLE_CACHESIZE 8

namespace ce
{
	namespace game2d
	{
		class DefaultPhysicsHandler : public PhysicsHandler
		{
			class Zone;
			class Plane;
			class ObjectHandle : public PhysicsHandler::ObjectHandle
			{
				Vector2<float> m_moveBoxMin, m_moveBoxMax, m_movement, m_movePadding;
				bool m_canMove[2], m_startedPhysics, m_finishedPhysics, m_cache[CE_OBJECTHANDLE_CACHESIZE];

				static std::vector<ObjectHandle *> ms_cacheVectors[CE_OBJECTHANDLE_CACHESIZE];
				static void ClearCache(unsigned int idx);
				bool Cache(unsigned int idx);

			protected:
				std::vector<Zone *> m_zones;

			public:
				ObjectHandle(PhysicsHandler *handler, PhysicalObject *object);
				~ObjectHandle();

				void AddZone(Zone *zone);
				bool ContainsZone(Zone *zone) const;
				void RemoveZone(Zone *zone);
				void RemoveFromZones();
				Vector2<float> GetExtent() const;
				Vector2<float> GetPosition() const;
				Vector2<float> GetVelocity() const;
				void SetExtent(Vector2<float> extent);
				void SetPosition(Vector2<float> position);
				void SetVelocity(Vector2<float> velocity);
				void Move(Vector2<float> movement);
				bool CollidesWith(ObjectHandle *entity, Vector2<float> offsetA = Vector2<float>(0.f, 0.f), Vector2<float> offsetB = Vector2<float>(0.f, 0.f));
				bool CollidesWith(Vector2<float> boxMin, Vector2<float> boxMax, Vector2<float> offset = Vector2<float>(0.f, 0.f));
				std::vector<Zone *> &GetZones();

				unsigned int GetCollisionMask() const;

				friend class Zone;
				friend class Plane;
			};

			class Zone : public Entity
			{
				float m_minX, m_minY, m_maxX, m_maxY;
				Plane *m_plane;
				std::vector<ObjectHandle *> m_children;

			protected:
				virtual void DoRender();

			public:
				Zone(float minX, float minY, float maxX, float maxY);
				~Zone();

				void Add(ObjectHandle *entity);
				void Remove(ObjectHandle *entity);
				bool IsMember(ObjectHandle *entity) const;
				void RemoveDead();

				void SetPlane(Plane *plane);
				Plane *GetPlane() const;

				void MoveEntity(ObjectHandle *entity, Vector2<float> movement);
				void ProcessPhysics(float dt);

				void PhysicsPhase1(float dt);
				void PhysicsPhase2(float dt);
				void PhysicsPhase3(float dt);

				std::vector<ObjectHandle *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, ObjectHandle *ignore = 0);
				std::vector<ObjectHandle *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, ObjectHandle *ignore = 0);

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
				void Place(ObjectHandle *entity);
				void MoveEntity(ObjectHandle *entity, Vector2<float> movement);
				void ProcessPhysics(float dt);
				void RemoveDead();

				std::vector<ObjectHandle *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, ObjectHandle *ignore = 0);
				std::vector<ObjectHandle *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, ObjectHandle *ignore = 0);
			};

		protected:
			virtual void Initialize();
			virtual void Render(float minX, float minY, float maxX, float maxY);
			virtual void Process(float dt);
			virtual void Cleanup();
			virtual void SetupObject(PhysicalObject *object);
			virtual void CleanupObject(PhysicalObject *object);
			
			std::vector<PhysicalObject *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			std::vector<PhysicalObject *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			
		private: //- TODO: Remove - Temp Static Stuff -
			Plane *m_plane;
		};
	}
}

#endif
