#ifndef _CE_GAME2D_PHYSICALTEXT_H_
#define _CE_GAME2D_PHYSICALTEXT_H_

//- Centhra Engine -
#include <CE/Font.h>
#include <CE/Game2D/PhysicalObject.h>

//- Standard Library -
#include <string>

namespace ce
{
	namespace game2d
	{
		class PhysicalText : public PhysicalObject
		{
			std::string m_text;
			Font *m_font;

		protected:
			virtual void DoRender();

		public:
			PhysicalText(Vector2<float> position, Vector2<float> extent, Font *font, std::string text = "");

			void SetText(std::string text);
			void SetText(const char *format, ...);
			std::string GetText() const;
		};
	}
}

#endif
