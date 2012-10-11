//- Standard Library -
#include <stdarg.h>
#include <stdio.h>

//- SDL2 -
#include <SDL.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/uiInstance.h>
#include <CE/uiView3d.h>

using namespace std;

namespace ce
{
	uiView3d::uiView3d(uiInstance *ui) : uiEntity(ui)
	{
		m_inputScale = Vector2<float>(1.f, 1.f);
		m_camera = 0;
		m_moveBackward = false;
		m_moveDown = false;
		m_moveForward = false;
		m_moveLeft = false;
		m_moveRight = false;
		m_moveUp = false;
	}
	void uiView3d::draw()
	{
		Vector2<int> position = getPosition();
		Vector2<int> size = getSize();

		getUIInstance()->prepare3d(position[0], position[1], size[0], size[1]);

		if(m_camera)
			m_camera->displayView();
		
		getUIInstance()->prepareUI();
	}
	o3dCamera *uiView3d::getCamera() const
	{
		return m_camera;
	}
	Vector2<float> uiView3d::getInputScale() const
	{
		return m_inputScale;
	}
	void uiView3d::onKeyPressed(unsigned int key, unsigned short mod)
	{
		switch(key)
		{
			case 'w':
				m_moveForward = true;
				break;
			case 'a':
				m_moveLeft = true;
				break;
			case 's':
				m_moveBackward = true;
				break;
			case 'd':
				m_moveRight = true;
				break;
			case ' ': // space
				m_moveUp = true;
				break;
			case 0x400000E1: // shift
				m_moveDown = true;
				break;
		}
		updateControl();
	}
	void uiView3d::onKeyReleased(unsigned int key, unsigned short mod)
	{
		switch(key)
		{
			case 'w':
				m_moveForward = false;
				break;
			case 'a':
				m_moveLeft = false;
				break;
			case 's':
				m_moveBackward = false;
				break;
			case 'd':
				m_moveRight = false;
				break;
			case ' ': // space
				m_moveUp = false;
				break;
			case 0x400000E1: // shift
				m_moveDown = false;
				break;
		}
		updateControl();
	}
	void uiView3d::onMouseMoved(int x, int y, int deltaX, int deltaY)
	{
		o3dCamera *cam = getCamera();
		if(cam)
		{
			Canvas *canvas = Canvas::getCurrent();
			if(canvas)
			{
				bool captureMouse = canvas->getCaptureMouse();
				Vector2<int> size = canvas->getSize();
				if(!captureMouse || (captureMouse && !(x == (size[0] / 2) && y == (size[1] / 2))))
				{
					Vector3<float> oldRotation = cam->getEulerRotation();
					Vector3<float> rotation = Vector3<float>(oldRotation[0] - (float)deltaY * m_inputScale[1], oldRotation[1] - (float)deltaX * m_inputScale[0], 0.f);

					if(rotation[0] > 90.f)
						rotation[0] = 90.f;
					else if(rotation [0] < -90.f)
						rotation[0] = -90.f;

					cam->setEulerRotation(rotation);
					updateControl();
				}
			}
		}
	}
	void uiView3d::setCamera(o3dCamera *camera)
	{
		m_camera = camera;
	}
	void uiView3d::setInputScale(Vector2<float> inputScale)
	{
		m_inputScale = inputScale;
	}
	void uiView3d::updateControl()
	{
		o3dCamera *cam = getCamera();
		if(cam)
		{
			Vector3<float> rotation = cam->getEulerRotation();

			float degToRad = atan(1.f) / 45.f;

			Vector3<float> cameraNormal(-sin(rotation[1] * degToRad) * fabs(cos(rotation[0] * degToRad)), sin(rotation[0] * degToRad), -cos(rotation[1] * degToRad) * fabs(cos(rotation[0] * degToRad)));
			cameraNormal.normalize();
			Vector3<float> inverseNormal(cos(rotation[1] * degToRad), 0.f, -sin(rotation[1] * degToRad));
			inverseNormal.normalize();
			Vector3<float> vertical(0.f, 1.f, 0.f);

			float speed = cam->getSpeed();
			Vector3<float> vel;
			if(m_moveForward)
				vel += cameraNormal;
			if(m_moveBackward)
				vel -= cameraNormal;
			if(m_moveLeft)
				vel -= inverseNormal;
			if(m_moveRight)
				vel += inverseNormal;
			if(m_moveUp)
				vel += vertical;
			if(m_moveDown)
				vel -= vertical;
			cam->setVelocity(vel.normalize() * speed);
		}
	}
}
