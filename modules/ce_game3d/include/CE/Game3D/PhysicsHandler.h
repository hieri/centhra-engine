#ifndef _CE_GAME3D_PHYSICSHANDLER_H_
#define _CE_GAME3D_PHYSICSHANDLER_H_

//- Centhra Engine -
#include <CE/Game3D/Entity.h>
#include <CE/Game3D/PhysicalGroup.h>

namespace ce
{
	namespace game3d
	{
		class PhysicsHandler
		{
			PhysicalGroup *m_referenceGroup;

		protected:
			virtual void Initialize();
//			virtual void Render(float minX, float minY, float maxX, float maxY);
			virtual void Process(float dt);
			virtual void Cleanup();
			virtual void SetupObject(PhysicalObject *object);
			virtual void CleanupObject(PhysicalObject *object);
			
//			virtual std::vector<PhysicalObject *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, PhysicalObject *ignore = 0);
//			virtual std::vector<PhysicalObject *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, PhysicalObject *ignore = 0);

		public:
			class ObjectHandle
			{
			protected:
				ObjectHandle(PhysicsHandler *handler, PhysicalObject *object);
				~ObjectHandle();

				PhysicsHandler *m_physicsHandler;
				PhysicalObject *m_object;

				virtual void OnCreate();
				virtual void OnRelocate(PhysicalGroup *oldGroup, PhysicalGroup *newGroup);
				virtual void OnDestroy();
				
				virtual void OnSetCollisionMask();
				virtual void OnSetStatic();
				virtual void OnSetPosition();
				virtual void OnSetSize();
				virtual void OnSetVelocity();
				virtual void OnSetRotation();

			public:
				PhysicalObject *GetReferenceObject() const;
				PhysicsHandler *GetHandler() const;

				friend class PhysicalObject;
			};
			
			PhysicsHandler();
			~PhysicsHandler();

			PhysicalGroup *GetReferenceGroup() const;

			friend class PhysicalGroup;
		};
	}
}

#endif
