#ifdef linux
	//- Linux -
	#include <sys/time.h>
	#include <stdio.h>
	#include <unistd.h>
#endif

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/App.h>

namespace ce
{
	App *App::ms_current = 0;
	App *App::getCurrent()
	{
		return ms_current;
	}

	App::App()
	{
		m_isRunning = false;
		m_startTimeMS = 0;
	}
	App::~App()
	{
		stop(true);
	}
	unsigned long App::getRunTimeMS() const
	{
		unsigned long currTimeMS;

		#ifdef linux
			struct timeval currTime;
			gettimeofday(&currTime, 0);
			currTimeMS = currTime.tv_sec * 1000 + currTime.tv_usec / 1000;
		#endif

		#ifdef _WIN32
			LARGE_INTEGER frequency;
			if(QueryPerformanceFrequency(&frequency))
			{
				LARGE_INTEGER count;
				QueryPerformanceCounter(&count);
				currTimeMS = (1000 * count.QuadPart) / frequency.QuadPart;
			}
			else
				currTimeMS = GetTickCount();
		#endif

		return currTimeMS - m_startTimeMS;
	}
	bool App::isRunning() const
	{
		return m_isRunning;
	}
	bool App::process()
	{
		if(!m_isRunning)
			return false;

		bool keepRunning = onProcess();
		if(m_isRunning && !keepRunning)
			stop(true);

		return m_isRunning;
	}
	void App::setCurrent()
	{
		ms_current = this;
	}
	void App::sleepMS(unsigned long timeMS)
	{
		#ifdef linux
			usleep(timeMS * 1000);
		#endif

		#ifdef _WIN32
			Sleep(timeMS);
		#endif
	}
	bool App::start()
	{
		if(m_isRunning)
			return false;

		setCurrent();

		m_isRunning = onStart();

		if(m_isRunning)
		{
			#ifdef linux
				timeval currTime;
				gettimeofday(&currTime, 0);
				m_startTimeMS = currTime.tv_sec * 1000 + currTime.tv_usec / 1000;
			#endif

			#ifdef _WIN32
				LARGE_INTEGER frequency;
				if(QueryPerformanceFrequency(&frequency))
				{
					LARGE_INTEGER count;
					QueryPerformanceCounter(&count);
					m_startTimeMS = (1000 * count.QuadPart) / frequency.QuadPart;
				}
				else
					m_startTimeMS = GetTickCount();
			#endif
		}

		return m_isRunning;
	}
	bool App::stop(bool force)
	{
		if(!m_isRunning)
			return false;

		bool isValid = onStop(force) || force;

		if(isValid)
			m_isRunning = false;

		return isValid;
	}

	bool App::onProcess()
	{
		return true;
	}
	bool App::onStart()
	{
		return true;
	}
	bool App::onStop(bool force)
	{
		return true;
	}
}