#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>
#include <GL/glu.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Font.h>
#include <CE/Frontend.h>
#include <CE/UI.h>

using namespace ce;

class AppUserInterfaceSample : public AppFrontend
{
	Canvas *m_canvas;
	ui::Control *m_rootCtrl;
	Font *m_font;
	Image *m_scrollImage;
	ui::Skin *m_scrollSkin;

public:
	AppUserInterfaceSample()
	{
		m_canvas = 0;
		m_rootCtrl = 0;
		m_font = 0;
	}
	bool OnStart()
	{
		m_canvas = Canvas::Create(640, 480, "300 - User Interface");

		Image::Init();
		Font::Init();
		m_font = Font::CreateFromFile("../res/FreeMono.ttf", 14, m_canvas->GetHorizontalDPI(), m_canvas->GetVerticalDPI());

		m_rootCtrl = new ui::Control(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(1280, 720));
		
		{
//			m_rootCtrl->Add(new ui::ColorCtrl(Vector2<int_canvas>(0, 0), Vector2<int_canvas>(64, 64), Color(255, 0, 0)));
			
			{
				ui::TextCtrl *textCtrl = new ui::TextCtrl(Vector2<int_canvas>(64, 0), Vector2<int_canvas>(256, 64), m_font, "Igggg\nIgggg\nIgggg", Color<float>(1.f, 1.f, 1.f, 1.f));
				textCtrl->SetWrapping(true);
				m_rootCtrl->Add(textCtrl);
			}

/*			ui::Control *random = new ui::ColorCtrl(Vector2<int_canvas>(64, 64), Vector2<int_canvas>(64, 64), Color(255, 255, 0));
			m_rootCtrl->Add(random);

			for(int a = 0; a < 4; a++)
				m_rootCtrl->Add(new ui::TextEditCtrl(Vector2<int_canvas>(0, 200 + a * 24), Vector2<int_canvas>(400, 128), m_font, 128, "A: ", Color(255, 255, 0)));
			for(int a = 0; a < 4; a++)
			{
				ui::Control *ctrl = new ui::TextEditCtrl(Vector2<int_canvas>(300, 200 + a * 24), Vector2<int_canvas>(400, 128), m_font, 128, "A: ", Color(255, 255, 0));
				m_rootCtrl->Add(ctrl);
			}*/
		}

		
		{
			m_scrollImage = Image::CreateFromFile("../res/scrollSkin.png");
			m_scrollSkin = new ui::Skin(m_scrollImage);
			ui::ScrollCtrl *testScroll = new ui::ScrollCtrl(Vector2<int_canvas>(200, 0), Vector2<int_canvas>(400, 400), m_scrollSkin);
			m_rootCtrl->Add(testScroll);

			for(int a = 0; a < 4; a++)
				testScroll->Add(new ui::TextEditCtrl(Vector2<int_canvas>(0, 64 + a * 24), Vector2<int_canvas>(257, 128), m_font, 128, "A: ", Color<float>(0.f, 1.f, 0.f, 1.f)));
			for(int a = 0; a < 4; a++)
			{
				ui::Control *ctrl = new ui::TextEditCtrl(Vector2<int_canvas>(300, 364 + a * 24), Vector2<int_canvas>(400, 128), m_font, 128, "A: ", Color<float>(0.f, 1.f, 0.f, 1.f));
				testScroll->Add(ctrl);
			}

			{
				ui::TextDropDownCtrl *dropDownCtrl = new ui::TextDropDownCtrl(Vector2<int_canvas>(64, 300), Vector2<int_canvas>(128, 20), m_font, 150, "Select Here");
				dropDownCtrl->AddSelection(0, "Item A");
				dropDownCtrl->AddSelection(2, "Item B");
				dropDownCtrl->AddSelection(1, "Item C");
				testScroll->Add(dropDownCtrl);
			}
		}

		return true;
	}
	bool OnProcess()
	{
		sleepMS(1);
		return true;
	}
	void OnStopped()
	{
		delete m_rootCtrl;

		delete m_scrollSkin;
		delete m_scrollImage;
		delete m_font;
		delete m_canvas;
	}
	bool OnEvent(Event &event)
	{
		switch(event.type)
		{
			case event::KeyDown:
				if(event.key.keyCode == Key_Tab)
				{
					ui::Control *nextFocus = m_rootCtrl->FindNextFocus(ui::Control::GetCurrentFocus());
//					ui::Control *nextFocus = m_rootCtrl->FindPreviousFocus(ui::Control::GetCurrentFocus());
					if(nextFocus)
						nextFocus->Focus();
					break;
				}

				if(ui::Control::GetCurrentFocus())
					ui::Control::GetCurrentFocus()->ProcessEvent(event);
				break;
			case event::KeyUp:
				if(ui::Control::GetCurrentFocus())
					ui::Control::GetCurrentFocus()->ProcessEvent(event);
				break;
			case event::MouseButtonDown:
				if(m_rootCtrl)
					if(m_rootCtrl->ProcessEvent(event))
						m_rootCtrl->SetFocusByPosition(Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y));
				break;
			case event::MouseButtonUp:
			case event::MouseMotion:
				if(m_rootCtrl)
					m_rootCtrl->ProcessEvent(event);
			case event::MouseScroll:
				if(m_rootCtrl)
					m_rootCtrl->ProcessEvent(event);
				break;
			case event::PostRender:
				m_rootCtrl->Render(m_canvas);
				break;
			case event::WindowResize:
				if(m_rootCtrl)
					m_rootCtrl->SetExtent(Vector2<int_canvas>(event.windowResize.width, event.windowResize.height));
				break;
		}
		return true;
	}
};

int main(int argc, char **argv)
{
	print("300 - User Interface | Centhra Engine v%s\n", getVersionString().c_str());

	AppUserInterfaceSample myApp;
	myApp.Start();

	while(myApp.IsRunning())
		myApp.Process();

	return 0;
}
