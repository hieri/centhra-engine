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
			static std::vector<Entity *> ms_active, ms_deleted;

			bool m_isDeleted;

		protected:
			virtual void DoRender();
			virtual void OnDelete();

		public:
			static void FinalizeDelete();
			static void Process(float dt);

			Entity();
			~Entity();

			bool IsDeleted() const;
			void Delete();
			void Render();

			virtual void OnProcess(float dt);
		};
	}
}

#endif
