//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/PhysicsHandler.h>

using namespace std;

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
		PhysicalGroup *PhysicsHandler::GetReferenceGroup() const
		{
			return m_referenceGroup;
		}
		void PhysicsHandler::Initialize()
		{
		}
		void PhysicsHandler::Render(float minX, float minY, float maxX, float maxY)
		{
		}
		void PhysicsHandler::Process(float dt)
		{
		}
		void PhysicsHandler::Cleanup()
		{
			m_referenceGroup = 0;
		}
		vector<PhysicalObject *> PhysicsHandler::BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask, PhysicalObject *ignore)
		{
			return vector<PhysicalObject *>();
		}
		vector<PhysicalObject *> PhysicsHandler::SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, PhysicalObject *ignore)
		{
			return vector<PhysicalObject *>();
		}
		vector<pair<PhysicalObject *, Vector2<float> > > PhysicsHandler::SpecialSegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, PhysicalObject *ignore)
		{
			return vector<pair<PhysicalObject *, Vector2<float> > >();
		}
		pair<PhysicalObject *, Vector2<float> > PhysicsHandler::RaycastSearch(float startX, float startY, float endX, float endY, unsigned int mask)
		{
			return pair<PhysicalObject *, Vector2<float> >();
		}

		void PhysicsHandler::SetupObject(PhysicalObject *object)
		{
		}
		void PhysicsHandler::CleanupObject(PhysicalObject *object)
		{
		}

		PhysicsHandler::ObjectHandle::ObjectHandle(PhysicsHandler *handler, PhysicalObject *object)
		{
			m_physicsHandler = handler;
			m_object = object;
			object->m_objectHandle = this;
			OnCreate();
		}
		PhysicsHandler::ObjectHandle::~ObjectHandle()
		{
			OnDestroy();
			if(m_object)
				m_object->m_objectHandle = 0;
		}
		PhysicalObject *PhysicsHandler::ObjectHandle::GetReferenceObject() const
		{
			return m_object;
		}
		PhysicsHandler *PhysicsHandler::ObjectHandle::GetHandler() const
		{
			return m_physicsHandler;
		}
		bool PhysicsHandler::ObjectHandle::CollidesWith(Vector2<float> pt)
		{
			return false;
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
		void PhysicsHandler::ObjectHandle::OnSetStatic()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetPosition()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetExtent()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetVelocity()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetRotation()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetTypeMask()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetCollisionMask()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetAngularVelocity()
		{
		}
		void PhysicsHandler::ObjectHandle::OnSetFixedRotation()
		{
		}

		//- Axis-Aligned Bounding Box -
		void PhysicsHandler::ObjectHandle::UpdateObjectAABB(Rect<float> aabb)
		{
			m_object->m_aabb = aabb;
		}
	}
}
