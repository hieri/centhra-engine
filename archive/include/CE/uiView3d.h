#ifndef CE_UIVIEW3D_H
#define CE_UIVIEW3D_H

//- Centhra Engine -
#include <CE/o3dCamera.h>
#include <CE/uiEntity.h>
#include <CE/Vector2.h>

namespace ce
{
	/* 	UI View 3D Class
	 * 	- Displays an o3dInstance from the perspective of an o3dCamera.
	 */
	class uiView3d : public uiEntity
	{
		o3dCamera * m_camera;

	protected:
		bool m_moveBackward, m_moveDown, m_moveForward, m_moveLeft, m_moveRight, m_moveUp;
		Vector2<float> m_inputScale;

		void draw();
		void updateControl();

	public:
		uiView3d(uiInstance *ui);

		o3dCamera *getCamera() const;
		Vector2<float> getInputScale() const;
		void onKeyPressed(unsigned int key, unsigned short mod);
		void onKeyReleased(unsigned int key, unsigned short mod);
		void onMouseMoved(int x, int y, int deltaX, int deltaY);
		void setCamera(o3dCamera *camera);
		void setInputScale(Vector2<float> inputScale);
	};
}

#endif