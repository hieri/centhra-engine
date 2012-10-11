//- Centhra Engine -
#include <CE.h>

using namespace ce;

class gameHelloWorld : public Game
{
	int m_ticks;

public:
	void onStart()
	{
		print("Centhra Engine v%s - Hello World Example\n", getVersionString().c_str());

		m_ticks = 0;
		print("->onStart\n");
	}
	void onLoop()
	{
		m_ticks++;

		print("  ->onLoop [%i/16]\n", m_ticks);

		if(m_ticks >= 16)
			quit();
	}
	void onQuit()
	{
		print("->onQuit\n");
	}
};

int main(int argc, char **argv)
{
	setVerboseLogging(true);
	gameHelloWorld *game = new gameHelloWorld;
	game->start();

	while(game->isRunning())
		game->process();

	delete game;
	return 0;
}
