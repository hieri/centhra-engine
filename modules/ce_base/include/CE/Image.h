#ifndef _CE_BASE_IMAGE_H_
#define _CE_BASE_IMAGE_H_

//- Centhra Engine -
#include <CE/Color.h>
#include <CE/File.h>
#include <CE/Vector2.h>

namespace ce
{
	/**	@brief Image Data Structure Class
	 */
	class Image : public File::FileObject
	{
		unsigned char m_bytesPerPixel;
		unsigned int m_glTexture, m_ilImage;
		Vector2<unsigned int> m_size;

		Image();

		virtual void OnFileChange();
		bool LoadFromFile(const char *file);

	public:
		/**	@brief Initializes the registered image loaders.
		 *	@return Whether or not the image loader initialization succeeded.
		 */
		static bool Init();
		static void Cleanup();

		/**	@brief File Constructor.
		 *	@param file Path to image file.
		 *	@return Image object or 0 if creation failed.
		 */
		static Image *CreateFromFile(const char *file);

		/**	@brief Memory Constructor.
		 *	@param memory Address of image data.
		 *	@param size Size of image data.
		 *	@return Image object or 0 if creation failed.
		 */
		static Image *CreateFromMemory(const unsigned char *memory, unsigned int size);

		/**	@brief Pixel Array Constructor.
		 *	@param pixels Array of pixels.
		 *	@param bytesPerPixel Number of bytes per pixel.
		 *	@param size Size of the image.
		 *	@return Image object or 0 if creation failed.
		 */
		static Image *CreateFromPixels(char *pixels, unsigned char bytesPerPixel, Vector2<unsigned int> size);

		/**	@brief Screenshot Constructor.
		 *	@param start Start point of the screen.
		 *	@param end End point of the screen.
		 *	@return Image object or 0 if creation failed.
		 */
		static Image *CreateFromScreen(Vector2<unsigned int> start, Vector2<unsigned int> end);

		/**	@brief Destructor.
		 */
		~Image();

		/**	@brief Binds the image for rendering.
		 */
		void Bind();

		/**	@brief Returns the Color value of a pixel.
		 *	@param x X coordinate of the pixel.
		 *	@param y Y coordinate of the pixel.
		 *	@return Color value of the pixel.
		 */
		Color GetPixel(unsigned int x, unsigned int y) const;

		/**	@brief Returns the size of the image.
		 *	@return Size of the image.
		 */
		Vector2<unsigned int> GetSize() const;

		/**	@brief Saves the image to a file.
		 *	@param file Target path of file saving.
		 *	@return Whether or not the saving succeeded.
		 */
		bool SaveToFile(const char *file) const;

		inline unsigned int GetTextureID() const
		{
			return m_glTexture;
		}
	};
}

#endif
