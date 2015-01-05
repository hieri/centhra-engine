#ifndef _CE_GAME2D_ENTITY_H_
#define _CE_GAME2D_ENTITY_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Group.h>

namespace ce
{
	namespace game2d
	{
		class Entity : public Group::Member
		{
			static std::vector<Entity *> ms_active, ms_deleted, ms_pending;
			bool m_isDeleted;

		protected:
			virtual void DoRender();
			virtual void OnDelete();
			virtual void OnProcess(float dt);

		public:
			static void FinalizeDelete();
			static void Process(float dt);

			Entity();
			virtual ~Entity();

			bool IsDeleted() const;
			void Delete();
			void Render();
		};
	}
}

#endif
