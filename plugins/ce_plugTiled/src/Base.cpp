//- Centhra Engine -
#include <CE/Plugin/Tiled/Base.h>

namespace ce
{
	namespace plugin
	{
		Tiled *Tiled::ms_instance = 0;

		Tiled::Tiled()
		{
			m_name = "Tiled";
			m_type = Map;
		}
		int Tiled::Import()
		{
			ms_instance = new Tiled();
			Register(ms_instance);
			return 0;
		}
		int Tiled::OnInitialize()
		{
			return 0;
		}
		int Tiled::OnCleanup()
		{
			return 0;
		}
	}
}
