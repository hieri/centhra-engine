#ifndef _CE_GAME3D_PHYSICALGROUP_H_
#define _CE_GAME3D_PHYSICALGROUP_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Group.h>
#include <CE/Game3D/Entity.h>

namespace ce
{
	namespace game3d
	{
		class PhysicalObject;
		class PhysicsHandler;

		class PhysicalGroup : public Entity, public Group
		{
//			PhysicsHandler *m_physicsHandler;

		public:
			PhysicalGroup();
			
//			void ProcessPhysics(float dt);
//			void Render(float minX, float minY, float maxX, float maxY);

//			void AttachHandler(PhysicsHandler *handler);
//			void DetachHandler();
//			PhysicsHandler *GetPhysicsHandler() const;

//			std::vector<PhysicalObject *> BoxSearch(float minX, float minY, float maxX, float maxY, unsigned int mask = -1, PhysicalObject *ignore = 0);
//			std::vector<PhysicalObject *> SegmentSearch(float startX, float startY, float endX, float endY, unsigned int mask = -1, PhysicalObject *ignore = 0);

			virtual void OnMemberAdded(Group::Member *entity);
			virtual void OnMemberRemoved(Group::Member *entity);
		};
	}
}

#endif
