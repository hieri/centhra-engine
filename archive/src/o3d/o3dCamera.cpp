//- SDL2 -
#include <SDL_opengl.h>

//- Bullet Physics -
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

//- Centhra Engine -
#include <CE/o3dCamera.h>

namespace ce
{
	void *o3dCamera::ms_collisionShape = 0;
	o3dCamera::o3dCamera(o3dInstance *instance) : o3dObject(instance)
	{
		m_speed = 1.f;

		btCollisionShape *collisionShape;
		if(!ms_collisionShape)
		{
			///create a few basic rigid bodies
			collisionShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
			ms_collisionShape = (void *)collisionShape;
		}
		else
			collisionShape = (btCollisionShape *)ms_collisionShape;

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(0, 0, 0));

		btScalar mass(0.f);

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

		btKinematicCharacterController *characterController = new btKinematicCharacterController((btPairCachingGhostObject *)rigidBody, (btConvexShape *)collisionShape, 0.f);
		m_characterController = characterController;

		rigidBody->setUserPointer((void *)this);
		getO3DInstance()->addRigidBody(m_rigidBody, COL_GHOST, COL_NONE);
	}
	o3dCamera::~o3dCamera()
	{
		getO3DInstance()->removeRigidBody(m_rigidBody);

		btDefaultMotionState *motionState = (btDefaultMotionState *)m_motionState;
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;
		btKinematicCharacterController *characterController = (btKinematicCharacterController *)m_characterController;

		delete characterController;
		delete motionState;
		delete rigidBody;
	}
	void o3dCamera::setDebugDraw(bool debugDraw)
	{
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;
		rigidBody->setCollisionFlags(debugDraw ? 0 : btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
	}
	void o3dCamera::displayView()
	{
		glPushMatrix();

			Matrix4x4<float> rotation = m_rotation.getMatrix4x4();
			glMultMatrixf(&rotation[0]);
			
			glTranslatef(-m_position.getX(), -m_position.getY(), -m_position.getZ());

			getO3DInstance()->render();

		glPopMatrix();
	}
	void o3dCamera::draw()
	{
	}
	Vector3<float> o3dCamera::getEulerRotation() const
	{
		return m_eulerRotation;
	}
	float o3dCamera::getSpeed() const
	{
		return m_speed;
	}
	void o3dCamera::onSetPosition()
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
	void o3dCamera::onSetRotation()
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
	void o3dCamera::onSetVelocity()
	{
		btRigidBody *rigidBody = (btRigidBody *)m_rigidBody;
		rigidBody->setLinearVelocity(btVector3(m_velocity[0], m_velocity[1], m_velocity[2]));
		rigidBody->activate();
	}
	void o3dCamera::setEulerRotation(Vector3<float> rotation)
	{
		m_eulerRotation = rotation;
		m_rotation = Quaternion<float>(rotation[0], rotation[1], rotation[2], true);
	}
	void o3dCamera::setSpeed(float speed)
	{
		m_speed = speed;
	}
	void o3dCamera::update()
	{
		btDefaultMotionState *motionState = (btDefaultMotionState *)m_motionState;

		btTransform transform;
		motionState->getWorldTransform(transform);

		float x = transform.getOrigin().getX();
		float y = transform.getOrigin().getY();
		float z = transform.getOrigin().getZ();

		m_position = Vector3<float>(x, y, z);
	}
}
