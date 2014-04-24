#ifndef _CE_BASE_PLUGIN_H_
#define _CE_BASE_PLUGIN_H_

//- Standard Library -
#include <string>

namespace ce
{
	class Plugin
	{
	protected:
		bool m_isRunning;
		std::string m_name;
		unsigned int m_type;

		Plugin();
		~Plugin();

	public:
		typedef enum PluginType
		{
			Unknown,
			Image,
			Audio,
			Video,
			Physics2D,
			Physics3D,
			Map
		} PluginType;

		static int Register(Plugin *plugin);
		static int InitializeByType(unsigned int type);
		static int CleanupByType(unsigned int type);
		static int DeleteByType(unsigned int type);

		bool IsRunning() const;

		int Initialize();
		int Cleanup();

		virtual int OnInitialize();
		virtual int OnCleanup();
	};
}

#endif
