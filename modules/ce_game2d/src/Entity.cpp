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
		vector<Entity *> Entity::ms_alive, Entity::ms_dead, Entity::ms_pending;
		bool g_doingProcess = false;
		vector<Entity *>::iterator g_processIterator;
		void Entity::DeleteDead()
		{
			for(vector<Entity *>::iterator it = ms_dead.begin(); it != ms_dead.end(); it++)
				delete *it;
			ms_dead.clear();
		}
		void Entity::Process(float dt)
		{
			g_doingProcess = true;
			for(g_processIterator = ms_alive.begin(); g_processIterator != ms_alive.end(); g_processIterator++)
				(*g_processIterator)->OnProcess(dt);
			g_doingProcess = false;
			for(vector<Entity *>::iterator it = ms_pending.begin(); it != ms_pending.end(); it++)
				ms_alive.push_back(*it);
			ms_pending.clear();
		}

		Entity::Entity()
		{
			m_isDead = false;
			if(g_doingProcess)
				ms_pending.push_back(this);
			else
				ms_alive.push_back(this);
		}
		Entity::~Entity()
		{
		}
		void Entity::Kill()
		{
			if(!m_isDead)
			{
				m_isDead = true;
				if(g_doingProcess)
				{
					g_processIterator = ms_alive.erase(g_processIterator);
					if(g_processIterator != ms_alive.begin())
						g_processIterator--;
				}
				else
					ms_alive.erase(find(ms_alive.begin(), ms_alive.end(), this));
				ms_dead.push_back(this);
				OnKill();
			}
		}
		bool Entity::IsDead() const
		{
			return m_isDead;
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
		void Entity::OnKill()
		{
		}
		void Entity::OnProcess(float dt)
		{
		}
	}
}
