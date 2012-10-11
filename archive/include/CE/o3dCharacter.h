#ifndef CE_O3DCHARACTER_H
#define CE_O3DCHARACTER_H

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/o3dObject.h>
#include <CE/Vector3.h>

namespace ce
{
	class o3dCharacter : public o3dObject
	{
		void *m_collisionShape, *m_motionState, *m_rigidBody;

	protected:
		virtual void draw();
		virtual void onSetPosition();
		virtual void onSetRotation();
		virtual void onSetScale();
		virtual void onSetVelocity();
		
	public:
		o3dCharacter(o3dInstance *instance, Vector3<float> scale = Vector3<float>(1.f, 1.f, 1.f));
		~o3dCharacter();

		void setDebugDraw(bool debugDraw);
		void setStatic(bool isStatic);
		virtual void update();
	};
}

#endif
