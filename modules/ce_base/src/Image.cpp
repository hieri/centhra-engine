#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- DevIL -
#include <IL/il.h>
#include <IL/ilu.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Image.h>
#include <CE/Plugin.h>
#include <CE/Mutex.h>

//TODO: create Image Loader abstraction
//TODO: externalize DevIL support into a plugin (ce_plugDevIL)

using namespace std;

namespace ce
{
	Mutex g_imageMutex;

	bool Image::Init()
	{
		Plugin::InitializeByType(Plugin::Image);

		ilInit();
		iluInit();

		g_imageMutex.Init();
		return true;
	}
	void Image::Cleanup()
	{
		g_imageMutex.Destroy();
	}

	bool Image::LoadFromFile(const char *file)
	{
		unsigned int ilImage;
		ilGenImages(1, &ilImage);
		ilBindImage(ilImage);

		if(!ilLoadImage(file))
		{
			ilDeleteImages(1, &ilImage);
			setError("ilLoadImage: %s", iluErrorString(ilGetError()));
			return false;
		}

		unsigned int bytesPerPixel = ilGetInteger(IL_IMAGE_BPP);
		unsigned int textureFormat = 0;
		bool conversion = false;
		switch(bytesPerPixel)
		{
		case 3:
			conversion = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE) != 0;
			textureFormat = GL_RGB;
			break;
		case 4:
			conversion = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) != 0;
			textureFormat = GL_RGBA;
			break;
		default:
			ilDeleteImages(1, &ilImage);
			setError("Image::CreateFromFile: Format not supported for <%s>", file);
			return false;
		}

		if(!conversion)
		{
			ilDeleteImages(1, &ilImage);
			setError("ilConvertImage: %s", iluErrorString(ilGetError()));
			return false;
		}

		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		m_bytesPerPixel = bytesPerPixel;
		m_ilImage = ilImage;
		m_size = Vector2<unsigned int>(width, height);

		return true;
	}
	Image *Image::CreateFromFile(const char *file)
	{
		if(!fileExists(file))
		{
			setError("Image::CreateFromFile: File does not exist <%s>.", file);
			return 0;
		}

		Image *image = new Image;
		if(!image->LoadFromFile(file))
		{
			delete image;
			return 0;
		}

		File *fileObj = new File(file);
		fileObj->SetObject(image);

		return image;
	}
	Image *Image::CreateFromMemory(const unsigned char *memory, unsigned int size)
	{
		if(!memory)
		{
			setError("Image::CreateFromMemory: Empty memory pointer");
			return 0;
		}

		unsigned int ilImage;
		ilGenImages(1, &ilImage);
		ilBindImage(ilImage);

		if(!ilLoadL(IL_TYPE_UNKNOWN, memory, size))
		{
			ilDeleteImages(1, &ilImage);
			setError("ilLoadL: %s", iluErrorString(ilGetError()));
			return 0;
		}

		unsigned int bytesPerPixel = ilGetInteger(IL_IMAGE_BPP);
		bool conversion = false;
		switch(bytesPerPixel)
		{
			case 3:
				conversion = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE) != 0;
				break;
			case 4:
				conversion = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) != 0;
				break;
			default:
				ilDeleteImages(1, &ilImage);
				setError("Image::CreateFromFile: Format not supported.");
				return 0;
		}

		if(!conversion)
		{
			ilDeleteImages(1, &ilImage);
			setError("ilConvertImage: %s", iluErrorString(ilGetError()));
			return 0;
		}

		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);

		Image *image = new Image;
		image->m_bytesPerPixel = bytesPerPixel;
		image->m_ilImage = ilImage;
		image->m_size = Vector2<unsigned int>(width, height);
		return image;
	}
	Image *Image::CreateFromPixels(char *pixels, unsigned char bytesPerPixel, Vector2<unsigned int> size)
	{
		unsigned int imageFormat = 0;
		switch(bytesPerPixel)
		{
			case 3:
				imageFormat = IL_RGB;
				break;
			case 4:
				imageFormat = IL_RGBA;
				break;
			default:
				setError("Image::CreateFromPixels: Format not supported.");
				return 0;
		}

		unsigned int ilImage;
		ilGenImages(1, &ilImage);
		ilBindImage(ilImage);

		if(!ilTexImage(size[0], size[1], 1, bytesPerPixel, imageFormat, IL_UNSIGNED_BYTE, pixels))
		{
			ilDeleteImages(1, &ilImage);
			setError("ilTexImage: %s", iluErrorString(ilGetError()));
			return 0;
		}

		Image *image = new Image;
		image->m_bytesPerPixel = bytesPerPixel;
		image->m_ilImage = ilImage;
		image->m_size = size;

		return image;
	}
	Image *Image::CreateFromScreen(Vector2<unsigned int> start, Vector2<unsigned int> end)
	{
		Vector2<unsigned int> size = end - start;
		char *pixels = new char[3 * size[0] * size[1]];
		glReadPixels(start[0], start[1], size[0], size[1], GL_RGB, GL_BYTE, pixels);

		Image *image = CreateFromPixels(pixels, 3, size);
		delete [] pixels;

		return image;
	}
	Image::Image() : File::FileObject()
	{
		m_glTexture = 0;
		m_ilImage = 0;
	}
	void Image::OnFileChange()
	{
		if(m_ilImage)
		{
			ilBindImage(0);
			ilDeleteImages(1, &m_ilImage);
			m_ilImage = 0;
		}
		g_imageMutex.Lock();
			if(m_glTexture)
			{
				glDeleteTextures(1, &m_glTexture);
				m_glTexture = 0;
			}
			LoadFromFile(m_file->GetFilePath().c_str());
		g_imageMutex.Unlock();
	}
	Image::~Image()
	{
		if(m_ilImage)
			ilDeleteImages(1, &m_ilImage);
		if(m_glTexture)
			glDeleteTextures(1, &m_glTexture);
	}
	void Image::Bind()
	{
		//- Create OpenGL Texture -
		g_imageMutex.Lock();
		if(!m_glTexture)
		{
			ilBindImage(m_ilImage);
			glGenTextures(1, &m_glTexture);
			glBindTexture(GL_TEXTURE_2D, m_glTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, m_bytesPerPixel, m_size[0], m_size[1], 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		}
		else
			glBindTexture(GL_TEXTURE_2D, m_glTexture);
		g_imageMutex.Unlock();
	}
	Color Image::GetPixel(unsigned int x, unsigned int y) const
	{
		Color color;
		ilBindImage(m_ilImage);
		unsigned char *data = ilGetData();
		unsigned int bytesPerPixel = ilGetInteger(IL_IMAGE_BPP);

		for(unsigned int a = 0; a < bytesPerPixel; a++)
			color[a] = data[(y * m_size[0] + x) * bytesPerPixel + a];

		return color;
	}
	Vector2<unsigned int> Image::GetSize() const
	{
		return m_size;
	}
	bool Image::SaveToFile(const char *file) const
	{
		ilEnable(IL_FILE_OVERWRITE);
		ilBindImage(m_ilImage);
		if(!ilSaveImage(file))
		{
			setError("ilSaveImage: %s", iluErrorString(ilGetError()));
			ilDisable(IL_FILE_OVERWRITE);
			return false;
		}
		ilDisable(IL_FILE_OVERWRITE);
		return true;
	}
}
