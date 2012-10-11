//- Standard Library -
#include <stdio.h>

//- Centhra Engine -
#include <CE.h>

using namespace ce;

class gameBasicUI : public Game
{
	ActionMap<gameBasicUI> m_actionMap;
	Canvas *m_canvas;
	Image *m_testSkin;
	ImageArray *m_imageArray;
	uiButton *m_button;
	uiButton::Template *m_buttonTemplate;
	uiImage *m_uiImage;
	uiInstance *m_uiInstance;
	uiText *m_fpsDisplay;
	uiWindow *m_window, *m_windowB, *m_windowC;
	uiWindow::Template *m_windowTemplate;

public:
	void onStart()
	{
		char title[48];
		snprintf(title, 48, "Centhra Engine v%s - Basic UI Example", getVersionString().c_str());
		print("%s\n", title);

		m_uiInstance = new uiInstance(Vector2<int>(640, 480));
		m_uiInstance->setClearColor(Color(127, 127, 127, 255));

		m_canvas = Canvas::create(Vector2<int>(640, 480), 32, title, Canvas::Resizable);
		m_canvas->setUIInstance(m_uiInstance);

		const unsigned char resWindowSkin[] = {
			#include <res/windowSkin.png.h>
		};
		m_testSkin = Image::createFromMemory(resWindowSkin, sizeof(resWindowSkin));
		m_imageArray = ImageArray::createFromImage(m_testSkin);

		m_uiImage = new uiImage(m_uiInstance);
		m_uiImage->setSize(Vector2<int>(256, 256));
		m_uiImage->setImage(m_testSkin);

		m_fpsDisplay = new uiText(m_uiInstance);
		m_fpsDisplay->setSize(Vector2<int>(256, 24));
		m_fpsDisplay->setPosition(Vector2<int>(2, 0));
		m_fpsDisplay->setTextSize(16);

		m_windowTemplate = new uiWindow::Template;
		m_windowTemplate->setEdgeSize(4);
		m_windowTemplate->setFont(Font::getDefault());
		m_windowTemplate->setMinimumSize(Vector2<int>(64, 64));
		m_windowTemplate->setSkin(m_imageArray);
		m_windowTemplate->setTitleHeight(32);
		m_windowTemplate->setTitlePosition(Vector2<int>(4, 5));
		m_windowTemplate->setTitleTextSize(16);

		m_window = new uiWindow(m_uiInstance, m_windowTemplate);
		m_window->setPosition(Vector2<int>(128, 128));
		m_window->setSize(Vector2<int>(256, 256));
		m_window->setTitle("Title of the Window");
		m_window->setCanMove(true);
		m_window->setCanResize(true);

		m_windowB = new uiWindow(m_uiInstance, m_windowTemplate);
		m_windowB->setPosition(Vector2<int>(16, 48));
		m_windowB->setSize(Vector2<int>(128, 64));
		m_windowB->setTitle("Child A");
		m_windowB->setCanMove(true);
		m_windowB->setCanResize(true);
		m_window->add(m_windowB);

		m_windowC = new uiWindow(m_uiInstance, m_windowTemplate);
		m_windowC->setPosition(Vector2<int>(16, 128));
		m_windowC->setSize(Vector2<int>(128, 128));
		m_windowC->setTitle("Child B");
		m_windowC->setCanMove(true);
		m_windowC->setCanResize(true);
		m_window->add(m_windowC);

		m_testSkin = Image::createFromFile("BasicUI/res/buttonSkin.png");
		m_imageArray = ImageArray::createFromImage(m_testSkin);

		m_buttonTemplate = new uiButton::Template;
		m_buttonTemplate->setFont(Font::getDefault());
		m_buttonTemplate->setSkin(m_imageArray);
		m_buttonTemplate->setTextSize(16);
		m_buttonTemplate->setToggle(true);

		m_button = new uiButton(m_uiInstance, m_buttonTemplate);
		m_button->setPosition(Vector2<int>(16, 48));
		m_button->setSize(Vector2<int>(64, 64));
		m_button->setText("Button");
		m_windowC->add(m_button);

		m_actionMap.addPopAction(std::pair<unsigned int, unsigned short>(Key_Escape, Mod_None), &Game::quit);
		m_actionMap.attach(this);
		setActionMap((ActionMap<Game> *)&m_actionMap);

		Cursor *cur = Cursor::getDefault(Cursor::Normal);
		m_canvas->setCursor(cur);
	//	Canvas::getCurrent()->setCursor(Cursor::getDefault(Cursor::Normal));
	}
	void onLoop()
	{
		m_fpsDisplay->setText("%.1f", getFPS());
	}
	void onQuit()
	{
		delete m_uiInstance;
	}
};

int main(int argc, char **argv)
{
	gameBasicUI *game = new gameBasicUI;	
	game->start();

	while(game->isRunning())
		game->process();

	delete game;

	return 0;
}
