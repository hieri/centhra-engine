//- SDL2 -
#include <SDL_opengl.h>

//- Bullet Physics -
#include <btBulletDynamicsCommon.h>

//- Centhra Engine -
#include <CE/o3dBox.h>

namespace ce
{
	o3dBox::o3dBox(o3dInstance *instance, Vector3<float> scale) : o3dObject(instance, scale)
	{
		m_scale = scale;

		///create a few basic rigid bodies
		btCollisionShape *collisionShape = new btBoxShape(btVector3(scale[0] / 2.f, scale[1] / 2.f, scale[2] / 2.f));
		m_collisionShape = (void *)collisionShape;

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(0, 0, 0));

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		btVector3 localInertia(0, 0, 0);
		collisionShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState *motionState = new btDefaultMotionState(transform);
		m_motionState = (void *)motionState;

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
		btRigidBody *rigidBody = new btRigidBody(rbInfo);
		rigidBody->setCollisionFlags(btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		m_rigidBody = (void *)rigidBody;

		rigidBody->setUserPointer((void *)this);
		getO3DInstance()->addRigidBody(m_rigidBody, COL_STANDARD, COL_STANDARD);
	}
	o3dBox::~o3dBox()
	{
		getO3DInstance()->removeRigidBody(m_rigidBody);

		btCollisionShape *collisionShape = (btCollisionShape *)m_collisionShape;
		btDefaultMotionState *motionState = (btDefaultMotionState *)m_motionState;
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;

		delete collisionShape;
		delete motionState;
		delete rigidBody;
	}
	void o3dBox::draw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4ub(m_color[0], m_color[1], m_color[2], m_color[3]);
		glScalef(m_scale[0], m_scale[1], m_scale[2]);

		glBegin(GL_QUADS);

			glVertex3f(-0.5f, -0.5f, -0.5f);
			glVertex3f(-0.5f,  0.5f, -0.5f);
			glVertex3f( 0.5f,  0.5f, -0.5f);
			glVertex3f( 0.5f, -0.5f, -0.5f);

			glVertex3f(-0.5f, -0.5f, 0.5f);
			glVertex3f(-0.5f,  0.5f, 0.5f);
			glVertex3f( 0.5f,  0.5f, 0.5f);
			glVertex3f( 0.5f, -0.5f, 0.5f);

			glVertex3f(-0.5f, -0.5f, -0.5f);
			glVertex3f(-0.5f,  0.5f, -0.5f);
			glVertex3f(-0.5f,  0.5f,  0.5f);
			glVertex3f(-0.5f, -0.5f,  0.5f);

			glVertex3f(0.5f, -0.5f, -0.5f);
			glVertex3f(0.5f,  0.5f, -0.5f);
			glVertex3f(0.5f,  0.5f,  0.5f);
			glVertex3f(0.5f, -0.5f,  0.5f);

			glVertex3f(-0.5f, -0.5f,  0.5f);
			glVertex3f(-0.5f, -0.5f, -0.5f);
			glVertex3f( 0.5f, -0.5f, -0.5f);
			glVertex3f( 0.5f, -0.5f,  0.5f);

			glVertex3f(-0.5f, 0.5f,  0.5f);
			glVertex3f(-0.5f, 0.5f, -0.5f);
			glVertex3f( 0.5f, 0.5f, -0.5f);
			glVertex3f( 0.5f, 0.5f,  0.5f);

		glEnd();
		
		glDisable(GL_BLEND);
	}
	Color o3dBox::getColor() const
	{
		return m_color;
	}
	void o3dBox::onSetPosition()
	{
		btDefaultMotionState *motionState = (btDefaultMotionState *)m_motionState;
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;

		btTransform transform;
		motionState->getWorldTransform(transform);

		btVector3 origin = transform.getOrigin();
		origin.setX(m_position.getX());
		origin.setY(m_position.getY());
		origin.setZ(m_position.getZ());
		transform.setOrigin(origin);

		motionState->setWorldTransform(transform);
		rigidBody->setMotionState(motionState);
	}
	void o3dBox::onSetRotation()
	{
		btDefaultMotionState *motionState = (btDefaultMotionState *)m_motionState;
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;

		btTransform transform;
		motionState->getWorldTransform(transform);

		btQuaternion rot = transform.getRotation();
		btQuaternion newRot(m_rotation[1], m_rotation[2], m_rotation[3], m_rotation[0]);
		rot *= newRot;
		transform.setRotation(rot);

		motionState->setWorldTransform(transform);
		rigidBody->setMotionState(motionState);
	}
	void o3dBox::onSetScale()
	{
		btDefaultMotionState *motionState = (btDefaultMotionState *)m_motionState;
		getO3DInstance()->removeRigidBody(m_rigidBody);

		delete (btRigidBody *)m_rigidBody;
		delete (btCollisionShape *)m_collisionShape;
		
		btCollisionShape *collisionShape = new btBoxShape(btVector3(m_scale[0] / 2.f, m_scale[1] / 2.f, m_scale[2] / 2.f));
		m_collisionShape = (void *)collisionShape;

		float mass = isStatic() ? 0.f : 1.f;

		btVector3 localInertia(0, 0, 0);
		collisionShape->calculateLocalInertia(mass, localInertia);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, collisionShape, localInertia);
		btRigidBody *rigidBody = new btRigidBody(rbInfo);
		rigidBody->setCollisionFlags(btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		m_rigidBody = (void *)rigidBody;

		rigidBody->setUserPointer((void *)this);
		getO3DInstance()->addRigidBody(m_rigidBody, COL_STANDARD, COL_STANDARD);
	}
	void o3dBox::onSetVelocity()
	{
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;
		rigidBody->setLinearVelocity(btVector3(m_velocity[0], m_velocity[1], m_velocity[2]));
	}
	void o3dBox::setColor(Color color)
	{
		m_color = color;
	}
	void o3dBox::setDebugDraw(bool debugDraw)
	{
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;
		rigidBody->setCollisionFlags(debugDraw ? 0 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	}
	void o3dBox::setStatic(bool isStatic)
	{
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;
		rigidBody->setMassProps(btScalar(isStatic ? 0.f : 1.f), btVector3(0, 0, 0));
		rigidBody->setCollisionFlags(getDebugDraw() ? 0 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	}
	void o3dBox::update()
	{
		btDefaultMotionState *motionState = (btDefaultMotionState *)m_motionState;

		btTransform transform;
		motionState->getWorldTransform(transform);

		float x = transform.getOrigin().getX();
		float y = transform.getOrigin().getY();
		float z = transform.getOrigin().getZ();

		m_position = Vector3<float>(x, y, z);

		btQuaternion rot = transform.getRotation();

		m_rotation = Quaternion<float>(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
		m_rotation.normalize();
	}
}
