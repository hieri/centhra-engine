#ifndef _CE_GAME2D_PHYSICSHANDLER_H_
#define _CE_GAME2D_PHYSICSHANDLER_H_

//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Game2D/PhysicalGroup.h>

namespace ce
{
	namespace game2d
	{
		class PhysicsHandler
		{
			PhysicalGroup *m_referenceGroup;

		protected:
			virtual void Initialize();
			virtual void Process(float dt);
			virtual void Cleanup();

		public:
			PhysicsHandler();
			~PhysicsHandler();

			class ObjectHandle
			{
				ObjectHandle(PhysicsHandler *handler, PhysicalObject *object);
				~ObjectHandle();

			protected:
				PhysicsHandler *m_physicsHandler;
				PhysicalObject *m_object;

				virtual void OnCreate();
				virtual void OnRelocate(PhysicalGroup *oldGroup, PhysicalGroup *newGroup);
				virtual void OnDestroy();
			};

			friend class PhysicalGroup;
		};
	}
}

#endif
