#ifndef _CE_UI_CONTROL_H_
#define _CE_UI_CONTROL_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/ConfigUI.h>
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
			unsigned int m_type;
			Control *m_parent;
			std::vector<Control *> m_children;
			Vector2<int_canvas> m_position, m_extent, m_absolutePosition, m_exposurePosition, m_exposureExtent;

			void UpdatePosition();
			virtual void DoRender();

		public:
			Control(Vector2<int_canvas> position, Vector2<int_canvas> extent);
			~Control();

			void Add(Control *control);
			void Remove(Control *control);
			bool IsAncestor(Control *control) const;
			bool IsMember(Control *control) const;
			bool IsVisible() const;
			bool Contains(Control *control);
			Control *GetParent() const;

			Control *GetFromPosition(Vector2<int_canvas> position, bool recurse = false);

			void Render();

			void SetVisible(bool isVisible);

			Vector2<int_canvas> GetPosition() const;
			Vector2<int_canvas> GetExtent() const;
			Vector2<int_canvas> GetAbsolutePosition() const;
			Vector2<int_canvas> GetExposurePosition() const;
			Vector2<int_canvas> GetExposureExtent() const;
			void SetPosition(Vector2<int_canvas> position);
			void SetExtent(Vector2<int_canvas> extent);

			virtual bool OnEvent(Event &event);
			virtual void OnSetPosition();
			virtual void OnSetExtent();
		};
	}
}

#endif
