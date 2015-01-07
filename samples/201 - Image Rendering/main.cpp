#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Image.h>
#include <CE/Frontend.h>

using namespace ce;

class AppImageRenderingSample : public AppFrontend
{
	Canvas *m_canvas;
	Image *m_image;
	Directory *m_directory;

public:
	AppImageRenderingSample()
	{
		m_canvas = 0;
		m_image = 0;
		m_directory = 0;
	}
	bool OnStart()
	{
		print("Initializing Image Library\n");
		Image::Init();

		m_canvas = Canvas::Create(300, 300, "201 - Image Rendering");

		print("Loading <../res/centhra.png>\n");
		m_image = Image::CreateFromFile("../res/centhra.png");

		m_directory = Directory::GetFromPath("../res/");
		m_directory->SetMonitoring(true);

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
		sleepMS(15);
		return true;
	}
	void OnStopped()
	{
		delete m_image;
		Image::Cleanup();

		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::PostRender:
				if(m_image)
				{
					//TODO: Update rendering
/*					glEnable(GL_TEXTURE_2D);
						m_image->Bind();
						glBegin(GL_QUADS);
							glTexCoord2i(0, 1);
							glVertex2i(0, 0);

							glTexCoord2i(1, 1);
							glVertex2i(300, 0);

							glTexCoord2i(1, 0);
							glVertex2i(300, 300);

							glTexCoord2i(0, 0);
							glVertex2i(0, 300);
						glEnd();
					glDisable(GL_TEXTURE_2D);*/
				}
				break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("201 - Image Rendering | Centhra Engine v%s\n", getVersionString().c_str());

	AppImageRenderingSample myApp;
	myApp.Start();

	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
