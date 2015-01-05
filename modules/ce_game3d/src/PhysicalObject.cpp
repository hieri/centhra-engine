//- Standard Library -
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine 
#include <CE/Base.h>
#include <CE/Game3D/PhysicalObject.h>

using namespace std;

namespace ce
{
	namespace game3d
	{
		unsigned int PhysicalObject::ms_lastID = -1;
		PhysicalObject::PhysicalObject(Vector3<float> position, Vector3<float> size)
		{
			m_position = position;
			m_size = size;
			m_color = Color(rand() % 256,  rand() % 256, rand() % 256);
			m_velocity = Vector3<float>(0.f, 0.f, 0.f);
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
		}
		Vector3<float> PhysicalObject::GetSize() const
		{
			return m_size;
		}
		Vector3<float> PhysicalObject::GetPosition() const
		{
			return m_position;
		}
		Vector3<float> PhysicalObject::GetVelocity() const
		{
			return m_velocity;
		}
		Quaternion<float> PhysicalObject::GetRotation() const
		{
			return m_rotation;
		}
		void PhysicalObject::SetSize(Vector3<float> size, bool updateHandle)
		{
			m_size = size;
			OnSetSize();
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetSize();
		}
		void PhysicalObject::SetPosition(Vector3<float> position, bool updateHandle)
		{
			m_position = position;
			OnSetPosition();

			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetPosition();
		}
		void PhysicalObject::SetVelocity(Vector3<float> velocity, bool updateHandle)
		{
			m_velocity = velocity;
			OnSetVelocity();
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetVelocity();
		}
		void PhysicalObject::SetRotation(Quaternion<float> rotation, bool updateHandle)
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
//		bool PhysicalObject::OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact)
//		{
//			return true;
//		}
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
		void PhysicalObject::OnSetSize()
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
