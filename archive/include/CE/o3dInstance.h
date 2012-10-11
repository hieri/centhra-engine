#ifndef CE_O3DINSTANCE_H
#define CE_O3DINSTANCE_H

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Vector3.h>

namespace ce
{
	enum collisionTypes
	{
		COL_NONE = 0,
		COL_STANDARD = 1,
		COL_GHOST = 2,
	};

	class o3dObject;
	class o3dEntity;

	/* 	3d Object Instance Class
	 *	- Main container for 3d objects.
	 */
	class o3dInstance
	{
		bool m_isPhysicsRunning;
		std::vector<o3dEntity *> m_objects;
		unsigned int m_count, m_physicsUpdateRate;
		void *m_bulletHandler, *m_bulletThread;

	public:
		o3dInstance();
		~o3dInstance();

		void add(o3dEntity *obj);
		void addController(void *controllerPtr);
		void addRigidBody(void *rigidBodyPtr);
		void addRigidBody(void *rigidBodyPtr, int collisionFilterGroup, int collisionFilterMask);
		unsigned int getCount() const;
		o3dEntity *getObject(unsigned int idx) const;
		unsigned int getPhysicsUpdateRate() const;
		float getTimeScale() const;
		bool isMember(o3dEntity *obj) const;
		bool isPhysicsRunning() const;
		void remove(o3dEntity *obj);
		void removeController(void *controllerPtr);
		void removeRigidBody(void *rigidBodyPtr);
		void render();
		void setPhysicsUpdateRate(unsigned int physicsUpdateRate);
		void setTimeScale(float timeScale);
		void startPhysics();
		void stopPhysics();
	};
}

#endif
