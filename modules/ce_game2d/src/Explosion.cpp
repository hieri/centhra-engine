#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

#ifdef _WIN32
	#include <GL/glext.h>
	#include <GL/wglext.h>
	#define glGetProcAddress wglGetProcAddress
#elif __linux__
	#include <GL/glx.h>
	#define glGetProcAddress glXGetProcAddress
#endif

//- Standard Library -
#include <vector>
#include <fstream>
#include <sstream>

//- Centhra Engine -
#include <CE/Image.h>
#include <CE/Base.h>
#include <CE/Game2D/Sprite.h>
#include <CE/Game2D/Explosion.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		//-------------------- EXPLOSIONS???? ----------------
		void Explosion::InitAssets()
		{
		}
		void Explosion::CleanupAssets()
		{
		}
		void Explosion::LoadDefinitionsFromFile(const char *file)
		{
		}
		Explosion::Explosion(ce::Vector2<float> position, unsigned long long timeout) : PhysicalObject(position, Vector2<float>(1.f, 1.f), true), m_timeout(timeout)
		{
		}
		void Explosion::DoRender()
		{
		}
		void Explosion::OnProcess(float dt)
		{
			if(getRunTimeMS() > m_timeout)
				Delete();
		}
	}
}
