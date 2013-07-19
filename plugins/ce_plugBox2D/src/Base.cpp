//- Centhra Engine -
#include <CE/Plugin/Box2D/Base.h>

namespace ce
{
	namespace plugin
	{
		Box2D *Box2D::ms_instance = 0;

		Box2D::Box2D()
		{
		}
		int Box2D::Import()
		{
			ms_instance = new Box2D();
			RegisterPlugin(1, "Box2D", ms_instance);
			return 0;
		}
		int Box2D::Initialize()
		{
			return 0;
		}
		int Box2D::Cleanup()
		{
			return 0;
		}
	}
}
