//- Standard Library -
#include <algorithm>
#include <stdlib.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game3D/DefaultPhysicsHandler.h>
#include <CE/Game3D/PhysicalObject.h>

using namespace std;

namespace ce
{
	namespace game3d
	{
		DefaultPhysicsHandler::ObjectHandle::ObjectHandle(PhysicsHandler *handler, PhysicalObject *object) : PhysicsHandler::ObjectHandle(handler, object)
		{
		}
		DefaultPhysicsHandler::ObjectHandle::~ObjectHandle()
		{
		}
	}
}
