#ifndef _CE_GAME2D_PHYSICALGROUP_H_
#define _CE_GAME2D_PHYSICALGROUP_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Group.h>
#include <CE/Game2D/Entity.h>
#include <CE/Vector2.h>

namespace ce
{
	namespace game2d
	{
		class PhysicalObject;
		class PhysicsHandler;

		class PhysicalGroup : public Entity, public Group
		{
			PhysicsHandler *m_physicsHandler;

		public:
			PhysicalGroup();
			virtual ~PhysicalGroup();
			
			void ProcessPhysics(float dt);
			virtual void Render(float minX, float minY, float maxX, float maxY);

			void AttachHandler(PhysicsHandler *handler);
			void DetachHandler();
			PhysicsHandler *GetPhysicsHandler() const;

			std::vector<PhysicalObject *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			std::vector<PhysicalObject *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			std::vector<std::pair<PhysicalObject *, Vector2<float> > > SpecialSegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, PhysicalObject *ignore = 0);
			std::pair<PhysicalObject *, Vector2<float> > RaycastSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1);

			virtual void OnMemberAdded(Group::Member *entity);
			virtual void OnMemberRemoved(Group::Member *entity);

			//- Debug Rendering -
		protected:
			bool m_renderDebug;
		public:
			bool IsRenderingDebug() const;
			void SetDebugRendering(bool renderDebug);
		};
	}
}

#endif
