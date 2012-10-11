//- SDL2 -
#include <SDL.h>
//#include <SDL_mixer.h>
#include <SDL_ttf.h>

//- DevIL -
#include <IL/il.h>
#include <IL/ilu.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Font.h>
#include <CE/Game.h>

// POTENTIAL: Add FPS update time variable to Game.
// TODO: Add support for multiple Game instances, if SDL ever supports multiple SDL contexts.

using namespace std;

namespace ce
{
	bool ms_ignoreNextMouseMotion = true;
	Game *Game::ms_current = 0;
	Game::Game()
	{
		m_actionMap = 0;
		m_fpsFrameCount = 0;
		m_fpsLastCheckTime = 0;
		m_isRunning = false;
		m_useVerticalSync = true;

		//- Set this Game instance as the current instance -
		setCurrent();
	}
	Game::~Game()
	{
		quit();
	}
	ActionMap<Game> *Game::getActionMap() const
	{
		return m_actionMap;
	}
	Game *Game::getCurrent()
	{
		return ms_current;
	}
	float Game::getFPS() const
	{
		return m_fps;
	}
	unsigned int Game::getRunTime() const
	{
		return SDL_GetTicks();
	}
	bool Game::getVerticalSync() const
	{
		return m_useVerticalSync;
	}
	bool Game::isRunning() const
	{
		return m_isRunning;
	}
	void Game::onLoop()
	{
	}
	void Game::onQuit()
	{
	}
	void Game::onStart()
	{
	}
	bool Game::process()
	{
		if(!m_isRunning)
			return false;

		//- Update FPS every 250ms -
		unsigned int time = getRunTime();
		m_fpsFrameCount++;
		if((time - m_fpsLastCheckTime) >= 250)
		{
			m_fps = (float)m_fpsFrameCount / (float)(time - m_fpsLastCheckTime) * 1000.f;
			m_fpsLastCheckTime = time;
			m_fpsFrameCount = 0;
		}

		//- Call the user-defined onLoop function -
		onLoop();

		SDL_Event event;
		while(SDL_PollEvent(&event))
			switch(event.type)
			{
				case SDL_DOLLARGESTURE:
					break;
				case SDL_DROPFILE:
					break;
				case SDL_FINGERDOWN:
					break;
				case SDL_FINGERMOTION:
					break;
				case SDL_FINGERUP:
					break;
				case SDL_JOYAXISMOTION:
					break;
				case SDL_JOYBALLMOTION:
					break;
				case SDL_JOYHATMOTION:
					break;
				case SDL_KEYDOWN:
					m_canvasMap[event.key.windowID]->onKeyPressed(event.key.keysym.sym, event.key.keysym.mod);
					if(m_actionMap)
						m_actionMap->push(event.key.keysym.sym, event.key.keysym.mod);
					break;
				case SDL_KEYUP:
					m_canvasMap[event.key.windowID]->onKeyReleased(event.key.keysym.sym, event.key.keysym.mod);
					if(m_actionMap)
						m_actionMap->pop(event.key.keysym.sym);
					break;
				case SDL_QUIT:
					quit();
					return false;
				case SDL_MOUSEMOTION:
					if(ms_ignoreNextMouseMotion)
						ms_ignoreNextMouseMotion = false;
					else
						m_canvasMap[event.motion.windowID]->onMouseMoved(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_canvasMap[event.button.windowID]->onMouseButtonPressed(event.button.button, event.button.x, event.button.y);
					break;
				case SDL_MOUSEBUTTONUP:
					m_canvasMap[event.button.windowID]->onMouseButtonReleased(event.button.button, event.button.x, event.button.y);
					break;
				case SDL_MOUSEWHEEL:
					m_canvasMap[event.wheel.windowID]->onMouseWheelMoved(event.wheel.x, event.wheel.y);
					break;
				case SDL_MULTIGESTURE:
					break;
				case SDL_TEXTEDITING:
					break;
				case SDL_TEXTINPUT:
			//		print("text in: %i %s\n", SDL_TEXTINPUTEVENT_TEXT_SIZE, event.text.text);
					break;
				case SDL_TOUCHBUTTONDOWN:
					break;
				case SDL_TOUCHBUTTONUP:
					break;
				case SDL_WINDOWEVENT:
					Canvas *canvas = m_canvasMap[event.window.windowID];
					switch(event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE:
							delete canvas;
							break;
						case SDL_WINDOWEVENT_ENTER:
							canvas->onMouseEnter();
							break;
						case SDL_WINDOWEVENT_EXPOSED:
							break;
						case SDL_WINDOWEVENT_FOCUS_GAINED:
							canvas->onFocusGained();
							break;
						case SDL_WINDOWEVENT_FOCUS_LOST:
							canvas->onFocusLost();
							if(m_actionMap)
								m_actionMap->flush();
							SDL_SetModState(KMOD_NONE);
							break;
						case SDL_WINDOWEVENT_HIDDEN:
					//		print("hidden\n");
							break;
						case SDL_WINDOWEVENT_LEAVE:
							canvas->onMouseLeave();
							break;
						case SDL_WINDOWEVENT_MAXIMIZED:
					//		print("maximized\n");
							break;
						case SDL_WINDOWEVENT_MINIMIZED:
					//		print("minimized\n");
							break;
						case SDL_WINDOWEVENT_MOVED:
						//	print("moved\n");
							break;
						case SDL_WINDOWEVENT_RESIZED:
					//		print("resized\n");
							break;
						case SDL_WINDOWEVENT_RESTORED:
					//		print("restored\n");
							break;
						case SDL_WINDOWEVENT_SHOWN:
					//		print("shown\n");
							break;
					}
					break;
			}

		//- Iterate onLoop for all Canvases -
		if(m_canvasVector.size())
			for(vector<Canvas *>::iterator it = m_canvasVector.begin(); it != m_canvasVector.end(); it++)
				if(!(*it)->onLoop())
					break;

		return true;
	}
	void Game::quit()
	{
		if(m_isRunning)
		{
			m_isRunning = false;

			//- Call the user-defined onInit function -
			onQuit();

			//- Delete all Canvases -
			while(m_canvasVector.size())
				delete m_canvasVector.at(0);

			//- Delete Default Resources -
			Font::deleteDefault();

			//- Quit SDL -
		//	Mix_Quit();
			TTF_Quit();
			SDL_Quit();
		}
	}
	void Game::setActionMap(ActionMap<Game> *actionMap)
	{
		m_actionMap = actionMap;
	}
	void Game::setCurrent()
	{
		ms_current = this;
	}
	void Game::setVerticalSync(bool status)
	{
		m_useVerticalSync = status;
	}
	void Game::sleep(unsigned int millis)
	{
		//- Delay SDL -
		SDL_Delay(millis);
	}
	bool Game::start()
	{
		if(m_isRunning)
			return false;

		//- Init SDL -
		int sdlFlags = SDL_INIT_TIMER | SDL_INIT_VIDEO;
	//	int sdlMixerFlags = MIX_INIT_OGG;//MIX_INIT_OGG | MIX_INIT_MP3;
		if(SDL_Init(sdlFlags) == -1)
		{
			setError("SDL_Init: %s", SDL_GetError());
			return false;
		}
	/*	if((Mix_Init(sdlMixerFlags) & sdlMixerFlags) != sdlMixerFlags)
		{
			SDL_Quit();
			setError("Mix_Init: %s", Mix_GetError());
			return false;
		}
	*/	if(TTF_Init() == -1)
		{
	//		Mix_Quit();
			SDL_Quit();
			setError("TTF_Init: %s", TTF_GetError());
			return false;
		}

		//- Init DevIL -
		ilInit();
		iluInit();

		m_isRunning = true;

		//- Call the user-defined onStart function -
		onStart();

		return true;
	}
}
