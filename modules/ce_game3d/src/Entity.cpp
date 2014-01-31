//- Standard Library -
#include <algorithm>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game3D/Entity.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game3d
	{
		vector<Entity *> Entity::ms_alive, Entity::ms_dead;
		void Entity::DeleteDead()
		{
			for(vector<Entity *>::iterator it = ms_dead.begin(); it != ms_dead.end(); it++)
				delete *it;
			ms_dead.clear();
		}
		void Entity::Process(float dt)
		{
			for(vector<Entity *>::iterator it = ms_alive.begin(); it != ms_alive.end(); it++)
				(*it)->OnProcess(dt);
		}

		Entity::Entity()
		{
			m_isDead = false;
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
