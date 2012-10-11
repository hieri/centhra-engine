#ifndef CE_O3DOBJECT_H
#define CE_O3DOBJECT_H

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/o3dEntity.h>
#include <CE/Quaternion.h>
#include <CE/Vector3.h>

namespace ce
{
	/* 	3d Object Class
	 * 	- Template class meant for emulating a 3d object.
	 */
	class o3dObject : public o3dEntity
	{
		bool m_isStatic, m_debugDraw;

	protected:
		Vector3<float> m_position, m_scale, m_velocity;
		Quaternion<float> m_rotation;

		virtual void onSetPosition();
		virtual void onSetRotation();
		virtual void onSetScale();
		virtual void onSetVelocity();

	public:
		o3dObject(o3dInstance *instance, Vector3<float> scale = Vector3<float>(1.f, 1.f, 1.f));

		bool getDebugDraw() const;
		Vector3<float> getPosition() const;
		Quaternion<float> getRotation() const;
		Vector3<float> getScale() const;
		Vector3<float> getVelocity() const;
		bool isStatic() const;
		virtual void render();
		void setDebugDraw(bool debugDraw);
		void setPosition(Vector3<float> position);
		void setStatic(bool isStatic);
		void setRotation(Quaternion<float> rotation);
		void setScale(Vector3<float> scale);
		void setVelocity(Vector3<float> velocity);
		virtual void update();
	};
}

#endif
