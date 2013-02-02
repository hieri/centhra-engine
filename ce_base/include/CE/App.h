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
		unsigned long m_startTimeMS;

	public:
		static App *getCurrent();

		App();
		~App();

		unsigned long getRunTimeMS() const;
		bool isRunning() const;
		bool process();
		bool quit(bool force = false);
		void setCurrent();
		void sleep(unsigned long timeMS);
		bool start();

		//- User-Defined Functions -
		virtual bool onLoop();
		virtual bool onQuit(bool force = false);
		virtual bool onStart();
	};
}

#endif
