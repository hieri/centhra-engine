#ifndef _CE_GAME3D_DEFAULTPHYSICSHANDLER_H_
#define _CE_GAME3D_DEFAULTPHYSICSHANDLER_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector3.h>
#include <CE/Game3D/PhysicsHandler.h>

#define CE_OBJECTHANDLE_CACHESIZE 8

namespace ce
{
	namespace game3d
	{
		class DefaultPhysicsHandler : public PhysicsHandler
		{
			class ObjectHandle : public PhysicsHandler::ObjectHandle
			{
			public:
				ObjectHandle(PhysicsHandler *handler, PhysicalObject *object);
				~ObjectHandle();
			};
		};
	}
}

#endif
