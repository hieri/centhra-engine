//- Standard Library -
#include <algorithm>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/PhysicalObject.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		PhysicalGroup::PhysicalGroup()
		{
			m_physicsHandler = 0;
		}
		void PhysicalGroup::ProcessPhysics(float dt)
		{
			if(m_physicsHandler)
				m_physicsHandler->Process(dt);
		}
		void PhysicalGroup::Render(float minX, float minY, float maxX, float maxY)
		{
			if(m_physicsHandler)
				m_physicsHandler->Render(minX, minY, maxX, maxY);
			else //TODO: Determine if this is necessary
			{
				for(vector<Group::Member *>::iterator it = m_members.begin(); it != m_members.end(); it++)
				{
					PhysicalObject *object = (PhysicalObject *)*it;
					object->Render();
				}
			}
		}
		void PhysicalGroup::AttachHandler(PhysicsHandler *handler)
		{
			if(m_physicsHandler != handler)
			{
				DetachHandler();

				m_physicsHandler = handler;
				handler->m_referenceGroup = this;
				handler->Initialize();
			}
		}
		PhysicsHandler *PhysicalGroup::GetPhysicsHandler() const
		{
			return m_physicsHandler;
		}
		void PhysicalGroup::DetachHandler()
		{
			if(m_physicsHandler)
			{
				m_physicsHandler->Cleanup();
				m_physicsHandler = 0;
			}
		}
		vector<PhysicalObject *> PhysicalGroup::BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask, PhysicalObject *ignore)
		{
			vector<PhysicalObject *> ret;
			if(m_physicsHandler)
				ret = m_physicsHandler->BoxSearch(minX, minY, maxX, maxY, mask, ignore);
			return ret;
		}
		vector<PhysicalObject *> PhysicalGroup::SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, PhysicalObject *ignore)
		{
			vector<PhysicalObject *> ret;
			if(m_physicsHandler)
				ret = m_physicsHandler->SegmentSearch(startX, startY, endX, endY, mask, ignore);
			return ret;
		}
		vector<pair<PhysicalObject *, Vector2<float> > > PhysicalGroup::SpecialSegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask, PhysicalObject *ignore)
		{
			vector<pair<PhysicalObject *, Vector2<float> > > ret;
			if(m_physicsHandler)
				ret = m_physicsHandler->SpecialSegmentSearch(startX, startY, endX, endY, mask, ignore);
			return ret;
		}
		pair<PhysicalObject *, Vector2<float> > PhysicalGroup::RaycastSearch(float startX, float startY, float endX, float endY, unsigned int mask)
		{
			pair<PhysicalObject *, Vector2<float> > ret;
			if(m_physicsHandler)
				ret = m_physicsHandler->RaycastSearch(startX, startY, endX, endY, mask);
			return ret;
		}

		void PhysicalGroup::OnMemberAdded(Group::Member *entity)
		{
			if(m_physicsHandler)
				m_physicsHandler->SetupObject((PhysicalObject *)entity);
		}
		void PhysicalGroup::OnMemberRemoved(Group::Member *entity)
		{
			if(m_physicsHandler)
				m_physicsHandler->CleanupObject((PhysicalObject *)entity);
		}
	}
}
