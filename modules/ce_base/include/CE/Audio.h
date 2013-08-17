#ifndef _CE_BASE_AUDIO_H_
#define _CE_BASE_AUDIO_H_

namespace ce
{
	class Audio
	{
		Audio();

	public:
		static bool Init();
		static void Cleanup();

	};
}

#endif
