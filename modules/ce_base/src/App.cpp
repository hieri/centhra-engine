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
	App *App::GetCurrent()
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
		Stop(true);
	}
	unsigned long App::GetRunTimeMS() const
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
				currTimeMS = (unsigned long)((1000 * count.QuadPart) / frequency.QuadPart);
			}
			else
				currTimeMS = GetTickCount();
		#endif

		return currTimeMS - m_startTimeMS;
	}
	bool App::IsRunning() const
	{
		return m_isRunning;
	}
	bool App::Process()
	{
		if(!m_isRunning)
			return false;

		bool keepRunning = OnProcess();
		if(m_isRunning && !keepRunning)
			Stop(true);

		return m_isRunning;
	}
	void App::SetCurrent()
	{
		ms_current = this;
	}
	bool App::Start()
	{
		if(m_isRunning)
			return false;

		SetCurrent();

		m_isRunning = OnStart();

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
					m_startTimeMS = (unsigned long)((1000 * count.QuadPart) / frequency.QuadPart);
				}
				else
					m_startTimeMS = GetTickCount();
			#endif

			OnStarted();
		}

		return m_isRunning;
	}
	bool App::Stop(bool force)
	{
		if(!m_isRunning)
			return false;

		bool isValid = OnStop(force) || force;
		if(isValid)
		{
			m_isRunning = false;
			OnStopped();
		}

		return isValid;
	}

	bool App::OnProcess()
	{
		return true;
	}
	bool App::OnStart()
	{
		return true;
	}
	void App::OnStarted()
	{
	}
	bool App::OnStop(bool force)
	{
		return true;
	}
	void App::OnStopped()
	{
	}
	void App::OnError(const char *str)
	{
	}
	void App::OnPrint(const char *str)
	{
	}
	void App::OnWarn(const char *str)
	{
	}
}
