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
    bool onStart()
    {
        print("Initializing Image Library\n");
        Image::init();

        print("Loading <centhra.png>\n");
        m_image = Image::createFromFile("centhra.png");

        Vector2<int> imageSize = m_image->getSize();
        print("  Width: %d Height: %d\n", imageSize.getX(), imageSize.getY());

        return true;
    }
    bool onProcess()
    {
        return false;
    }
    bool onStop(bool force)
    {
        delete m_image;

        return true;
    }
};

int main(int argc, char **argv)
{
    print("000 - Test | Centhra Engine v%s\n", getVersionString().c_str());

    AppTest myApp;
    myApp.start();

    //- Run the App's main loop. -
    while(myApp.isRunning())
        myApp.process();

    return 0;
}
