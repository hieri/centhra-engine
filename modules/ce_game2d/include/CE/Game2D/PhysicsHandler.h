#ifndef _CE_GAME2D_PHYSICSHANDLER_H_
#define _CE_GAME2D_PHYSICSHANDLER_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class PhysicsHandler
		{
		public:
			class ObjectHandler
			{
				PhysicalObject *m_object;
			};

			void Process(float dt);
		};
	}
}

#endif
