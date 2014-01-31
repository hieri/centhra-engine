#ifndef _CE_GAME3D_CAMERA_H_
#define _CE_GAME3D_CAMERA_H_

//- Centhra Engine -
#include <CE/Game3D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class Camera : public Entity
		{
			static Camera *ms_current;

			PhysicalObject *m_focus;

		public:
			static Camera *GetCurrent();

			Camera();

			void SetCurrent();
			PhysicalObject *GetFocus() const;
			void SetFocus(PhysicalObject *focus);
		};
	}
}

#endif
