//- DevIL -
#include <IL/il.h>
#include <IL/ilu.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Image.h>

//- OpenGL -
#include <GL/gl.h>

//- TODO: create Image Loader abstraction -
//- TODO: externalize DevIL support into a plugin (ce_plugDevIL) -

using namespace std;

namespace ce
{
	bool Image::Init()
	{
		ilInit();
		iluInit();

		return true;
	}

	Image *Image::CreateFromFile(const char *file)
	{
		if(!fileExists(file))
		{
			setError("Image::createFromFile: File does not exist <%s>.", file);
			return 0;
		}

		unsigned int ilImage;
		ilGenImages(1, &ilImage);
		ilBindImage(ilImage);

		if(!ilLoadImage(file))
		{
			ilDeleteImages(1, &ilImage);
			setError("ilLoadImage: %s", iluErrorString(ilGetError()));
			return 0;
		}

		unsigned int bytesPerPixel = ilGetInteger(IL_IMAGE_BPP);
		unsigned int textureFormat = 0;
		bool conversion = false;
		switch(bytesPerPixel)
		{
			case 3:
				conversion = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
				textureFormat = GL_RGB;
				break;
			case 4:
				conversion = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				textureFormat = GL_RGBA;
				break;
			default:
				ilDeleteImages(1, &ilImage);
				setError("Image::createFromFile: Format not supported for <%s>", file);
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
		image->m_size = Vector2<int>(width, height);
		return image;
	}
	Image *Image::CreateFromMemory(const unsigned char *memory, unsigned int size)
	{
		if(!memory)
		{
			setError("Image::createFromMemory: Empty memory pointer");
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
				conversion = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
				break;
			case 4:
				conversion = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				break;
			default:
				ilDeleteImages(1, &ilImage);
				setError("Image::createFromFile: Format not supported.");
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
		image->m_size = Vector2<int>(width, height);
		return image;
	}
	Image *Image::CreateFromPixels(char *pixels, unsigned char bytesPerPixel, Vector2<int> size)
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
				setError("Image::createFromPixels: Format not supported.");
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
	Image::Image()
	{
		m_glTexture = 0;
		m_ilImage = 0;
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
	Vector2<int> Image::GetSize() const
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
