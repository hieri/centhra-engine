//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Game2D/Trigger.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		Trigger::Trigger(Vector2<float> position, Vector2<float> extent) : PhysicalObject(position, extent)
		{
			m_isTrigger = true;
			m_color = Color(255, 255, 255, 50);
		}
		void Trigger::OnObjectEnter(PhysicalObject *object)
		{
		}
		void Trigger::OnObjectLeave(PhysicalObject *object)
		{
		}
	}
}
