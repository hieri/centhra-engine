#ifndef CE_O3DENTITY_H
#define CE_O3DENTITY_H

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/o3dInstance.h>

namespace ce
{
	/* 	3d Entity Class
	 * 	- Template class meant for emulating a 3d element.
	 */
	class o3dEntity
	{
		float m_timescale;
		o3dInstance *m_o3dInstance;

	protected:
		virtual void draw();
	
	public:
		o3dEntity(o3dInstance *instance);

		o3dInstance *getO3DInstance() const;
		virtual void render();
	};
}

#endif
