//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/Plane.h>
#include <CE/Game2D/Zone.h>
#include <CE/Game2D/ZoneTrigger.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		ZoneTrigger::ZoneTrigger(Vector2<float> position, Vector2<float> extent) : ZoneEntity(position, extent)
		{
			m_collisionMask = 0;
		}
		void ZoneTrigger::Process()
		{
			if(m_zones.size())
			{
				vector<ZoneEntity *> intersections;
				Vector2<float> position = GetPosition(), extent = GetExtent();
				if(m_zones[0]->GetPlane())
					intersections = m_zones[0]->GetPlane()->BoxSearch(position[0], position[1], position[0] + extent[0], position[1] + extent[1], this);
				else
					intersections = m_zones[0]->BoxSearch(position[0], position[1], position[0] + extent[0], position[1] + extent[1], this);
				OnProcess(intersections);
			}
		}
		void ZoneTrigger::OnProcess(vector<ZoneEntity *> intersections)
		{
		}
	}
}
