#ifndef _CE_GAME2D_ZONETRIGGER_H_
#define _CE_GAME2D_ZONETRIGGER_H_

//- Centhra Engine -
#include <CE/Game2D/ZoneEntity.h>

namespace ce
{
	namespace game2d
	{
		class ZoneTrigger : public ZoneEntity
		{
		public:
			ZoneTrigger(Vector2<float> position, Vector2<float> extent);

			void Process();
			virtual void OnProcess(std::vector<ZoneEntity *> intersections);
		};
	}
}

#endif
