//- Standard Library -
#include <string>
#include <stdio.h>
#include <iostream>

//- Centhra Engine -
#include <CE/App.h>
#include <CE/Audio.h>
#include <CE/Thread.h>
#include <CE/Plugin.h>
#include <CE/Base.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
	typedef __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;
#endif

#ifdef linux
	//- Linux -
	#include <unistd.h>
	#include <stdint.h>
#endif

//- OpenAL -
#include <al.h>
#include <alc.h>

using namespace std;

namespace ce
{
	typedef struct Play2DStruct
	{
		Thread *thread;
		Audio *audio;

		Play2DStruct()
		{
			thread = 0;
			audio = 0;
		}
	} Play2DStruct;

	ALfloat GetBufferLength(ALuint buffer)
	{
		ALint size, bits, channels, freq;

		alGetBufferi(buffer, AL_SIZE, &size);
		alGetBufferi(buffer, AL_BITS, &bits);
		alGetBufferi(buffer, AL_CHANNELS, &channels);
		alGetBufferi(buffer, AL_FREQUENCY, &freq);
		if(alGetError() != AL_NO_ERROR)
			return -1.0f;

		return (ALfloat)((ALuint)size / channels / (bits / 8)) / (ALfloat)freq;
	}

	void *Audio::Play2DProcess(void *data)
	{
		App *app = App::GetCurrent();
		Play2DStruct *structData = (Play2DStruct *)data;
		Audio *audio = structData->audio;
		Thread *thread = structData->thread;
		delete structData;

		ALuint source;
		ALfloat timeS = GetBufferLength(audio->m_buffer);

		alGenSources(1, &source);
		if(alGetError() != AL_NO_ERROR)
		{
			error("[Error] alGenSources: Unable to generate an audio source.\n");
			Thread::Exit(NULL);
			return 0;
		}

		ALfloat listenerPosition [] = { 0.f, 0.f, 0.f };
		ALfloat listenerVelocity [] = { 0.f, 0.f, 0.f };
		ALfloat listenerOrientation [] = { 0.f, 0.f, -1.f, 0.f, 1.f, 0.f };
		alListenerfv(AL_POSITION, listenerPosition);
		alListenerfv(AL_VELOCITY, listenerVelocity);
		alListenerfv(AL_ORIENTATION, listenerOrientation);

		ALfloat sourcePosition [] = { 0.f, 0.f, 0.f };
		ALfloat sourceVelocity [] = { 0.f, 0.f, 0.f };
		alSourcei(source, AL_BUFFER, audio->m_buffer);
		alSourcef(source, AL_PITCH, 1.0f);
		alSourcef(source, AL_GAIN, 1.0f);
		alSourcefv(source, AL_POSITION, sourcePosition);
		alSourcefv(source, AL_VELOCITY, sourceVelocity);
		alSourcei(source, AL_LOOPING, AL_FALSE);

		alSourcePlay(source);

		if(alGetError() != AL_NO_ERROR)
		{
			error("[Error] alSourcePlay: Unable to play audio.\n");
			Thread::Exit(NULL);
			return 0;
		}

		unsigned long timeMS = (unsigned long)(timeS * 1000);
		unsigned long targetTime = app->GetRunTimeMS() + timeMS + 50;

		while(app->IsRunning())
		{
			if(app->GetRunTimeMS() > targetTime)
				break;
			sleepMS(100);
		}
		//	sleepMS(timeMS + 50);

		alDeleteSources(1, &source);
		thread->End();
		Thread::Exit(NULL);
		return 0;
	}

	Audio *Audio::LoadWAV(const char *file)
	{
		FILE *fp = 0;
		#ifdef _WIN32
			fopen_s(&fp, file, "rb");
		#else
			fp = fopen(file, "rb");
		#endif
		if(!fp)
		{
			error("[Error] Audio::LoadWAV: Unable to open file <%s>.\n", file);
			return 0;
		}

		char type[4];
		uint32_t size, chunkSize;
		uint16_t formatType, channels;
		uint32_t sampleRate, avgBytesPerSec;
		uint16_t bytesPerSample, bitsPerSample;
		uint32_t dataSize;

		fread(type, sizeof(char), 4, fp);
		if(type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F')
		{
			print("[Error] Audio::LoadWAV: File is not a WAV file <%s>.\n", file);
			return 0;
		}

		fread(&size, sizeof(uint32_t), 1, fp);
		fread(type, sizeof(char), 4, fp);
		if(type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E')
		{
			print("[Error] Audio::LoadWAV: File is not a WAV file <%s>.\n", file);
			return 0;
		}

		fread(type, sizeof(char), 4, fp);
		if(type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ')
		{
			print("[Error] Audio::LoadWAV: File is not a WAV file <%s>.\n", file);
			return 0;
		}

		fread(&chunkSize, sizeof(uint32_t), 1, fp);
		fread(&formatType, sizeof(uint16_t), 1, fp);
		fread(&channels, sizeof(uint16_t), 1, fp);
		fread(&sampleRate, sizeof(uint32_t), 1, fp);
		fread(&avgBytesPerSec, sizeof(uint32_t), 1, fp);
		fread(&bytesPerSample, sizeof(uint16_t), 1, fp);
		fread(&bitsPerSample, sizeof(uint16_t), 1, fp);

		fread(type, sizeof(char), 4, fp);
		if(type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a')
		{
			print("[Error] Audio::LoadWAV: Unexpected end to WAV file <%s>.\n", file);
			return 0;
		}

		fread(&dataSize, sizeof(uint32_t), 1, fp);

		unsigned char *buffer = new unsigned char[dataSize];
		fread(buffer, sizeof(char), dataSize, fp);

		ALuint frequency = sampleRate;
		ALenum format = 0;

		Audio *audio = new Audio();

		if(bitsPerSample == 8)
		{
			if(channels == 1)
				format = AL_FORMAT_MONO8;
			else if(channels == 2)
				format = AL_FORMAT_STEREO8;
		}
		else if(bitsPerSample == 16)
		{
			if(channels == 1)
				format = AL_FORMAT_MONO16;
			else if(channels == 2)
				format = AL_FORMAT_STEREO16;
		}
		if(!format)
		{
			print("[Error] Audio::LoadWAV: File has unknown WAV format <%s>.\n", file);
			delete audio;
			return 0;
		}

		alBufferData(audio->m_buffer, format, buffer, dataSize, frequency);
		if(alGetError() != AL_NO_ERROR)
		{
			print("[Error] alBufferData: Unable to buffer audio data.\n");
			delete audio;
			return 0;
		}

		fclose(fp);
		delete [] buffer;
		return audio;
	}

	bool Audio::Init()
	{
		Plugin::InitializeByType(Plugin::Audio);

		ALCdevice *alcDevice = alcOpenDevice(NULL);
		if(!alcDevice)
			return false;

		ALCcontext *alcContext = alcCreateContext(alcDevice, NULL);
		alcMakeContextCurrent(alcContext);
		return alGetError() == AL_NO_ERROR;
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
		string fileName(file);
		unsigned long lastPeriod = fileName.find_last_of('.');

		if(lastPeriod == string::npos || lastPeriod == (fileName.size() - 1))
		{
			setError("Audio::CreateFromFile: File extension unrecognizable <%s>.", file);
			return 0;
		}

		string extension = fileName.substr(lastPeriod + 1, string::npos);
		for(string::iterator it = extension.begin(); it != extension.end(); it++)
			*it = tolower(*it);

		Audio *audio = 0;

		if(!extension.compare("wav"))
			audio = Audio::LoadWAV(file);

		return audio;
	}

	Audio::Audio()
	{
		m_buffer = 0;
		alGenBuffers(1, &m_buffer);
	}
	Audio::~Audio()
	{
		alDeleteBuffers(1, &m_buffer);
	}
	void Audio::Play2D(bool spawnThread)
	{
		Play2DStruct *data = new Play2DStruct();
		data->audio = this;

		if(spawnThread)
		{
			Thread *thread = new Thread(Audio::Play2DProcess);
			data->thread = thread;
			thread->Start(data);
		}
		else
			Play2DProcess(data);
	}
}
