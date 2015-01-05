#ifndef _CE_GAME2D_PHYSICALTEXT_H_
#define _CE_GAME2D_PHYSICALTEXT_H_

//- Standard Library -
#include <string>

//- Centhra Engine -
#include <CE/Font.h>
#include <CE/Game2D/PhysicalObject.h>

namespace ce
{
	namespace game2d
	{
		class PhysicalText : public PhysicalObject
		{
			std::string m_text;
			Font *m_font;

		protected:
			virtual void DoRender(RenderContext &context);

		public:
			PhysicalText(Vector2<float> position, Vector2<float> extent, Font *font, std::string text = "");

			void SetText(std::string text);
			void SetText(const char *format, ...);
			std::string GetText() const;
		};
	}
}

#endif
