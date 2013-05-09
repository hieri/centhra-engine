#ifndef _CE_UI_CONTROL_H_
#define _CE_UI_CONTROL_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/Vector2.h>
#include <CE/Event.h>

namespace ce
{
	namespace ui
	{
		class Control
		{
		protected:
			bool m_isVisible;
			Control *m_parent;
			std::vector<Control *> m_children;
			Vector2<int> m_position, m_extent, m_absolutePosition, m_exposurePosition, m_exposureExtent;

			void UpdatePosition();
			virtual void DoRender();

		public:
			Control(Vector2<int> position, Vector2<int> extent);

			void Add(Control *control);
			void Remove(Control *control);
			bool IsAncestor(Control *control) const;
			bool IsMember(Control *control) const;
			bool IsVisible() const;
			bool Contains(Control *control);
			Control *GetParent() const;
			void Render();
			void SetVisible(bool isVisible);
			Vector2<int> GetPosition() const;
			Vector2<int> GetExtent() const;
			Vector2<int> GetAbsolutePosition() const;
			Vector2<int> GetExposurePosition() const;
			Vector2<int> GetExposureExtent() const;
			void SetPosition(Vector2<int> position);
			void SetExtent(Vector2<int> extent);
			virtual bool OnEvent(Event &event);
		};
	}
}

#endif
