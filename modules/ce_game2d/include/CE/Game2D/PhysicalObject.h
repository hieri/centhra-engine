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
			static unsigned short ms_lastID;
			static std::vector<PhysicalObject *> ms_objects, ms_netObjects;

			unsigned short m_id, m_netID;
			PhysicalGroup *m_parentGroup;
			PhysicsHandler::ObjectHandle *m_objectHandle;

		protected:
			Vector2<float> m_position, m_extent, m_velocity;
			float m_rotation, m_angularVelocity;
			Color m_color; //TODO: Remove this after finished
			unsigned int m_typeMask, m_collisionMask;
			bool m_isTrigger, m_isStatic;

			virtual void DoRender();

		public:
			static unsigned short DEFAULT_ID;
			static PhysicalObject *GetObjectByID(unsigned short id);
			static PhysicalObject *GetNetObjectByID(unsigned short id);

			PhysicalObject(Vector2<float> position, Vector2<float> extent, unsigned short id = DEFAULT_ID, unsigned short netID = DEFAULT_ID, bool noID = false);
			virtual ~PhysicalObject();

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

			unsigned short GetID() const;
			unsigned short GetNetID() const;
			void SetNetID(unsigned short id);

			PhysicsHandler::ObjectHandle *GetObjectHandle() const;
			unsigned int GetTypeMask() const;
			unsigned int GetCollisionMask() const;
			void SetTypeMask(unsigned int mask);
			void SetCollisionMask(unsigned int mask);
			bool IsTrigger() const;

			bool CollidesWith(Vector2<float> pt);

			virtual void OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact);
			virtual bool OnCollisionTest(PhysicalObject *collider, Vector2<float> pointOfContact);

			virtual void OnSetTypeMask();
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
