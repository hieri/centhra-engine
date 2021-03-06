#ifndef _CE_GAME2D_PHYSICSHANDLER_H_
#define _CE_GAME2D_PHYSICSHANDLER_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Rect.h>
#include <CE/Game2D/Entity.h>
#include <CE/Game2D/PhysicalGroup.h>

namespace ce
{
	namespace game2d
	{
		class PhysicsHandler
		{
		protected:
			PhysicalGroup *m_referenceGroup;

			virtual void Initialize();
			virtual void Process(float dt);
			virtual void Cleanup();
			virtual void SetupObject(PhysicalObject *object);
			virtual void CleanupObject(PhysicalObject *object);
			
			virtual std::vector<PhysicalObject *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			virtual std::vector<PhysicalObject *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			virtual std::vector<std::pair<PhysicalObject *, Vector2<float> > > SpecialSegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			virtual std::pair<PhysicalObject *, Vector2<float> > RaycastSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1);

		public:
			class ObjectHandle
			{
			protected:
				ObjectHandle(PhysicsHandler *handler, PhysicalObject *object);
				virtual ~ObjectHandle();

				PhysicsHandler *m_physicsHandler;
				PhysicalObject *m_object;

				virtual void OnCreate();
				virtual void OnRelocate(PhysicalGroup *oldGroup, PhysicalGroup *newGroup);
				virtual void OnDestroy();

				virtual void OnSetTypeMask();
				virtual void OnSetCollisionMask();
				virtual void OnSetStatic();
				virtual void OnSetPosition();
				virtual void OnSetExtent();
				virtual void OnSetVelocity();
				virtual void OnSetRotation();
				virtual void OnSetAngularVelocity();
				virtual void OnSetFixedRotation();

				//- Axis-Aligned Bounding Box -
			protected:
				void UpdateObjectAABB(Rect<float> aabb);

			public:
				PhysicalObject *GetReferenceObject() const;
				PhysicsHandler *GetHandler() const;

				virtual bool CollidesWith(Vector2<float> pt);

				friend class PhysicalObject;
			};
			
			PhysicsHandler();
			virtual ~PhysicsHandler();

			PhysicalGroup *GetReferenceGroup() const;
			virtual void Render(float minX, float minY, float maxX, float maxY);

			friend class PhysicalGroup;
		};
	}
}

#endif
