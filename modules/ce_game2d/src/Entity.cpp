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
		vector<Entity *> Entity::ms_dead;
		void Entity::DeleteDead()
		{
			for(vector<Entity *>::iterator it = ms_dead.begin(); it != ms_dead.end(); it++)
				delete *it;
			ms_dead.clear();
		}

		Entity::Entity()
		{
			m_isDead = false;
		}
		Entity::~Entity()
		{
		}
		void Entity::Kill()
		{
			if(!m_isDead)
			{
				m_isDead = true;
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
	}
}
