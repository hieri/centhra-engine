#ifndef _CE_PLUGIN_BOX2D_PHYSICSHANDLER_H_
#define _CE_PLUGIN_BOX2D_PHYSICSHANDLER_H_

//- Centhra Engine -
#include <CE/Vector2.h>
#include <CE/Game2D/PhysicsHandler.h>

namespace ce
{
	namespace plugin
	{
		namespace box2d
		{
			class bPhysicsHandler : public game2d::PhysicsHandler
			{
				void *m_b2d_system;

			protected:
				virtual void Initialize();
				virtual void Render(float minX, float minY, float maxX, float maxY);
				virtual void Process(float dt);
				virtual void Cleanup();

				virtual std::vector<game2d::PhysicalObject *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, game2d::PhysicalObject *ignore = 0);
				virtual std::vector<game2d::PhysicalObject *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, game2d::PhysicalObject *ignore = 0);

			public:
				class bObjectHandle : public game2d::PhysicsHandler::ObjectHandle
				{
					void *m_b2d_shape, *m_b2d_body;

				protected:
					bObjectHandle(game2d::PhysicsHandler *handler, game2d::PhysicalObject *object);
					~bObjectHandle();

					virtual void OnCreate();
					virtual void OnRelocate(game2d::PhysicalGroup *oldGroup, game2d::PhysicalGroup *newGroup);
					virtual void OnDestroy();
				
					virtual void OnSetPosition();
					virtual void OnSetExtent();
					virtual void OnSetVelocity();
					virtual void OnSetCollisionMask();

				public:
					friend class bPhysicsHandler;
				};

				void *GetBox2DSystem() const;
				void SetGravity(Vector2<float> gravity);

				bPhysicsHandler();
				~bPhysicsHandler();

/*
				void AddPulleyJoint(game2d::PhysicalObject *objA, game2d::PhysicalObject *objB);
				void AddDistanceJoint(game2d::PhysicalObject *objA, game2d::PhysicalObject *objB);
				void AddRevoluteJoint(game2d::PhysicalObject *objA, game2d::PhysicalObject *objB);
				void AddRopeJoint(game2d::PhysicalObject *objA, game2d::PhysicalObject *objB);
*/			};

		}
	}
}

#endif
