#ifndef _CE_UI_CONTROL_H_
#define _CE_UI_CONTROL_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Vector2.h>

namespace ce
{
	namespace ui
	{
		class Control
		{
			bool m_isVisible;
			Control *m_parent;
			std::vector<Control *> m_children;
			Vector2<int> m_position, m_extent;

		protected:
			virtual void DoRender();

		public:
			Control();

			void Add(Control *control);
			void Remove(Control *control);
			bool IsAncestor(Control *control) const;
			bool IsMember(Control *control) const;
			bool IsVisible() const;
			bool Contains(Control *control);
			Control *GetParent() const;
			void Render();
			void SetVisible(bool isVisible);
		};
	}
}

#endif
