//- Standard Library -
#include <fstream>
#include <sstream>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Game2D/AppGame2D.h>
#include <CE/Game2D/Sprite.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		void Sprite::OnFileChange()
		{
			AppGame2D *app = (AppGame2D *)App::GetCurrent();
			app->LockWorldMutex();
			LoadFromFile(m_file->GetFilePath().c_str());
			app->UnlockWorldMutex();
		}
		bool Sprite::LoadFromFile(const char *file)
		{
			if(m_animations.empty() == false)
			{
				Animation **markAnimations = &m_animations.front();
				Animation **endAnimations = markAnimations + m_animations.size();
				while(markAnimations != endAnimations)
					delete *markAnimations++;
				m_animations.clear();
			}

			ifstream inFile;
			inFile.open(file);

			string in;
			unsigned short minX, minY, maxX, maxY, stepX, stepY, numFrames;
			float *frameTimes, duration;

			Vector2<unsigned int> imageSize = m_source->GetSize();
			while(getline(inFile, in))
			{
				stringstream lineStream(in);

				if(in.find("//") == 0)
					continue;

				lineStream >> minX >> minY >> maxX >> maxY >> stepX >> stepY >> duration >> numFrames;
				frameTimes = new float[numFrames];
				for(unsigned short a = 0; a < numFrames; a++)
					lineStream >> frameTimes[a];
				AddAnimation(Vector2<float>(((float)minX) / imageSize[0], ((float)minY) / imageSize[1]), Vector2<float>(((float)maxX) / imageSize[0], ((float)maxY) / imageSize[1]), Vector2<float>(((float)stepX) / imageSize[0], ((float)stepY) / imageSize[1]), frameTimes, numFrames, duration);
				delete [] frameTimes;
			}
			inFile.close();
			return true;
		}
		Sprite *Sprite::LoadSpriteFromFile(const char *file, Image *image)
		{
			Sprite *sprite = new Sprite(image);
			sprite->LoadFromFile(file);

			File *fileObj = new File(file);
			fileObj->SetObject(sprite);

			return sprite;
		}

		Sprite::Animation::Animation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames, float duration)
		{
			m_min = min;
			m_max = max;
			m_step = step;
			m_numFrames = numFrames;
			m_frameTimes = new float[numFrames];
			m_duration = duration;
			for(int a = 0; a < numFrames; a++)
				m_frameTimes[a] = frameTimes[a];
		}
		Sprite::Animation::~Animation()
		{
			delete [] m_frameTimes;
		}

		Sprite::Sprite(Image *source) : File::FileObject()
		{
			m_source = source;
		}
		Sprite::~Sprite()
		{
			if(m_animations.empty() == false)
			{
				Animation **markAnimations = &m_animations.front();
				Animation **endAnimations = markAnimations + m_animations.size();
				while(markAnimations != endAnimations)
					delete *markAnimations++;
			}
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
		int Sprite::AddAnimation(Vector2<float> min, Vector2<float> max, Vector2<float> step, float *frameTimes, int numFrames, float duration)
		{
			m_animations.push_back(new Animation(min, max, step, frameTimes, numFrames, duration));
			return m_animations.size() - 1;
		}
		float Sprite::GetAnimationTime(int animation, float time)
		{
			if(((int)m_animations.size()) > animation && animation >= 0)
			{
				Animation *anim = m_animations[animation];
				while(time > anim->m_duration)
					time -= anim->m_duration;
				return time;
			}
			return 0.f;
		}
	}
}
