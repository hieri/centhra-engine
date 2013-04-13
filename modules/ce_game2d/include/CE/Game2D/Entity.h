#ifndef _CE_GAME2D_ENTITY_H_
#define _CE_GAME2D_ENTITY_H_

//- Standard Library -
#include <vector>

namespace ce
{
	namespace game2d
	{
		class Entity
		{
			static std::vector<Entity *> ms_dead;

			bool m_isDead;

		protected:
			virtual void DoRender();

		public:
			static void DeleteDead();

			Entity();

			bool IsDead() const;
			void Kill();
			void Render();
		};
	}
}

#endif
