//- Centhra Engine 
#include <CE/Base.h>
#include <CE/RenderPrimitives.h>
#include <CE/Game2D/PhysicalObject.h>

//- Standard Library -
#include <algorithm>
#include <stdlib.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		unsigned int PhysicalObject::ms_lastID = -1;
		PhysicalObject::PhysicalObject(Vector2<float> position, Vector2<float> extent)
		{
			m_rotation = 0.f;
			m_position = position;
			m_extent = extent;
			m_color = Color(rand() % 256,  rand() % 256, rand() % 256);
			m_velocity = Vector2<float>(0.f, 0.f);
			m_collisionMask = 1;

			m_objectHandle = 0;

			ms_lastID++;
			m_id = ms_lastID;

			m_parentGroup = 0;
			m_isTrigger = m_isStatic = false;
		}
		PhysicalObject::~PhysicalObject()
		{
			if(m_parentGroup)
				m_parentGroup->Remove(this);
		}
		void PhysicalObject::Render()
		{
			glPushMatrix();
			DoRender();
			glPopMatrix();
		}
		void PhysicalObject::DoRender()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4ub(m_color[0], m_color[1], m_color[2], m_color[3]);
//			glTranslatef(m_position[0], m_position[1], -m_position[1]);
			glTranslatef(m_position[0], m_position[1], 0.f);
			glRotatef(m_rotation, 0.f, 0.f, 1.f);
			glScalef(m_extent[0], m_extent[1], 1.f);
			glTranslatef(-0.5f, -0.5f, 0.f);
			RenderSquare();
			glColor3ub(255, 255, 255);

			glDisable(GL_BLEND);
		}
		Vector2<float> PhysicalObject::GetExtent() const
		{
			return m_extent;
		}
		Vector2<float> PhysicalObject::GetPosition() const
		{
			return m_position;
		}
		Vector2<float> PhysicalObject::GetVelocity() const
		{
			return m_velocity;
		}
		float PhysicalObject::GetRotation() const
		{
			return m_rotation;
		}
		void PhysicalObject::SetExtent(Vector2<float> extent, bool updateHandle)
		{
			m_extent = extent;
			OnSetExtent();
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetExtent();
		}
		void PhysicalObject::SetPosition(Vector2<float> position, bool updateHandle)
		{
			m_position = position;
			OnSetPosition();

			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetPosition();
		}
		void PhysicalObject::SetVelocity(Vector2<float> velocity, bool updateHandle)
		{
			m_velocity = velocity;
			OnSetVelocity();
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetVelocity();
		}
		void PhysicalObject::SetRotation(float rotation, bool updateHandle)
		{
			m_rotation = rotation;
			OnSetRotation();
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetRotation();
		}
		PhysicsHandler::ObjectHandle *PhysicalObject::GetObjectHandle() const
		{
			return m_objectHandle;
		}
		bool PhysicalObject::IsStatic() const
		{
			return m_isStatic;
		}
		unsigned int PhysicalObject::GetCollisionMask() const
		{
			return m_collisionMask;
		}
		void PhysicalObject::SetCollisionMask(unsigned int mask)
		{
			m_collisionMask = mask;
			OnSetCollisionMask();

			if(m_objectHandle)
				m_objectHandle->OnSetCollisionMask();
		}
		bool PhysicalObject::IsTrigger() const
		{
			return m_isTrigger;
		}
		bool PhysicalObject::OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact)
		{
			return true;
		}
		void PhysicalObject::SetStatic(bool isStatic)
		{
			m_isStatic = isStatic;
			OnSetStatic();

			if(m_objectHandle)
				m_objectHandle->OnSetStatic();
		}
		void PhysicalObject::OnSetCollisionMask()
		{
		}
		void PhysicalObject::OnSetStatic()
		{
		}
		void PhysicalObject::OnSetPosition()
		{
		}
		void PhysicalObject::OnSetExtent()
		{
		}
		void PhysicalObject::OnSetVelocity()
		{
		}
		void PhysicalObject::OnSetRotation()
		{
		}
	}
}
