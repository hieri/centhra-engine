//- Centhra Engine -
#include <CE/App.h>
#include <CE/Base.h>
#include <CE/Image.h>

using namespace ce;

//- Define your own implementation of the App class. -
class AppTest : public App
{
    Image *m_image;

public:
    AppTest()
    {
        m_image = 0;
    }

    //- Define the virtual functions for the class. -
    bool OnStart()
    {
        print("Initializing Image Library\n");
        Image::Init();

        print("Loading <centhra.png>\n");
        m_image = Image::CreateFromFile("centhra.png");

        Vector2<int> imageSize = m_image->GetSize();
        print("  Width: %d Height: %d\n", imageSize.GetX(), imageSize.GetY());

        return true;
    }
    bool OnProcess()
    {
        return false;
    }
    bool OnStop(bool force)
    {
        delete m_image;

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
