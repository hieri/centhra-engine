//- Standard Library -
#include <algorithm>

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Base.h>

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
		vector<Entity *> Entity::ms_active, Entity::ms_deleted, Entity::ms_pending;
		bool g_doingProcess = false;
		vector<Entity *>::iterator g_processIterator;
		void Entity::FinalizeDelete()
		{
			for(vector<Entity *>::iterator it = ms_deleted.begin(); it != ms_deleted.end(); it++)
				delete *it;
			ms_deleted.clear();
		}
		void Entity::Process(float dt)
		{
			g_doingProcess = true;
			for(g_processIterator = ms_active.begin(); g_processIterator != ms_active.end(); g_processIterator++)
				(*g_processIterator)->OnProcess(dt);
			g_doingProcess = false;
			for(vector<Entity *>::iterator it = ms_pending.begin(); it != ms_pending.end(); it++)
				ms_active.push_back(*it);
			ms_pending.clear();
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
					g_processIterator = ms_active.erase(g_processIterator);
					if(g_processIterator != ms_active.begin())
						g_processIterator--;
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
