#ifndef _CE_GAME2D_PHYSICALOBJECT_H_
#define _CE_GAME2D_PHYSICALOBJECT_H_

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Rect.h>
#include <CE/Vector2.h>
#include <CE/Game2D/Entity.h>
#include <CE/Game2D/PhysicsHandler.h>
#include <CE/Game2D/World.h>

namespace ce
{
	namespace game2d
	{
		typedef enum ObjectTypeMask
		{
			Base_Object     = 0,
			Mask_Object     = 1 << (Base_Object + 0),
			Mask_Wall       = 1 << (Base_Object + 1),
			Mask_Prop       = 1 << (Base_Object + 2),
			Mask_Projectile = 1 << (Base_Object + 3),
			Next_Object     = 4
		} ObjectTypeMask;

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
			bool m_isTrigger, m_isStatic, m_hasFixedRotation;

			virtual void DoRender();

		public:
			static unsigned short DEFAULT_ID;
			static PhysicalObject *GetObjectByID(unsigned short id);
			static PhysicalObject *GetNetObjectByID(unsigned short id);

			PhysicalObject(Vector2<float> position, Vector2<float> extent, unsigned short id = DEFAULT_ID, unsigned short netID = DEFAULT_ID, bool noID = false);
			virtual ~PhysicalObject();

			void Render();
			void RenderAABB();
			bool IsStatic() const;
			bool HasFixedRotation() const;
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
			void SetFixedRotation(bool fixedRotation);

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
			virtual void OnSetFixedRotation();

			friend class PhysicalGroup;
			friend class PhysicsHandler::ObjectHandle;

			//- Axis-Aligned Bounding Box -
		protected:
			Rect<float> m_aabb;
		public:
			Rect<float> GetAABB() const;

			//- Toggle Collision -
		protected:
			bool m_isCollidable;
			unsigned int m_actualCollisionMask;
		public:
			void SetCollidable(bool collidable);
			bool IsCollidable() const;

			//- Layered Rendering -
		protected:
			World::ObjectLayer *m_renderLayer;
		public:
			World::ObjectLayer *GetRenderLayer() const;
			void SetRenderLayer(World::ObjectLayer *layer);
		};
	}
}

#endif
