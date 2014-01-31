#ifndef _CE_GAME3D_ENTITY_H_
#define _CE_GAME3D_ENTITY_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Group.h>

namespace ce
{
	namespace game3d
	{
		class Entity : public Group::Member
		{
			static std::vector<Entity *> ms_alive, ms_dead;

			bool m_isDead;

		protected:
			virtual void DoRender();
			virtual void OnKill();

		public:
			static void DeleteDead();
			static void Process(float dt);

			Entity();
			~Entity();

			bool IsDead() const;
			void Kill();
			void Render();

			virtual void OnProcess(float dt);
		};
	}
}

#endif
