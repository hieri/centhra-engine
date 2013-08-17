//- Centhra Engine -
#include <CE/Game2D/PhysicsHandler.h>

namespace ce
{
	namespace game2d
	{
		PhysicsHandler::PhysicsHandler()
		{
			m_referenceGroup = 0;
		}
		PhysicsHandler::~PhysicsHandler()
		{
			if(m_referenceGroup)
				Cleanup();
		}
		void PhysicsHandler::Initialize()
		{
		}
		void PhysicsHandler::Process(float dt)
		{
		}
		void PhysicsHandler::Cleanup()
		{
		}

		PhysicsHandler::ObjectHandle::ObjectHandle(PhysicsHandler *handler, PhysicalObject *object)
		{
			m_physicsHandler = handler;
			m_object = object;
		}
		PhysicsHandler::ObjectHandle::~ObjectHandle()
		{
		}
		void PhysicsHandler::ObjectHandle::OnCreate()
		{
		}
		void PhysicsHandler::ObjectHandle::OnRelocate(PhysicalGroup *oldGroup, PhysicalGroup *newGroup)
		{
		}
		void PhysicsHandler::ObjectHandle::OnDestroy()
		{
		}
	}
}
