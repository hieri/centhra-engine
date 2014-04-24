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
			static unsigned long long ms_lastID;
			static std::vector<PhysicalObject *> ms_objects, ms_netObjects;

			unsigned long long m_id, m_netID;
			PhysicalGroup *m_parentGroup;
			PhysicsHandler::ObjectHandle *m_objectHandle;

		protected:
			Vector2<float> m_position, m_extent, m_velocity;
			float m_rotation, m_angularVelocity;
			Color m_color; //- TODO: Remove this after finished -
			unsigned int m_collisionMask;
			bool m_isTrigger, m_isStatic;

			virtual void DoRender();

		public:
			static PhysicalObject *GetObjectByID(unsigned long long id);
			static PhysicalObject *GetNetObjectByID(unsigned long long id);

			PhysicalObject(Vector2<float> position, Vector2<float> extent, unsigned long long id = -1, unsigned long long netID = -1);
			~PhysicalObject();

			void Render();
			bool IsStatic() const;
			float GetRotation() const;
			float GetAngularVelocity() const;
			Vector2<float> GetExtent() const;
			Vector2<float> GetPosition() const;
			Vector2<float> GetVelocity() const;
			void SetStatic(bool isStatic);
			void SetExtent(Vector2<float> extent, bool updateHandle = true);
			void SetPosition(Vector2<float> position, bool updateHandle = true);
			void SetVelocity(Vector2<float> velocity, bool updateHandle = true);
			void SetRotation(float rotation, bool updateHandle = true);
			void SetAngularVelocity(float angularVelocity, bool updateHandle = true);

			unsigned long long GetID() const;
			unsigned long long GetNetID() const;

			PhysicsHandler::ObjectHandle *GetObjectHandle() const;
			unsigned int GetCollisionMask() const;
			void SetCollisionMask(unsigned int mask);
			bool IsTrigger() const;

			virtual bool OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact);

			virtual void OnSetCollisionMask();
			virtual void OnSetStatic();
			virtual void OnSetPosition();
			virtual void OnSetExtent();
			virtual void OnSetVelocity();
			virtual void OnSetRotation();
			virtual void OnSetAngularVelocity();

			friend class PhysicalGroup;
			friend class PhysicsHandler::ObjectHandle;
		};
	}
}

#endif
