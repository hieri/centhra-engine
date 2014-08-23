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
				virtual void SetupObject(game2d::PhysicalObject *object);
				virtual void CleanupObject(game2d::PhysicalObject *object);

				virtual std::vector<game2d::PhysicalObject *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, game2d::PhysicalObject *ignore = 0);
				virtual std::vector<game2d::PhysicalObject *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, game2d::PhysicalObject *ignore = 0);
				virtual std::vector<std::pair<game2d::PhysicalObject *, Vector2<float> > > SpecialSegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, game2d::PhysicalObject *ignore = 0);

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
				
					virtual void OnSetStatic();
					virtual void OnSetPosition();
					virtual void OnSetExtent();
					virtual void OnSetRotation();
					virtual void OnSetVelocity();
					virtual void OnSetCollisionMask();
					virtual void OnSetAngularVelocity();

				public:
					friend class bPhysicsHandler;

					void SetFixedRotation(bool fixedRotation);
				};

				void *GetBox2DSystem() const;
				void SetGravity(Vector2<float> gravity);

				bPhysicsHandler();
				~bPhysicsHandler();

				void AddJoint(game2d::PhysicalObject *objA, game2d::PhysicalObject *objB, Vector2<float> offsetA, Vector2<float> offsetB);
				void ChangeToCircle(game2d::PhysicalObject *obj);
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
