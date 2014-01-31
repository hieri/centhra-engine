#ifndef _CE_GAME3D_PHYSICALOBJECT_H_
#define _CE_GAME3D_PHYSICALOBJECT_H_

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector3.h>
#include <CE/Quaternion.h>
#include <CE/Game3D/Entity.h>
#include <CE/Game3D/PhysicsHandler.h>

namespace ce
{
	namespace game3d
	{
		class PhysicalGroup;

		class PhysicalObject : public Entity
		{
			static unsigned int ms_lastID;

			Vector3<float> m_position, m_size, m_velocity;
			Quaternion<float> m_rotation;
			unsigned int m_id;
			PhysicalGroup *m_parentGroup;
			PhysicsHandler::ObjectHandle *m_objectHandle;

		protected:
			Color m_color; //- TODO: Remove this after finished -
			unsigned int m_collisionMask;
			bool m_isTrigger, m_isStatic;

			virtual void DoRender();

		public:
			PhysicalObject(Vector3<float> position, Vector3<float> size);
			~PhysicalObject();

			void Render();
			bool IsStatic() const;
			Quaternion<float> GetRotation() const;
			Vector3<float> GetSize() const;
			Vector3<float> GetPosition() const;
			Vector3<float> GetVelocity() const;
			void SetStatic(bool isStatic);
			void SetSize(Vector3<float> size, bool updateHandle = true);
			void SetPosition(Vector3<float> position, bool updateHandle = true);
			void SetVelocity(Vector3<float> velocity, bool updateHandle = true);
			void SetRotation(Quaternion<float> rotation, bool updateHandle = true);

			PhysicsHandler::ObjectHandle *GetObjectHandle() const;
			unsigned int GetCollisionMask() const;
			void SetCollisionMask(unsigned int mask);
			bool IsTrigger() const;

//			virtual bool OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact);

			virtual void OnSetCollisionMask();
			virtual void OnSetStatic();
			virtual void OnSetPosition();
			virtual void OnSetSize();
			virtual void OnSetVelocity();
			virtual void OnSetRotation();

			static void Cleanup();

			friend class PhysicalGroup;
			friend class PhysicsHandler::ObjectHandle;
		};
	}
}

#endif
