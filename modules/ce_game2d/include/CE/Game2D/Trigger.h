#ifndef _CE_GAME2D_TRIGGER_H_
#define _CE_GAME2D_TRIGGER_H_

//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class Trigger : public PhysicalObject
		{
		public:
			Trigger(Vector2<float> position, Vector2<float> extent);

			virtual void OnObjectEnter(PhysicalObject *object);
			virtual void OnObjectLeave(PhysicalObject *object);
		};
	}
}

#endif
