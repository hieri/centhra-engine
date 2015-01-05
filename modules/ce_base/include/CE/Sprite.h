#ifndef _CE_BASE_SPRITE_H_
#define _CE_BASE_SPRITE_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Image.h>
#include <CE/RenderContext.h>
#include <CE/Vector2.h>

namespace ce
{
	class Sprite : public File::FileObject
	{
		class Animation
		{
		public:
			Vector2<float> m_min, m_max, m_step;
			int m_numFrames, *m_frameBases;
			float *m_frameTimes, m_duration;

			Animation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames, float duration);
			~Animation();
		};

		Image *m_source;
		virtual void OnFileChange();
		bool LoadFromFile(const char *file);
	public:
		static Sprite *LoadSpriteFromFile(const char *file, Image *image);

		Sprite(Image *source);
		~Sprite();

		int AddAnimation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames, float duration);
		float GetAnimationTime(int animation, float time);
		void Draw(RenderContext &context, int animation, float time);

	private:
		std::vector<Animation *> m_animations;

		//- Buffer Object -
	protected:
		unsigned int m_glBuffer;
		void GenerateBuffer();
	};
}

#endif
