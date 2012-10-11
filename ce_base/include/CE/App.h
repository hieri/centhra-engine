#ifndef _CE_APP_H_
#define _CE_APP_H_

//- Standard Library -
#include <map>
#include <string>
#include <vector>

//- Centhra Engine -
#include <CE/ConfigBase.h>

namespace ce
{
	class App
	{
		static App *ms_current;

		bool m_isRunning;
		unsigned int m_startTimeMS;

	public:
		static App *getCurrent();

		App();
		~App();

		unsigned int getRunTimeMS() const;
		bool isRunning() const;
		bool process();
		bool quit();
		void setCurrent();
		void sleep(unsigned int timeMS);
		bool start();

		//- User-Defined Functions -
		virtual bool onLoop();
		virtual bool onQuit();
		virtual bool onStart();
	};
}

#endif
