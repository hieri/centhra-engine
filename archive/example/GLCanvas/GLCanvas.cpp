//- Standard Library -
#include <stdio.h>

//- Centhra Engine -
#include <CE.h>

using namespace ce;

class gameGLCanvas : public Game
{
	ActionMap<gameGLCanvas> m_actionMap;
	Canvas *m_canvas;
	uiInstance *m_uiInstance;

public:
	void onStart()
	{
		char title[48];
		snprintf(title, 48, "Centhra Engine v%s - OpenGL Canvas Example", getVersionString().c_str());
		print("%s\n", title);

		m_uiInstance = new uiInstance(Vector2<int>(640, 480));
		m_uiInstance->setClearColor(Color(255, 0, 0, 255));

		m_canvas = Canvas::create(Vector2<int>(640, 480), 32, title, Canvas::Resizable);
		m_canvas->setUIInstance(m_uiInstance);

		m_actionMap.addPopAction(std::pair<unsigned int, unsigned short>(Key_Escape, Mod_None), &Game::quit);
		m_actionMap.attach(this);
		setActionMap((ActionMap<Game> *)&m_actionMap);
	}
	void onLoop()
	{
	}
	void onQuit()
	{
		delete m_uiInstance;
	}
};

int main(int argc, char **argv)
{
	gameGLCanvas *game = new gameGLCanvas;
	game->start();

	while(game->isRunning())
		game->process();

	delete game;
	return 0;
}
