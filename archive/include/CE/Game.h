#ifndef CE_GAME_H
#define CE_GAME_H

//- Standard Library -
#include <map>
#include <string>
#include <vector>

//- Centhra Engine -
#include <CE/ActionMap.h>

namespace ce
{
	class Canvas;

	/* 	Game Class
	 * 	- Template class meant for housing game data and running the game.
	 */
	class Game
	{
		static Game *ms_current;

		ActionMap<Game> *m_actionMap;
		bool m_isRunning, m_useVerticalSync;
		float m_fps;
		std::map<unsigned int, Canvas *> m_canvasMap;
		std::vector<Canvas *> m_canvasVector;
		unsigned int m_fpsFrameCount, m_fpsLastCheckTime;

	public:
		friend class Canvas;
		static Game *getCurrent();

		Game();
		~Game();

		float getFPS() const;
		unsigned int getRunTime() const;
		ActionMap<Game> *getActionMap() const;
		bool getVerticalSync() const;
		bool isRunning() const;
		bool process();
		void quit();
		void setActionMap(ActionMap<Game> *actionMap);
		void setCurrent();
		void setVerticalSync(bool status);
		void sleep(unsigned int milliseconds);
		bool start();

		//- User-Defined Functions -
		virtual void onLoop();
		virtual void onQuit();
		virtual void onStart();
	};
}

#endif
