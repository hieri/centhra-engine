#ifndef _CE_PLUGIN_BOX2D_BASE_H_
#define _CE_PLUGIN_BOX2D_BASE_H_

//- Centhra Engine -
#include <CE/Plugin.h>

namespace ce
{
	namespace plugin
	{
		class Box2D : public Plugin
		{
			static Box2D *ms_instance;

		protected:
			Box2D();

		public:
			static int Import();

			virtual int Initialize();
			virtual int Cleanup();
		};
	}
}

//- Import -
#define CE_IMPORT_PLUGIN_BOX2D int ce_plugBox2D_importMsg = ce::plugin::Box2D::Import();

#endif
