#ifndef _CE_IMAGE_H_
#define _CE_IMAGE_H_

//- Standard Library -
#include <map>

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/Vector2.h>

namespace ce
{
	/**	@brief Image Data Structure Class
	 */
	class Image
	{
		unsigned char m_bytesPerPixel;
		unsigned int m_glTexture, m_ilImage;
		Vector2<int> m_size;

		Image();

	public:
		/**	@brief Initializes the registered image loaders.
		 */
		static bool Init();

		/**	@brief File Constructor.
		 */
		static Image *CreateFromFile(const char *file);

		/**	@brief Memory Constructor.
		 */
		static Image *CreateFromMemory(const unsigned char *memory, unsigned int size);

		/**	@brief Pixel Array Constructor.
		 */
		static Image *CreateFromPixels(char *pixels, unsigned char bytesPerPixel, Vector2<int> size);

		/**	@brief Destructor.
		 */
		~Image();

		/**	@brief Binds the image for rendering.
		 */
		void Bind();

		/**	@brief Returns the Color value of a pixel.
		 */
		Color GetPixel(unsigned int x, unsigned int y) const;

		/**	@brief Returns the size of the image.
		 */
		Vector2<int> GetSize() const;

		/**	@brief Saves the image to a file.
		 */
		bool SaveToFile(const char *file) const;
	};
}

#endif
