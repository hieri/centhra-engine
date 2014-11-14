#ifndef _CE_UI_CONTROL_H_
#define _CE_UI_CONTROL_H_

//- Standard Library -
#include <vector>

//- Centhra Engine -
#include <CE/ConfigUI.h>
#include <CE/Vector2.h>
#include <CE/Event.h>
#include <CE/Canvas.h>

namespace ce
{
	namespace ui
	{
		class Control
		{
			//- Visibility -
		protected:
			bool m_isVisible;
		public:
			bool IsVisible() const;
			void SetVisible(bool isVisible);

			//- Dimension Update -
		protected:
			bool m_isUpdatingDimensions;
			void UpdateDimensions();

			//- Focus -
		protected:
			static Control *ms_currentFocus;
			bool m_acceptsFocus, m_isFocused;
			void OnFocus();
			void OnFocusLost();
			Control *GetFocusFromPosition(Vector2<int_canvas> position);
		public:
			static Control *GetCurrentFocus();
			bool IsFocused() const;
			void Focus();
			void SetFocusByPosition(Vector2<int_canvas> position);
			// Tabbed Traversal
//			Control *FindNextFocus();
//			Control *FindPreviousFocus();

		protected:
			unsigned int m_type;
			Control *m_parent;
			std::vector<Control *> m_children;
			Vector2<int_canvas> m_position, m_extent, m_absolutePosition, m_exposedPosition, m_exposedExtent;

			virtual void DoRender();

		public:
			Control(Vector2<int_canvas> position, Vector2<int_canvas> extent);
			virtual ~Control();

			void Add(Control *control);
			void Remove(Control *control);
			bool IsAncestor(Control *control) const;
			bool IsMember(Control *control) const;
			bool Contains(Control *control);
			Control *GetParent() const;

			Control *GetFromPosition(Vector2<int_canvas> position, bool recurse = false);


			typedef struct UIContext
			{
				int_canvas width, height;
				bool isCanvas;
			} UIContext;

			void Render(UIContext &context);
			void Render(Canvas *canvas);


			void SetUpdatingAbsolute(bool isUpdatingAbsolute);
			bool IsUpdatingAbsolute() const;

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
