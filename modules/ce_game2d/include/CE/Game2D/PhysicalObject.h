#ifndef _CE_GAME2D_PHYSICALOBJECT_H_
#define _CE_GAME2D_PHYSICALOBJECT_H_

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector2.h>
#include <CE/Game2D/Entity.h>
#include <CE/Game2D/PhysicsHandler.h>

namespace ce
{
	namespace game2d
	{
		class PhysicalGroup;

		class PhysicalObject : public Entity
		{
			static unsigned int ms_lastID;

			Vector2<float> m_position, m_extent, m_velocity;
			float m_rotation;
			unsigned int m_id;
			PhysicalGroup *m_parentGroup;
			PhysicsHandler::ObjectHandle *m_objectHandle;

		protected:
			Color m_color; //- TODO: Remove this after finished -
			unsigned int m_collisionMask;
			bool m_isTrigger;

			virtual void DoRender();

		public:
			PhysicalObject(Vector2<float> position, Vector2<float> extent);
			~PhysicalObject();

			void Render();
			float GetRotation() const;
			Vector2<float> GetExtent() const;
			Vector2<float> GetPosition() const;
			Vector2<float> GetVelocity() const;
			void SetExtent(Vector2<float> extent, bool updateHandle = true);
			void SetPosition(Vector2<float> position, bool updateHandle = true);
			void SetVelocity(Vector2<float> velocity, bool updateHandle = true);
			void SetRotation(float rotation, bool updateHandle = true);

			PhysicsHandler::ObjectHandle *GetObjectHandle() const;
			unsigned int GetCollisionMask() const;
			void SetCollisionMask(unsigned int mask);
			bool IsTrigger() const;

			virtual bool OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact);

			static void Cleanup();

			friend class PhysicalGroup;
			friend class PhysicsHandler::ObjectHandle;
		};
	}
}

#endif
