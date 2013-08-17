//- Centhra Engine -
#include <CE/Audio.h>
#include <CE/Plugin.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

#ifdef linux
	//- Linux -
	#include <unistd.h>
#endif

//- OpenAL -
#include <al.h>
#include <alc.h>

namespace ce
{
	bool Audio::Init()
	{
		Plugin::InitializeByType(Plugin::Audio);

		ALCdevice *alcDevice = alcOpenDevice(NULL);
		if(!alcDevice)
			return false;

		ALCcontext *alcContext = alcCreateContext(alcDevice, NULL);
		alcMakeContextCurrent(alcContext);
		alGetError();
		return true;
	}
	void Audio::Cleanup()
	{
		ALCcontext *alcContext = alcGetCurrentContext();
		ALCdevice *alcDevice = alcGetContextsDevice(alcContext);
		alcMakeContextCurrent(NULL);
		alcDestroyContext(alcContext);
		alcCloseDevice(alcDevice);
	}

	Audio *Audio::CreateFromFile(const char *file)
	{
//		alListenerfv(AL_POSITION, pos);
//		alListenerfv(AL_VELOCITY, pos);
//		alListenerfv(AL_ORIENTATION, matr);


//		alGenBuffers(

//		alGetError() != AL_NO_ERROR
		return 0;
	}

	Audio::Audio()
	{
	}

}
