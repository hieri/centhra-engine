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
		void PhysicalGroup::Render()
		{
			for(vector<Group::Member *>::iterator it = m_members.begin(); it != m_members.end(); it++)
			{
				PhysicalObject *object = (PhysicalObject *)*it;
				object->Render();
			}
		}
		void PhysicalGroup::SetHandler(PhysicsHandler *handler)
		{
			if(m_physicsHandler != handler)
			{
				m_physicsHandler = handler;
			}
		}
		void PhysicalGroup::CleanupHandler()
		{
		}
	}
}
