#ifndef _CE_IMAGE_H_
#define _CE_IMAGE_H_

//- Standard Library -
#include <map>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector2.h>

namespace ce
{
	/* 	Image Class
	 * 	- Holds image data.
	 */
	class Image
	{
		unsigned char m_bytesPerPixel;
		unsigned int m_glTexture, m_ilImage;
		Vector2<int> m_size;

		Image();

	public:
		static bool init();

		static Image *createFromFile(const char *file);
		static Image *createFromMemory(const unsigned char *memory, unsigned int size);
		static Image *createFromPixels(char *pixels, unsigned char bytesPerPixel, Vector2<int> size);

		~Image();

		void bind();
		Color getPixel(unsigned int x, unsigned int y) const;
		Vector2<int> getSize() const;
		bool saveToFile(const char *file) const;
	};
}

#endif
