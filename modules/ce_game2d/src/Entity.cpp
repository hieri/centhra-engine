//- Centhra Engine -
#include <CE/Game2D/Entity.h>
#include <CE/Base.h>

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		Entity::Entity()
		{
		}
		void Entity::Render()
		{
			glPushMatrix();
			DoRender();
			glPopMatrix();
		}
		void Entity::DoRender()
		{
			print("ATF\n");
		}
	}
}