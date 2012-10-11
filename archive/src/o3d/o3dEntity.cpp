//- Centhra Engine -
#include <CE/o3dEntity.h>

namespace ce
{
	o3dEntity::o3dEntity(o3dInstance *instance)
	{
		m_o3dInstance = instance;
		instance->add(this);
	}
	void o3dEntity::draw()
	{
	}
	o3dInstance *o3dEntity::getO3DInstance() const
	{
		return m_o3dInstance;
	}
	void o3dEntity::render()
	{
		draw();
	}
}
