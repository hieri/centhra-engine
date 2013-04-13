//- Centhra Engine -
#include <CE/AppFrontend.h>
#include <CE/Base.h>
#include <CE/Canvas.h>
#include <CE/Image.h>
#include <CE/Game2D/Sprite.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace ce;

//- Define your own implementation of the AppFrontend class. -
class AppTest : public AppFrontend
{
	Canvas *m_canvas;
	Image *m_image;
	game2d::Sprite *m_sprite;

public:
	AppTest()
	{
		m_canvas = 0;
		m_image = 0;
		m_sprite = 0;
	}

	//- Define the virtual functions for the class. -
	bool OnStart()
	{
		print("Initializing Image Library\n");
		Image::Init();

		m_canvas = Canvas::Create(300, 300, "000 - Test");

		print("Loading <../000 - Test/sprite.png>\n");
		m_image = Image::CreateFromFile("../000 - Test/sprite.png");
		m_sprite = new game2d::Sprite(m_image);
		float times[3] = { 1.f, 2.f, 3.f };
		m_sprite->AddAnimation(Vector2<float>(0.f, 0.f), Vector2<float>(15.f/256.f, 15.f/256.f), Vector2<float>(15.f/256.f, 0.f), times, 3);

		if(m_image)
		{
			Vector2<unsigned int> imageSize = m_image->GetSize();
			print("  Width: %d Height: %d\n", imageSize.GetX(), imageSize.GetY());
		}
		else
			print("  Unable to load image.\n");

		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		delete m_sprite;
		delete m_image;
		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::PostRender:
				if(m_image)
				{
					glEnable(GL_TEXTURE_2D);
						glPushMatrix();
							glScalef(128.f, 128.f, 1.f);
							m_sprite->Draw(0, ((float)GetRunTimeMS()) / 1000.f);
						glPopMatrix();
					glDisable(GL_TEXTURE_2D);
				}
				break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("000 - Test | Centhra Engine v%s\n", getVersionString().c_str());

	AppTest myApp;
	myApp.Start();

	//- Run the App's main loop. -
	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
