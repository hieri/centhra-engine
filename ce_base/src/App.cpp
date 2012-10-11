#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

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
		quit();
	}
	unsigned int App::getRunTimeMS() const
	{
		unsigned int currTimeMS;
		struct timeval currTime;
		gettimeofday(&currTime, 0);
		currTimeMS = currTime.tv_sec * 1000 + currTime.tv_usec / 1000;

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

		return onLoop();
	}
	bool App::quit()
	{
		if(!m_isRunning)
			return false;

		m_isRunning = false;

		return onQuit();
	}
	void App::setCurrent()
	{
		ms_current = this;
	}
	void App::sleep(unsigned int timeMS)
	{
		usleep(timeMS * 1000);
	}
	bool App::start()
	{
		if(m_isRunning)
			return false;

		setCurrent();

		m_isRunning = true;

		timeval currTime;
		gettimeofday(&currTime, 0);
		m_startTimeMS = currTime.tv_sec * 1000 + currTime.tv_usec / 1000;

		return onStart();
	}

	bool App::onLoop()
	{
		return true;
	}
	bool App::onQuit()
	{
		return true;
	}
	bool App::onStart()
	{
		return true;
	}
}