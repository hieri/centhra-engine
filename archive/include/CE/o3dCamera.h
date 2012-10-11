#ifndef CE_O3DCAMERA_H
#define CE_O3DCAMERA_H

//- Centhra Engine -
#include <CE/o3dObject.h>
#include <CE/Vector3.h>

namespace ce
{
	class o3dCamera : public o3dObject
	{
		static void *ms_collisionShape;
		void *m_characterController, *m_motionState, *m_rigidBody;

	protected:
		float m_speed;
		Vector3<float> m_eulerRotation;

		virtual void draw();
		virtual void onSetPosition();
		virtual void onSetRotation();
		virtual void onSetVelocity();
		
	public:
		o3dCamera(o3dInstance *instance);
		~o3dCamera();

		void displayView();
		Vector3<float> getEulerRotation() const;
		float getSpeed() const;
		void setDebugDraw(bool debugDraw);
		void setEulerRotation(Vector3<float> rotation);
		void setSpeed(float speed);
		virtual void update();
	};
}

#endif
