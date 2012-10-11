//- Standard Library -
#include <map>

//- SDL2 -
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_thread.h>

//- Bullet Physics -
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

//- Centhra Engine -
#include <CE/Game.h>
#include <CE/o3dEntity.h>
#include <CE/o3dInstance.h>
#include <CE/o3dObject.h>

using namespace std;

namespace ce
{
	class bulletOverlapFilterCallback : public btOverlapFilterCallback
	{
	public:
		virtual bool needBroadphaseCollision(btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1) const
		{
			return true;
			bool collides = (proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;
			collides = collides && (proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);
			return collides;
		}
	};

	class bulletDebugDraw : public btIDebugDraw
	{
		int m_debugMode;

	public:
		bulletDebugDraw() : btIDebugDraw()
		{
			m_debugMode = 0;
		}
		void draw3dText(const btVector3 &location, const char *textString)
		{
		}
		void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
		{
		}
		void reportErrorWarning(const char *warningString)
		{
		}
		void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
		{
			glColor4f(color.getX(), color.getY(), color.getZ(), 1.f);
			glBegin(GL_LINES);
				glVertex3f(from.getX(), from.getY(), from.getZ());
				glVertex3f(to.getX(), to.getY(), to.getZ());
			glEnd();
		}
		void setDebugMode(int debugMode)
		{
			m_debugMode = debugMode;
		}
		int getDebugMode() const
		{
			return m_debugMode;
		}
	};

	class bulletHandler
	{
		btDefaultCollisionConfiguration *m_collisionConfiguration;
		btCollisionDispatcher *m_dispatcher;
		btBroadphaseInterface *m_overlappingPairCache;
		btSequentialImpulseConstraintSolver *m_solver;
		btDiscreteDynamicsWorld *m_dynamicsWorld;

		bulletDebugDraw *m_debugDrawer;
		bulletOverlapFilterCallback *m_overlapFilterCallback;

		o3dInstance *m_o3dInstance;
		float m_timeScale;

	public:
		bulletHandler(o3dInstance *instance)
		{
			m_o3dInstance = instance;
			m_debugDrawer = new bulletDebugDraw;
			m_debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

			m_overlapFilterCallback = new bulletOverlapFilterCallback;

			///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
			m_collisionConfiguration = new btDefaultCollisionConfiguration();
			///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
			m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
			///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
		//	m_overlappingPairCache = new btDbvtBroadphase();
			btVector3 worldMin(-1000, -1000, -1000);
			btVector3 worldMax(1000, 1000, 1000);	
			btAxisSweep3* sweepBP = new btAxisSweep3(worldMin,worldMax);
			m_overlappingPairCache = sweepBP;

			///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
			m_solver = new btSequentialImpulseConstraintSolver;
			m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
			m_dynamicsWorld->setGravity(btVector3(0,-10,0));
			m_dynamicsWorld->setDebugDrawer(m_debugDrawer);
			m_dynamicsWorld->getPairCache()->setOverlapFilterCallback(m_overlapFilterCallback);

			m_timeScale = 1.f;
		}
		~bulletHandler()
		{
			delete m_dynamicsWorld;
			delete m_solver;
			delete m_overlappingPairCache;
			delete m_dispatcher;
			delete m_collisionConfiguration;
			delete m_debugDrawer;
		}
		void addController(void *controllerPtr)
		{
			btCharacterControllerInterface *controller = (btCharacterControllerInterface *)controllerPtr;
			m_dynamicsWorld->addAction(controller);
		}
		void addRigidBody(void *rigidBodyPtr, int collisionFilterGroup = btBroadphaseProxy::DefaultFilter, int collisionFilterMask = btBroadphaseProxy::AllFilter)
		{
			btRigidBody *rigidBody = (btRigidBody *)rigidBodyPtr;
			m_dynamicsWorld->addRigidBody(rigidBody, collisionFilterGroup, collisionFilterMask);
		}
		void removeController(void *controllerPtr)
		{
			btCharacterControllerInterface *controller = (btCharacterControllerInterface *)controllerPtr;
			m_dynamicsWorld->removeAction(controller);
		}
		void removeRigidBody(void *rigidBodyPtr)
		{
			btRigidBody *rigidBody = (btRigidBody *)rigidBodyPtr;
			m_dynamicsWorld->removeCollisionObject(rigidBody);
		}
		void draw()
		{
			m_dynamicsWorld->debugDrawWorld();
		}
		o3dInstance *getO3DInstance() const
		{
			return m_o3dInstance;
		}
		float getTimeScale() const
		{
			return m_timeScale;
		}
		void setTimeScale(float timeScale)
		{
			m_timeScale = timeScale;
		}
		void step(unsigned int timeMS)
		{
			m_dynamicsWorld->stepSimulation(((float)timeMS) / 1000.f * m_timeScale, 16);
			
			for(int a = m_dynamicsWorld->getNumCollisionObjects() - 1; a >= 0; a--)
			{
				btCollisionObject *obj = m_dynamicsWorld->getCollisionObjectArray()[a];
				if(obj)
					((o3dObject *)obj->getUserPointer())->update();
			}
		}
	};

	int sdlBulletThread(void *handlerPtr)
	{
		Game *game = Game::getCurrent();
		bulletHandler *handler = (bulletHandler *)handlerPtr;
		o3dInstance *instance = handler->getO3DInstance();
		unsigned int lastTime = game->getRunTime();

		while(game->isRunning() && instance->isPhysicsRunning())
		{
			SDL_Delay(1);

			unsigned int runTime = game->getRunTime();
			unsigned int updateTick = 1000 / instance->getPhysicsUpdateRate();
			if((runTime - lastTime) < updateTick)
				continue;

			handler->step(runTime - lastTime);
			lastTime = runTime;
		}

		return 0;
	}

	o3dInstance::o3dInstance()
	{
		m_bulletHandler = new bulletHandler(this);
		m_count = 0;
		m_isPhysicsRunning = false;
		m_physicsUpdateRate = 60;
	}
	o3dInstance::~o3dInstance()
	{
		if(m_isPhysicsRunning)
			stopPhysics();
		if(m_bulletHandler)
			delete (bulletHandler *)m_bulletHandler;
	}
	void o3dInstance::add(o3dEntity *obj)
	{
		if(!isMember(obj))
		{
			m_objects.insert(m_objects.end(), obj);
			m_count = m_objects.size();
		}
	}
	void o3dInstance::addController(void *controllerPtr)
	{
		bulletHandler *handler = (bulletHandler *)m_bulletHandler;
		handler->addController(controllerPtr);
	}
	void o3dInstance::addRigidBody(void *rigidBodyPtr)
	{
		bulletHandler *handler = (bulletHandler *)m_bulletHandler;
		handler->addRigidBody(rigidBodyPtr);
	}
	void o3dInstance::addRigidBody(void *rigidBodyPtr, int collisionFilterGroup = btBroadphaseProxy::DefaultFilter, int collisionFilterMask = btBroadphaseProxy::AllFilter)
	{
		bulletHandler *handler = (bulletHandler *)m_bulletHandler;
		handler->addRigidBody(rigidBodyPtr, collisionFilterGroup, collisionFilterMask);
	}
	unsigned int o3dInstance::getCount() const
	{
		return m_count;
	}
	o3dEntity *o3dInstance::getObject(unsigned int idx) const
	{
		if(idx >= getCount())
			return 0;
		return m_objects.at(idx);
	}
	unsigned int o3dInstance::getPhysicsUpdateRate() const
	{
		return m_physicsUpdateRate;
	}
	float o3dInstance::getTimeScale() const
	{
		return ((bulletHandler *)m_bulletHandler)->getTimeScale();
	}
	bool o3dInstance::isMember(o3dEntity *obj) const
	{
		for(unsigned int a = 0; a < getCount(); a++)
			if(getObject(a) == obj)
				return true;
		return false;
	}
	bool o3dInstance::isPhysicsRunning() const
	{
		return m_isPhysicsRunning;
	}
	void o3dInstance::remove(o3dEntity *obj)
	{
		if(isMember(obj))
		{
			for(vector<o3dEntity *>::iterator it = m_objects.begin(); it != m_objects.end(); it++)
				if((*it) == obj)
				{
					m_objects.erase(it);
					break;
				}
			m_count = m_objects.size();
		}
	}
	void o3dInstance::removeController(void *controllerPtr)
	{
		bulletHandler *handler = (bulletHandler *)m_bulletHandler;
		handler->removeController(controllerPtr);
	}
	void o3dInstance::removeRigidBody(void *rigidBodyPtr)
	{
		bulletHandler *handler = (bulletHandler *)m_bulletHandler;
		handler->removeRigidBody(rigidBodyPtr);
	}
	void o3dInstance::render()
	{
		for(unsigned int a = 0; a < getCount(); a++)
			getObject(a)->render();

		glClear(GL_DEPTH_BUFFER_BIT);

		bulletHandler *handler = (bulletHandler *)m_bulletHandler;
		handler->draw();
	}
	void o3dInstance::setPhysicsUpdateRate(unsigned int physicsUpdateRate)
	{
		m_physicsUpdateRate = physicsUpdateRate;
	}
	void o3dInstance::setTimeScale(float timeScale)
	{
		((bulletHandler *)m_bulletHandler)->setTimeScale(timeScale);
	}
	void o3dInstance::startPhysics()
	{
		if(!m_isPhysicsRunning)
		{
			m_isPhysicsRunning = true;
			m_bulletThread = SDL_CreateThread(sdlBulletThread, "BulletThread", m_bulletHandler);
		}
	}
	void o3dInstance::stopPhysics()
	{
		if(m_isPhysicsRunning)
		{
			m_isPhysicsRunning = false;
			SDL_WaitThread((SDL_Thread *)m_bulletThread, 0);
			m_bulletThread = 0;
		}
	}
}
