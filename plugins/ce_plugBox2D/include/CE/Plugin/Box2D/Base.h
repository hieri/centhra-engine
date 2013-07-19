#ifndef _CE_PLUGIN_BOX2D_BASE_H_
#define _CE_PLUGIN_BOX2D_BASE_H_

namespace ce
{
	namespace plugin
	{
		class Box2D
		{
			static Box2D *ms_instance;

		public:
			static int Import();
			static int Initialize();
			static int Cleanup();
		};
	}
}

//- Import -
#define CE_IMPORT_PLUGIN_BOX2D int ce_plugBox2D_importMsg = ce::plugin::Box2D::Import();

#endif
