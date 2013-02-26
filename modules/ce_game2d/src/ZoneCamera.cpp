//- Centhra Engine -
#include <CE/Game2D/ZoneCamera.h>

namespace ce
{
	namespace game2d
	{
		ZoneCamera::ZoneCamera()
		{
			m_focus = 0;
		}
		ZoneEntity *ZoneCamera::GetFocus() const
		{
			return m_focus;
		}
		void ZoneCamera::SetFocus(ZoneEntity *focus)
		{
			m_focus = focus;
		}
	}
}
