//- Centhra Engine -
#include <CE/Plugin.h>

namespace ce
{
	Plugin::Plugin()
	{
		m_isEnabled = true;
	}

	int RegisterPlugin(unsigned long mask, const char *name, void *plugin)
	{
		return 0;
	}
}
