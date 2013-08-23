#ifndef _CE_BASE_AUDIO_H_
#define _CE_BASE_AUDIO_H_

namespace ce
{
	class Audio
	{
		static void *Play2DProcess(void *data);
		static Audio *LoadWAV(const char *file);

		unsigned int m_buffer;

		Audio();

	public:
		static bool Init();
		static void Cleanup();

		static Audio *CreateFromFile(const char *file);

		~Audio();

		void Play2D(bool spawnThread = false);
	};
}

#endif
