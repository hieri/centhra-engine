//- Centhra Engine -
#include <CE/Audio.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

#ifdef linux
	//- Linux -
	#include <unistd.h>
#endif

//- OpenAL -
#include <AL/al.h>
#include <AL/alc.h>

namespace ce
{
	bool Audio::Init()
	{
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

	Audio::Audio()
	{
	}
}
