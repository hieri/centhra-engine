//- Standard Library -
#include <algorithm>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game3D/Entity.h>

using namespace std;

namespace ce
{
	namespace game3d
	{
		vector<Entity *> Entity::ms_active, Entity::ms_deleted;
		void Entity::FinalizeDelete()
		{
			if(ms_deleted.empty() == false)
			{
				Entity **markEntities = &ms_deleted.front();
				Entity **endEntities = markEntities + ms_deleted.size();
				while(markEntities != endEntities)
					delete *markEntities++;
				ms_deleted.clear();
			}
		}
		void Entity::Process(float dt)
		{
/*			g_doingProcess = true;
			for(g_processIterator = ms_active.begin(); g_processIterator != ms_active.end(); g_processIterator++)
				(*g_processIterator)->OnProcess(dt);
			g_doingProcess = false;
			if(ms_pending.empty() == false)
			{
				Entity **markEntities = &ms_pending.front();
				Entity **endEntities = markEntities + ms_pending.size();
				while(markEntities != endEntities)
					ms_active.push_back(*markEntities++);
				ms_pending.clear();
			}*/
		}

		Entity::Entity()
		{
			m_isDeleted = false;
			ms_active.push_back(this);
		}
		Entity::~Entity()
		{
		}
		void Entity::Delete()
		{
			if(!m_isDeleted)
			{
				m_isDeleted = true;
				ms_active.erase(find(ms_active.begin(), ms_active.end(), this));
				ms_deleted.push_back(this);
				OnDelete();
			}
		}
		bool Entity::IsDeleted() const
		{
			return m_isDeleted;
		}
		void Entity::Render()
		{
			glPushMatrix();
			DoRender();
			glPopMatrix();
		}
		void Entity::DoRender()
		{
		}
		void Entity::OnDelete()
		{
		}
		void Entity::OnProcess(float dt)
		{
		}
	}
}
