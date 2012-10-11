//- SDL2 -
#include <SDL_opengl.h>

//- Centhra Engine -
#include <CE/o3dObject.h>

namespace ce
{
	o3dObject::o3dObject(o3dInstance *instance, Vector3<float> scale) : o3dEntity(instance)
	{
		m_isStatic = false;
		m_debugDraw = false;
		m_scale = scale;
	}
	bool o3dObject::getDebugDraw() const
	{
		return m_debugDraw;
	}
	Vector3<float> o3dObject::getPosition() const
	{
		return m_position;
	}
	Quaternion<float> o3dObject::getRotation() const
	{
		return m_rotation;
	}
	Vector3<float> o3dObject::getScale() const
	{
		return m_scale;
	}
	Vector3<float> o3dObject::getVelocity() const
	{
		return m_velocity;
	}
	bool o3dObject::isStatic() const
	{
		return m_isStatic;
	}
	void o3dObject::onSetPosition()
	{
	}
	void o3dObject::onSetRotation()
	{
	}
	void o3dObject::onSetScale()
	{
	}
	void o3dObject::onSetVelocity()
	{
	}
	void o3dObject::render()
	{
		glPushMatrix();
			glColor4f(1.f, 1.f, 1.f, 1.f);
			glTranslatef(m_position.getX(), m_position.getY(), m_position.getZ());

			Matrix4x4<float> rotation = m_rotation.getTransposeMatrix4x4();
			glMultMatrixf(&rotation[0]);

			glPushMatrix();
				draw();
			glPopMatrix();
		glPopMatrix();
	}
	void o3dObject::setDebugDraw(bool debugDraw)
	{
		m_debugDraw = debugDraw;
	}
	void o3dObject::setPosition(Vector3<float> position)
	{
		m_position = position;
		onSetPosition();
	}
	void o3dObject::setRotation(Quaternion<float> rotation)
	{
		m_rotation = rotation;
		onSetRotation();
	}
	void o3dObject::setScale(Vector3<float> scale)
	{
		m_scale = scale;
		onSetScale();
	}
	void o3dObject::setStatic(bool isStatic)
	{
		m_isStatic = isStatic;
	}
	void o3dObject::setVelocity(Vector3<float> velocity)
	{
		m_velocity = velocity;
		onSetVelocity();
	}
	void o3dObject::update()
	{
	}
}
