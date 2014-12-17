//- Standard Library -
#include <algorithm>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		vector<Entity *> Entity::ms_active, Entity::ms_deleted, Entity::ms_pending;
		bool g_doingProcess = false;
		Entity **g_markProcess, **g_endProcess;
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
			g_doingProcess = true;
			if(ms_active.empty() == false)
			{
				g_markProcess = &ms_active.front();
				g_endProcess = g_markProcess + ms_active.size();
				while(g_markProcess != g_endProcess)
					(*g_markProcess++)->OnProcess(dt);
			}
			g_doingProcess = false;
			if(ms_pending.empty() == false)
			{
				Entity **markEntities = &ms_pending.front();
				Entity **endEntities = markEntities + ms_pending.size();
				while(markEntities != endEntities)
					ms_active.push_back(*markEntities++);
				ms_pending.clear();
			}
		}

		Entity::Entity()
		{
			m_isDeleted = false;
			if(g_doingProcess)
				ms_pending.push_back(this);
			else
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
				if(g_doingProcess)
				{
					unsigned long place = g_markProcess - &ms_active.front() - 1;
					ms_active.erase(ms_active.begin() + place);
					if(ms_active.empty() == false)
					{
						g_markProcess = &ms_active.front();
						g_endProcess = g_markProcess + ms_active.size();
						g_markProcess += place;
					}
					else
					{
						g_markProcess = 0;
						g_endProcess = g_markProcess;
					}
				}
				else
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
