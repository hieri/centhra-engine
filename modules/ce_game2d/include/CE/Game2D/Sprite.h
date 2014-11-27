#ifndef _CE_GAME2D_SPRITE_H_
#define _CE_GAME2D_SPRITE_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Image.h>
#include <CE/Vector2.h>

namespace ce
{
	namespace game2d
	{
		class Sprite
		{
			class Animation
			{
			public:
				Vector2<float> m_min, m_max, m_step;
				int m_numFrames;
				float *m_frameTimes, m_duration;

				Animation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames, float duration);
				~Animation();
			};

			Image *m_source;

		public:
			static Sprite *LoadSpriteFromFile(const char *file, Image *image);

			Sprite(Image *source);
			~Sprite();

			int AddAnimation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames, float duration);
			float GetAnimationTime(int animation, float time);
			void Draw(int animation, float time);

		private:
			std::vector<Animation *> m_animations;
		};
	}
}

#endif
