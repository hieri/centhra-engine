#ifndef _CE_PLUGIN_TILED_BASE_H_
#define _CE_PLUGIN_TILED_BASE_H_

//- Centhra Engine -
#include <CE/Plugin.h>

namespace ce
{
	namespace plugin
	{
		class Tiled : public Plugin
		{
			static Tiled *ms_instance;

		protected:
			Tiled();

		public:
			static int Import();

			virtual int OnInitialize();
			virtual int OnCleanup();
		};
	}
}

//- Import -
#define CE_IMPORT_PLUGIN_TILED int ce_plugTiled_importMsg = ce::plugin::Tiled::Import();

#endif
