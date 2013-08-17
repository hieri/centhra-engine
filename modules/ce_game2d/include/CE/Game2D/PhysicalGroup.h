#ifndef _CE_GAME2D_PHYSICALGROUP_H_
#define _CE_GAME2D_PHYSICALGROUP_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Group.h>
#include <CE/Game2D/Entity.h>

namespace ce
{
	namespace game2d
	{
		class PhysicalObject;
		class PhysicsHandler;

		class PhysicalGroup : public Entity, public Group
		{
			PhysicsHandler *m_physicsHandler;

		public:
			PhysicalGroup();

			void Render();

			void SetHandler(PhysicsHandler *handler);
			void CleanupHandler();
		};
	}
}

#endif
