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
#include <CE/Sprite.h>
#include <CE/Renderer.h>
#include <CE/Base.h>

using namespace std;

//TODO: Generate master VBO for all sprites

namespace ce
{
	void Sprite::OnFileChange()
	{
/*		AppGame2D *app = (AppGame2D *)App::GetCurrent();
		app->LockWorldMutex();
		LoadFromFile(m_file->GetFilePath().c_str());
		app->UnlockWorldMutex();*/
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
		m_frameBases = new int[numFrames];
		m_duration = duration;
		for(int a = 0; a < numFrames; a++)
		{
			m_frameTimes[a] = frameTimes[a];
			m_frameBases[a] = 0;
		}
	}
	Sprite::Animation::~Animation()
	{
		delete [] m_frameTimes;
	}

	Sprite::Sprite(Image *source) : File::FileObject(), m_source(source), m_glBuffer(0)
	{
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
		if(m_glBuffer)
			glDeleteBuffers(1, &m_glBuffer);
	}
	void Sprite::Draw(RenderContext &context, int animation, float time)
	{
		if(!m_glBuffer)
			GenerateBuffer();
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

			ConfigTextured();

			m_source->Bind();
			BindVBO(m_glBuffer);
			BindSquareEBO();

			ShaderProgram::TexturedProgram *program = 0;
			if(context.useShaders)
				program = UseTexturedProgram();
			if(program != 0)
			{
				glVertexAttribPointer(program->uv, 2, GL_FLOAT, 0, 8, (GLvoid *)anim->m_frameBases[curFrame]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
			}
			else
			{
				glVertexPointer(2, GL_FLOAT, 8, 0);
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, 8, (GLvoid *)anim->m_frameBases[curFrame]);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
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

	//- Buffer Object -
	void Sprite::GenerateBuffer()
	{
		if(!m_glBuffer)
		{
			LoadExtensions();

			glGenBuffers(1, &m_glBuffer);
			glBindBuffer(0x8892, m_glBuffer);

			unsigned int base = 8;

			//- Assume there's atleast one animation frame -
			if(m_animations.empty() == false)
			{
				Animation **markAnimations = &m_animations.front();
				Animation **endAnimations = markAnimations + m_animations.size();
				while(markAnimations != endAnimations)
				{
					Animation *anim = *markAnimations++;
					for(unsigned short a = 0; a < anim->m_numFrames; a++)
						anim->m_frameBases[a] = base + a * 8;
					base += anim->m_numFrames * 8;
				}
			}

			unsigned int size = base + 8; // 4 * 2 + Total animation frames * 8
			float *buffer = new float[size];

			buffer[0] = 0.f;
			buffer[1] = 0.f;

			buffer[2] = 1.f;
			buffer[3] = 0.f;

			buffer[4] = 1.f;
			buffer[5] = 1.f;

			buffer[6] = 0.f;
			buffer[7] = 1.f;
			
			//- Populate buffer with UV's for each animation frame -
			if(m_animations.empty() == false)
			{
				Animation **markAnimations = &m_animations.front();
				Animation **endAnimations = markAnimations + m_animations.size();
				while(markAnimations != endAnimations)
				{
					Animation *anim = *markAnimations++;
					for(unsigned short a = 0; a < anim->m_numFrames; a++)
					{
						int base = anim->m_frameBases[a];
						float minX = anim->m_min[0] + a * anim->m_step[0];
						float minY = anim->m_min[1] + a * anim->m_step[1];
						float maxX = minX + anim->m_max[0];
						float maxY = minY + anim->m_max[1];

						buffer[base]     = minX;
						buffer[base + 1] = maxY;

						buffer[base + 2] = maxX;
						buffer[base + 3] = maxY;

						buffer[base + 4] = maxX;
						buffer[base + 5] = minY;

						buffer[base + 6] = minX;
						buffer[base + 7] = minY;

						//- Calculate Actual Base -
						anim->m_frameBases[a] = base * 4;
					}
				}
			}
			glBufferData(0x8892, size * sizeof(float), buffer, 0x88E4);
			delete[] buffer;
		}
	}
}
