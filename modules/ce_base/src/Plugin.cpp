//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Plugin.h>

using namespace std;

#define CE_PLUGIN_NUMTYPES 6

namespace ce
{
	vector<Plugin *> g_pluginsByType[CE_PLUGIN_NUMTYPES];

	int Plugin::Register(Plugin *plugin)
	{
		if(plugin->m_type >= CE_PLUGIN_NUMTYPES)
			return 0;

		g_pluginsByType[plugin->m_type].push_back(plugin);
		return 0;
	}
	int Plugin::InitializeByType(unsigned int type)
	{
		if(type >= CE_PLUGIN_NUMTYPES)
			return 0;

		vector<Plugin *> &pluginVector = g_pluginsByType[type];
		for(vector<Plugin *>::iterator it = pluginVector.begin(); it != pluginVector.end(); it++)
			(*it)->Initialize();

		return 0;
	}
	int Plugin::CleanupByType(unsigned int type)
	{
		if(type >= CE_PLUGIN_NUMTYPES)
			return 0;

		vector<Plugin *> &pluginVector = g_pluginsByType[type];
		for(vector<Plugin *>::iterator it = pluginVector.begin(); it != pluginVector.end(); it++)
			(*it)->Cleanup();

		return 0;
	}
	int Plugin::DeleteByType(unsigned int type)
	{
		if(type >= CE_PLUGIN_NUMTYPES)
			return 0;

		vector<Plugin *> &pluginVector = g_pluginsByType[type];
		for(vector<Plugin *>::iterator it = pluginVector.begin(); it != pluginVector.end(); it++)
			delete *it;
		pluginVector.clear();

		return 0;
	}

	Plugin::Plugin()
	{
		m_isRunning = false;
		m_type = Unknown;
	}
	Plugin::~Plugin()
	{
		//TODO: Add force cleanup?
		Cleanup();
	}
	bool Plugin::IsRunning() const
	{
		return m_isRunning;
	}
	int Plugin::Initialize()
	{
		if(m_isRunning)
			return 0;
		int result = OnInitialize();
		if(!result)
			m_isRunning = true;
		return result;
	}
	int Plugin::Cleanup()
	{
		if(!m_isRunning)
			return 0;
		int result = OnCleanup();
		if(!result)
			m_isRunning = false;
		return result;
	}
	int Plugin::OnInitialize()
	{
		return 0;
	}
	int Plugin::OnCleanup()
	{
		return 0;
	}
}
