#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/Sprite.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		Sprite::Animation::Animation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames)
		{
			m_min = min;
			m_max = max;
			m_step = step;
			m_numFrames = numFrames;
			m_frameTimes = new float[numFrames];
			for(int a = 0; a < numFrames; a++)
				m_frameTimes[a] = frameTimes[a];
		}
		Sprite::Animation::~Animation()
		{
			delete [] m_frameTimes;
		}

		Sprite::Sprite(Image *source)
		{
			m_source = source;
		}
		Sprite::~Sprite()
		{
			for(vector<Animation *>::iterator it = m_animations.begin(); it != m_animations.end(); it++)
				delete *it;
		}
		void Sprite::Draw(int animation, float time)
		{
			if(((int)m_animations.size()) > animation && animation >= 0)
			{
					Animation *anim = m_animations[animation];

					float maxTime = anim->m_frameTimes[anim->m_numFrames - 1];
					time = fmodf(time, maxTime);
					int curFrame = 0;
					for(int a = 0; a < anim->m_numFrames; a++)
					{
						if(time < anim->m_frameTimes[a])
							break;
						curFrame++;
					}

					float minX = anim->m_min[0] + curFrame * anim->m_step[0];
					float minY = anim->m_min[1] + curFrame * anim->m_step[1];
					float maxX = minX + anim->m_max[0];
					float maxY = minY + anim->m_max[1];

					glColor4f(1.f, 1.f, 1.f, 1.f);
					glPushMatrix();
						m_source->Bind();
						glBegin(GL_QUADS);
						glTexCoord2f(minX, maxY);
						glVertex2i(0, 0);

						glTexCoord2f(maxX, maxY);
						glVertex2i(1, 0);

						glTexCoord2f(maxX, minY);
						glVertex2i(1, 1);

						glTexCoord2f(minX, minY);
						glVertex2i(0, 1);
					glEnd();
				glPopMatrix();
			}
		}
		int Sprite::AddAnimation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames)
		{
			m_animations.push_back(new Animation(min, max, step, frameTimes, numFrames));
			return m_animations.size() - 1;
		}
	}
}
