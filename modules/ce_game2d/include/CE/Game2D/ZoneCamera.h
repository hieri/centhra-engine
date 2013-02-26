#ifndef _CE_GAME2D_ZONECAMERA_H_
#define _CE_GAME2D_ZONECAMERA_H_

//- Centhra Engine -
#include <CE/Game2D/ZoneEntity.h>

namespace ce
{
	namespace game2d
	{
		class ZoneCamera : public Entity
		{
			ZoneEntity *m_focus;

		public:
			ZoneCamera();

			ZoneEntity *GetFocus() const;
			void SetFocus(ZoneEntity *focus);
		};
	}
}

#endif
