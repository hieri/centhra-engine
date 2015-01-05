//- Standard Library -
#include <algorithm>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/PhysicalGroup.h>
#include <CE/Game2D/PhysicalObject.h>
#include <CE/Game2D/World.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		PhysicalGroup::PhysicalGroup() : m_physicsHandler(0), m_renderDebug(false)
		{
		}
		PhysicalGroup::~PhysicalGroup()
		{
			DetachHandler();
			while(m_members.size())
				delete (PhysicalObject *)m_members.back();
		}
		void PhysicalGroup::ProcessPhysics(float dt)
		{
			if(m_physicsHandler)
				m_physicsHandler->Process(dt);
		}
		void PhysicalGroup::Render(float minX, float minY, float maxX, float maxY, RenderContext &context)
		{
			if(m_members.empty() == false)
			{
				Group::Member **markObjects = &m_members.front();
				Group::Member **endObjects = markObjects + m_members.size();
				while(markObjects != endObjects)
				{
					PhysicalObject *object = (PhysicalObject *)*markObjects++;
					if(object->m_aabb[0] > maxX || object->m_aabb[1] > maxY || object->m_aabb[2] < minX || object->m_aabb[3] < minY)
						continue;
					object->Render(context);
					if(m_renderDebug)
						object->RenderAABB(context);
				}
			}

			if(m_renderDebug)
				if(m_physicsHandler)
					m_physicsHandler->Render(minX, minY, maxX, maxY);
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
			}
			m_physicsHandler = 0;
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

		//- Debug Rendering -
		bool PhysicalGroup::IsRenderingDebug() const
		{
			return m_renderDebug;
		}
		void PhysicalGroup::SetDebugRendering(bool renderDebug)
		{
			m_renderDebug = renderDebug;
		}
	}
}
