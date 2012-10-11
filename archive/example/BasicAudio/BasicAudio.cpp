//- Centhra Engine -
#include <CE.h>

using namespace ce;

class gameBasicAudio : public Game
{

public:
	void onStart()
	{
		print("Centhra Engine v%s - Basic Audio Example\n", getVersionString().c_str());
	}
	void onLoop()
	{
	}
	void onQuit()
	{
	}
};

int main(int argc, char **argv)
{
	setVerboseLogging(true);
	gameBasicAudio *game = new gameBasicAudio;
	game->start();

	while(game->isRunning())
		game->process();

	delete game;
	return 0;
}
