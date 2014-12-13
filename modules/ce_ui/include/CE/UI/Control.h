#ifndef _CE_UI_CONTROL_H_
#define _CE_UI_CONTROL_H_

//- Standard Library -
#include <vector>
#include <deque>
#include <map>

//- Centhra Engine -
#include <CE/ConfigUI.h>
#include <CE/Vector2.h>
#include <CE/Event.h>
#include <CE/Canvas.h>

namespace ce
{
	namespace ui
	{
		//TODO: Split into masks for functionality
		typedef enum ControlType
		{
			Type_Control,
			Type_ColorCtrl,
			Type_ImageCtrl,
			Type_ButtonCtrl,
			Type_TextCtrl,
			Type_TextEditCtrl,
			Type_TextButtonCtrl,
			Type_ScrollCtrl
		} ControlType;

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
		public:
			void UpdateDimensions();

			//- Event Handling -
		protected:
			static std::map<unsigned char, std::deque<Control *> > ms_eventCaptures;
			std::deque<unsigned char> m_eventCapture;
			void CaptureEvent(unsigned char eventId);
			void ReleaseEvent(unsigned char eventId);
			void ClearEventCapture();
			unsigned short m_eventMask;
		public:
			unsigned short GetEventMask() const;
			void SetEventMask(unsigned short mask);
			bool ProcessEvent(Event &event);

			/**	@brief Callback for event handling.
			*	@return Whether or not the event is consumed.
			*/
			virtual bool OnEvent(Event &event);

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
			} AnchorMask;
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
			} ScalingMask;
			void ResetScaling();
			unsigned char GetScaling() const;
			void SetScaling(unsigned char scaling);

		protected:
			unsigned short m_type;
			Control *m_parent;
			std::vector<Control *> m_children;
			Vector2<int_canvas> m_position, m_extent, m_absolutePosition, m_exposedPosition, m_exposedExtent, m_childOffset;

			//- Render Under Children -
			virtual void DoRender();
			//- Render Over Children -
			bool m_hasOverlay;
			virtual void DoOverlay();

		public:
			Control(Vector2<int_canvas> position, Vector2<int_canvas> extent);
			virtual ~Control();

			unsigned short GetType() const;
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
			Vector2<int_canvas> GetChildOffset() const;
			void SetPosition(Vector2<int_canvas> position);
			void SetExtent(Vector2<int_canvas> extent);

			virtual void OnSetPosition();
			virtual void OnSetExtent();

			virtual void OnAdded(Control *parent);
			virtual void OnRemoved(Control *parent);

			virtual void OnMemberAdded(Control *ctrl);
			virtual void OnMemberRemoved(Control *ctrl);

			//- Control Zones -
		public:
			typedef struct ControlZone
			{
				unsigned char id;
				int_canvas x, y, width, height,
					minX, maxX, minY, maxY,
					dragOffsetX, dragOffsetY;
			} ControlZone;
		protected:
			bool m_hasControlZones;
			ControlZone *m_activeControlZone;
			std::vector<ControlZone> m_controlZones;
			ControlZone *GetControlZoneFromPosition(Vector2<int_canvas> position);
			virtual void OnControlZoneMove(ControlZone *zone);
		};
	}
}

#endif
