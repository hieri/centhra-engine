#ifndef _CE_PLUGIN_H_
#define _CE_PLUGIN_H_

//- Standard Library -
#include <map>

namespace ce
{
	class Plugin
	{
		bool m_isEnabled;

	protected:
		Plugin();

	public:
		virtual int Initialize();
		virtual int Cleanup();
	};

	int RegisterPlugin(unsigned long mask, const char *name, void *plugin);
}

#endif
