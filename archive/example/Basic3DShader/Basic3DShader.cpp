//- Standard Library -
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

//- Centhra Engine -
#include <CE.h>
#include <CE_o3d.h>

#ifdef _WIN32
	#include <windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace ce;

class gameBasic3DShader : public Game
{
	ActionMap<gameBasic3DShader> m_actionMap;
	Canvas *m_canvas;
	o3dBox *m_o3dBox;
	o3dBox *m_o3dPlane;
	o3dCamera *m_o3dCamera;
	o3dInstance *m_o3dInstance;
	Shader *m_fragmentShader;
	Shader *m_vertexShader;
	ShaderProgram *m_shaderProgram;
	uiInstance *m_uiInstance;
	uiView3d *m_uiView3d;

public:
	void onStart()
	{
		char title[48];
		snprintf(title, 48, "Centhra Engine v%s - Basic 3D Shader Example", getVersionString().c_str());
		print("%s\n", title);

		m_uiInstance = new uiInstance(Vector2<int>(640, 480));
		m_uiInstance->setClearColor(Color(127, 127, 127, 255));

		m_o3dInstance = new o3dInstance;

		m_canvas = Canvas::create(Vector2<int>(640, 480), 32, title);
		m_canvas->setUIInstance(m_uiInstance);

		m_fragmentShader = Shader::createFromFile("shaders/fragment.glsl", Shader::Fragment);
		m_vertexShader = Shader::createFromFile("shaders/vertex.glsl", Shader::Vertex);

		m_shaderProgram = new ShaderProgram;
		m_shaderProgram->attachShader(m_fragmentShader);
		m_shaderProgram->attachShader(m_vertexShader);
		m_shaderProgram->link();
		m_shaderProgram->use();

		m_o3dPlane = new o3dBox(m_o3dInstance, Vector3<float>(100.f, 8.f, 100.f));
		m_o3dPlane->setPosition(Vector3<float>(0.f, -20.f, 0.f));
		m_o3dPlane->setStatic(true);

		srand((unsigned int)time(0));

		for(unsigned int a = 0; a < 16; a++)
			for(unsigned int b = 0; b < 16; b++)
			{
				o3dBox *box = new o3dBox(m_o3dInstance);
				box->setPosition(Vector3<float>(a * 2.f - 16.f, 20.f, b * 2.f - 16.f));
				box->setRotation(Quaternion<float>((float)(rand() % 360), (float)(rand() % 360), (float)(rand() % 360), true));
			}

		m_o3dCamera = new o3dCamera(m_o3dInstance);
		m_o3dCamera->setSpeed(10.f);
		m_o3dCamera->setPosition(Vector3<float>(0.f, 0.f, 10.f));

		m_o3dBox = new o3dBox(m_o3dInstance);
		m_o3dBox->setPosition(Vector3<float>(0.f, 10.f, 0.f));
		m_o3dBox->setScale(Vector3<float>(5.f, 5.f, 5.f));
		m_o3dBox->setRotation(Quaternion<float>((float)(rand() % 360), (float)(rand() % 360), (float)(rand() % 360), true));

		m_uiView3d = new uiView3d(m_uiInstance);
		m_uiView3d->setSize(Vector2<int>(640, 480));
		m_uiView3d->setCamera(m_o3dCamera);
		m_uiView3d->setInputScale(Vector2<float>(0.25f, 0.25f));

		m_uiInstance->setKeyFocus(m_uiView3d);
		m_uiInstance->setMouseFocus(m_uiView3d);
		m_canvas->setCaptureMouse(true);
		m_canvas->showMouse(false);

		m_o3dInstance->startPhysics();
		m_o3dInstance->setPhysicsUpdateRate(60);

		m_actionMap.addPopAction(std::pair<unsigned int, unsigned short>(Key_Escape, Mod_None), &Game::quit);
		m_actionMap.attach(this);
		setActionMap((ActionMap<Game> *)&m_actionMap);
	}
	void onLoop()
	{
	}
	void onQuit()
	{
		delete m_shaderProgram;
		delete m_fragmentShader;
		delete m_vertexShader;
		delete m_o3dInstance;
		delete m_uiInstance;
	}
};

int main(int argc, char **argv)
{
	setVerboseLogging(true);
	gameBasic3DShader *game = new gameBasic3DShader;
	game->start();

	while(game->isRunning())
		game->process();

	delete game;
	return 0;
}
