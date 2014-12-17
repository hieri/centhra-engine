#ifndef _CE_GAME2D_EXPLOSION_H_
#define _CE_GAME2D_EXPLOSION_H_

//- Centhra Engine -
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class Explosion : public PhysicalObject
		{
		protected:
			virtual void DoRender();
			virtual void OnProcess(float dt);

		public:
			typedef enum ExplosionType
			{
				Boom
			} ExplosionType;

			static void InitAssets();
			static void CleanupAssets();
			static void LoadDefinitionsFromFile(const char *file);

			unsigned long long m_timeout;
			float m_animTime;

			Explosion(Vector2<float> position, unsigned long long timeout);
		};
	}
}

#endif
