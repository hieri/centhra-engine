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
		vector<Entity *> Entity::ms_active, Entity::ms_deleted;
		void Entity::FinalizeDelete()
		{
			for(vector<Entity *>::iterator it = ms_deleted.begin(); it != ms_deleted.end(); it++)
				delete *it;
			ms_deleted.clear();
		}
		void Entity::Process(float dt)
		{
			for(vector<Entity *>::iterator it = ms_active.begin(); it != ms_active.end(); it++)
				(*it)->OnProcess(dt);
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
