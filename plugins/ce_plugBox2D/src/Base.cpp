//- Centhra Engine -
#include <CE/Plugin/Box2D/Base.h>

namespace ce
{
	namespace plugin
	{
		Box2D *Box2D::ms_instance = 0;

		Box2D::Box2D()
		{
			m_name = "Box2D";
			m_type = Physics2D;
		}
		int Box2D::Import()
		{
			ms_instance = new Box2D();
			Register(ms_instance);
			return 0;
		}
		int Box2D::OnInitialize()
		{
			return 0;
		}
		int Box2D::OnCleanup()
		{
			return 0;
		}
	}
}
