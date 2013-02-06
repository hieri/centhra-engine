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
			Control *m_parent;
			std::vector<Control *> m_children;
			Vector2<int> m_position, m_extent;

		public:
			Control();

			void Add(Control *control);
			void Remove(Control *control);
			bool IsAncestor(Control *control) const;
			bool IsMember(Control *control) const;
			bool Contains(Control *control);
			Control *GetParent() const;
		};
	}
}

#endif
