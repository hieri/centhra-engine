#ifndef _CE_UI_CONTROL_H_
#define _CE_UI_CONTROL_H_

//- Standard Library -
#include <vector>
#include <deque>

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
		private:
			static std::deque<Control *> ms_focusCtrls;

		protected:
			static Control *ms_currentFocus;
			static void AddToFocusCtrls(Control *ctrl);
			static void RemoveFromFocusCtrls(Control *ctrl);

			bool m_acceptsFocus, m_isFocused;
			void OnFocus();
			void OnFocusLost();
			Control *GetFocusFromPosition(Vector2<int_canvas> position);
		public:
			static Control *GetCurrentFocus();
			bool IsFocused() const;
			void Focus();
			void SetFocusByPosition(Vector2<int_canvas> position);
			Control *FindNextFocus(Control *reference = 0, bool fromParent = false);
			Control *FindPreviousFocus(Control *reference = 0, bool fromParent = false);

			//- Anchor -
		protected:
			unsigned char m_anchor;
			bool m_isAnchorValid;
			Vector2<int_canvas> m_anchorPos;
		public:
			typedef enum AnchorMask
			{
				Anchor_None,
				Anchor_Left = 1,
				Anchor_Right = 2,
				Anchor_Top = 4,
				Anchor_Bottom = 8
			};
			unsigned char GetAnchor() const;
			void SetAnchor(unsigned char anchor);

			//- Scaling -
		protected:
			unsigned char m_scaling;
			Vector2<int_canvas> m_scalingReference, m_scalingExtent;
		public:
			typedef enum ScalingMask
			{
				Scaling_None,
				Scaling_Horizontal = 1,
				Scaling_Vertical = 2
			};
			void ResetScaling();
			unsigned char GetScaling() const;
			void SetScaling(unsigned char scaling);

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
			Control *GetRoot();

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

			virtual void OnAdded(Control *parent);
			virtual void OnRemoved(Control *parent);
		};
	}
}

#endif
