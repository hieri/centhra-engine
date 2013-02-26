#ifndef _CE_GAME2D_ENTITY_H_
#define _CE_GAME2D_ENTITY_H_

namespace ce
{
	namespace game2d
	{
		class Entity
		{
		protected:
			void DoRender();

		public:
			Entity();

			void Render();
		};
	}
}

#endif
