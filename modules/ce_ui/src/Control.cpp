//- Standard Library -
#include <algorithm>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/UI/ScrollCtrl.h>
#include <CE/Canvas.h>
#include <CE/Base.h>

using namespace std;

//TODO: Implement child trim toggle

namespace ce
{
	namespace ui
	{
		Control::Control(Vector2<int_canvas> position, Vector2<int_canvas> extent) :
			m_type(Type_Control), m_parent(0), m_eventMask(0),
			m_isVisible(true), m_isUpdatingDimensions(true), m_acceptsFocus(false), m_isFocused(false), m_hasOverlay(false),
			m_anchor(Anchor_None), m_isAnchorValid(false), m_scaling(Scaling_None)
		{
			m_position = position;
			m_extent = extent;
			UpdateDimensions();
		}
		Control::~Control()
		{
			if(m_parent)
				m_parent->Remove(this);
			while(m_children.size())
				delete m_children.back();
		}
		unsigned short Control::GetType() const
		{
			return m_type;
		}
		//- Dimension Update -
		void Control::UpdateDimensions()
		{
			//TODO: Handle scroll offsets
			//- Anchor -
			if(!m_isAnchorValid)
			{
				if(m_parent)
				{
					Vector2<int_canvas> extent = m_parent->GetExtent();

					if(m_anchor & Anchor_Left)
						m_anchorPos[0] = m_position[0];
					if(m_anchor & Anchor_Right)
						m_anchorPos[0] = extent[0] - m_position[0];

					if(m_anchor & Anchor_Top)
						m_anchorPos[1] = m_position[1];
					if(m_anchor & Anchor_Bottom)
						m_anchorPos[1] = extent[1] - m_position[1];

					//TODO: Handle center
				}
				else
				{
					//TODO: Handle root element resizing/anchor?
				}
				m_isAnchorValid = true;
			}
			if(m_anchor && m_parent)
			{
				Vector2<int_canvas> parentExtent = m_parent->GetExtent();

				if(m_anchor & Anchor_Left)
					m_position[0] = m_anchorPos[0];
				if(m_anchor & Anchor_Right)
					m_position[0] = parentExtent[0] - m_anchorPos[0];

				if(m_anchor & Anchor_Top)
					m_position[1] = m_anchorPos[1];
				if(m_anchor & Anchor_Bottom)
					m_position[1] = parentExtent[1] - m_anchorPos[1];
			}

			//- Scaling -
			if(m_scaling && m_parent)
			{
				Vector2<int_canvas> parentExtent = m_parent->GetExtent();
				float xScale = (float)parentExtent[0] / m_scalingReference[0];
				float yScale = (float)parentExtent[1] / m_scalingReference[1];

				if(m_scaling & Scaling_Horizontal)
					m_extent[0] = (int_canvas)(xScale * m_scalingExtent[0]);
				if(m_scaling & Scaling_Vertical)
					m_extent[1] = (int_canvas)(yScale * m_scalingExtent[1]);
			}

			//- Standard -
			if(m_parent)
				m_absolutePosition = m_position + m_parent->m_absolutePosition + m_parent->m_childOffset;
			else
				m_absolutePosition = m_position;
			
			int_canvas cx = m_position[0];
			int_canvas cy = m_position[1];
			int_canvas cw = m_extent[0];
			int_canvas ch = m_extent[1];
			
			if(m_parent)
			{
				cx = m_absolutePosition[0];
				cy = m_absolutePosition[1];

				if(cx > (m_parent->m_exposedPosition[0] + m_parent->m_exposedExtent[0]))
					cx = m_parent->m_exposedPosition[0] + m_parent->m_exposedExtent[0];
				if(cy > (m_parent->m_exposedPosition[1] + m_parent->m_exposedExtent[1]))
					cy = m_parent->m_exposedPosition[1] + m_parent->m_exposedExtent[1];
				if(cx < m_parent->m_exposedPosition[0])
					cx = m_parent->m_exposedPosition[0];
				if(cy < m_parent->m_exposedPosition[1])
					cy = m_parent->m_exposedPosition[1];
			}
			
		//	if(cx > game::windowWidth)
		//		cx = game::windowWidth;
		//	if(cy > game::windowHeight)
		//		cy = game::windowWidth;
			if(cx < 0)
				cx = 0;
			if(cy < 0)
				cy = 0;
			
			if(m_parent)
			{
				cw = m_parent->m_exposedPosition[0] + m_parent->m_exposedExtent[0] - cx;
				ch = m_parent->m_exposedPosition[1] + m_parent->m_exposedExtent[1] - cy;
				
				if(cw > m_extent[0])
					cw = m_extent[0];
				if(ch > m_extent[1])
					ch = m_extent[1];
				if((cx + cw) > (m_absolutePosition[0] + m_extent[0]))
					cw = m_absolutePosition[0] + m_extent[0] - cx;
				if((cy + ch) > (m_absolutePosition[1] + m_extent[1]))
					ch = m_absolutePosition[1] + m_extent[1] - cy;
			}
			else
			{
				if(m_position[0] < 0)
					cx += m_position[0];
				if(m_position[1] < 0)
					cy += m_position[1];
			}
		//	if((cx + _cw) > game::windowWidth)
		//		cx = game::windowWidth - _cw;
		//	if((cy + _ch) > game::windowHeight)
		//		cy = game::windowHeight - _ch;
			if(cx < 0)
				cx = 0;
			if(cy < 0)
				cy = 0;
			if(cx < 0)
				cx = 0;
			if(cy < 0)
				cy = 0;

			m_exposedPosition = Vector2<int_canvas>(cx, cy);
			m_exposedExtent = Vector2<int_canvas>(cw, ch);

			for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->UpdateDimensions();
		}
		void Control::Add(Control *control)
		{
			if(this != control)
				if(!IsMember(control))
					if(!IsAncestor(control))
					{
						if(control->m_parent)
							control->m_parent->Remove(control);
						m_children.push_back(control);
						control->m_parent = this;
						control->m_isAnchorValid = false;
						control->ResetScaling();
						if(control->m_isUpdatingDimensions)
							control->UpdateDimensions();
						control->OnAdded(this);
						OnMemberAdded(control);
					}
		}
		bool Control::Contains(Control *control)
		{
			if(this == control)
				return false;
			if(IsMember(control))
				return true;
			for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				if((*it)->Contains(control))
					return true;
			return false;
		}
		Control *Control::GetParent() const
		{
			return m_parent;
		}
		Control *Control::GetRoot()
		{
			if(m_parent)
				return m_parent->GetRoot();
			return this;
		}
		bool Control::IsAncestor(Control *control) const
		{
			if(this == control)
				return false;
			if(m_parent == 0)
				return false;
			if(m_parent == control)
				return true;
			return m_parent->IsAncestor(control);
		}
		bool Control::IsMember(Control *control) const
		{
			if(this == control)
				return false;
			return find(m_children.begin(), m_children.end(), control) != m_children.end();
		}
		void Control::Remove(Control *control)
		{
			if(this != control)
			{
				vector<Control *>::iterator it = find(m_children.begin(), m_children.end(), control);
				if(it != m_children.end())
				{
					Control *control = *it;
					control->m_parent = 0;
					control->m_isAnchorValid = false;
					control->OnRemoved(this);
					m_children.erase(it);
					if(control->m_isUpdatingDimensions)
						control->UpdateDimensions();
					OnMemberRemoved(control);
				}
			}
		}
		void Control::Render(UIContext &context)
		{
			bool noParent = !m_parent;
			if(context.isCanvas && noParent)
				glEnable(GL_SCISSOR_TEST);
			if(m_isVisible)
			{
				glPushMatrix();
				if(context.isCanvas)
					glScissor(m_exposedPosition[0], context.height - m_exposedPosition[1] - m_exposedExtent[1], m_exposedExtent[0], m_exposedExtent[1]);
				glTranslatef((float)m_position[0], (float)m_position[1], 0);
				DoRender();
				glPushMatrix();
					glTranslatef((float)m_childOffset[0], (float)m_childOffset[1], 0);
					for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
						(*it)->Render(context);
				glPopMatrix();
				if(m_hasOverlay)
				{
					if(context.isCanvas)
						glScissor(m_exposedPosition[0], context.height - m_exposedPosition[1] - m_exposedExtent[1], m_exposedExtent[0], m_exposedExtent[1]);
					DoOverlay();
				}
				glPopMatrix();
			}
			if(context.isCanvas && noParent)
				glDisable(GL_SCISSOR_TEST);
		}
		void Control::Render(Canvas *canvas)
		{
			UIContext context;
			context.width = canvas->GetWidth();
			context.height = canvas->GetHeight();
			context.isCanvas = true;
			
			glPushMatrix();
			glTranslatef(0.f, (float)context.height, 0.f);
			glScalef(1.f, -1.f, 1.f);
			Render(context);
			glPopMatrix();
		}
		void Control::DoRender()
		{
		}
		void Control::DoOverlay()
		{
		}
		//- Visibility -
		bool Control::IsVisible() const
		{
			return m_isVisible;
		}
		void Control::SetVisible(bool isVisible)
		{
			m_isVisible = isVisible;
		}
		Vector2<int_canvas> Control::GetExtent() const
		{
			return m_extent;
		}
		Vector2<int_canvas> Control::GetPosition() const
		{
			return m_position;
		}
		Vector2<int_canvas> Control::GetAbsolutePosition() const
		{
			return m_absolutePosition;
		}
		Vector2<int_canvas> Control::GetExposurePosition() const
		{
			return m_exposedPosition;
		}
		Vector2<int_canvas> Control::GetExposureExtent() const
		{
			return m_exposedExtent;
		}
		Vector2<int_canvas> Control::GetChildOffset() const
		{
			return m_childOffset;
		}
		void Control::SetExtent(Vector2<int_canvas> extent)
		{
			m_extent = extent;
			if(m_isUpdatingDimensions)
				UpdateDimensions();
			OnSetExtent();
			if(m_parent)
				if(m_parent->GetType() == Type_ScrollCtrl)
					((ScrollCtrl *)m_parent)->UpdateScroll();
		}
		void Control::SetPosition(Vector2<int_canvas> position)
		{
			m_position = position;
			m_isAnchorValid = false;
			if(m_isUpdatingDimensions)
				UpdateDimensions();
			OnSetPosition();
			if(m_parent)
				if(m_parent->GetType() == Type_ScrollCtrl)
					((ScrollCtrl *)m_parent)->UpdateScroll();
		}
		void Control::OnSetExtent()
		{
		}
		void Control::OnSetPosition()
		{
		}
		void Control::OnAdded(Control *parent)
		{
		}
		void Control::OnRemoved(Control *parent)
		{
		}
		void Control::OnMemberAdded(Control *ctrl)
		{
		}
		void Control::OnMemberRemoved(Control *ctrl)
		{
		}
		Control *Control::GetFromPosition(Vector2<int_canvas> position, bool recurse)
		{
			for(vector<Control *>::reverse_iterator it = m_children.rbegin(); it != m_children.rend(); it++)
			{
				Control *ctrl = *it;
				
				Vector2<int_canvas> expPos = ctrl->GetExposurePosition();
				Vector2<int_canvas> expExt = ctrl->GetExposureExtent();
				if(position[0] < expPos[0] || position[1] < expPos[1] || position[0] > (expPos[0] + expExt[0]) || position[1] > (expPos[1] + expExt[1]))
					continue;

				if(recurse)
					return ctrl->GetFromPosition(position, recurse);
				return ctrl;
			}
			return this;
		}
		void Control::SetUpdatingAbsolute(bool isUpdatingAbsolute)
		{
			m_isUpdatingDimensions = isUpdatingAbsolute;
		}
		bool Control::IsUpdatingAbsolute() const
		{
			return m_isUpdatingDimensions;
		}

		//- Event Handling -
		unsigned short Control::GetEventMask() const
		{
			return m_eventMask;
		}
		void Control::SetEventMask(unsigned short mask)
		{
			m_eventMask = mask;
		}
		unsigned short g_childEventTransfer = event::Mask_MouseButtonDown | event::Mask_MouseButtonUp | event::Mask_MouseScroll; //TODO: Use mouse motion
		bool Control::ProcessEvent(Event &event)
		{
			//- Handle the event for its children first -
			if(m_children.size())
				if(event.base.mask & g_childEventTransfer)
				{
					Vector2<int_canvas> position(event.mouse.x, event.mouse.y);
					for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
					{
						Control *ctrl = *it;
						Vector2<int_canvas> expPos = ctrl->GetExposurePosition();
						Vector2<int_canvas> expExt = ctrl->GetExposureExtent();
						if(position[0] < expPos[0] || position[1] < expPos[1] || position[0] > (expPos[0] + expExt[0]) || position[1] > (expPos[1] + expExt[1]))
							continue;
						bool ret = ctrl->ProcessEvent(event);
						if(!ret)
							return false;
					}
				}

			//- Handle the event if the event is included in the event mask -
			if(event.base.mask & m_eventMask)
				return OnEvent(event);

			return true;
		}
		bool Control::OnEvent(Event &event)
		{
			return true;
		}

		//- Focus -
		//TODO: Implement as always sorted static deque
		deque<Control *> Control::ms_focusCtrls;
		void Control::AddToFocusCtrls(Control *ctrl)
		{
			deque<Control *>::iterator it = find(ms_focusCtrls.begin(), ms_focusCtrls.end(), ctrl);
			if(it == ms_focusCtrls.end())
				ms_focusCtrls.push_back(ctrl);
		}
		void Control::RemoveFromFocusCtrls(Control *ctrl)
		{
			deque<Control *>::iterator it = find(ms_focusCtrls.begin(), ms_focusCtrls.end(), ctrl);
			if(it != ms_focusCtrls.end())
				ms_focusCtrls.erase(it);
		}

		Control *Control::ms_currentFocus = 0;
		Control *Control::GetCurrentFocus()
		{
			return ms_currentFocus;
		}
		void Control::Focus()
		{
			if(ms_currentFocus == this)
				return;
			if(ms_currentFocus)
			{
				ms_currentFocus->m_isFocused = false;
				ms_currentFocus->OnFocusLost();
			}
			m_isFocused = true;
			ms_currentFocus = this;
			OnFocus();
		}
		bool Control::IsFocused() const
		{
			return m_isFocused;
		}
		void Control::OnFocus()
		{
		}
		void Control::OnFocusLost()
		{
		}
		bool sortByYThenX(Control *A, Control *B)
		{
			Vector2<int_canvas> aPos = A->GetExposurePosition();
			Vector2<int_canvas> bPos = B->GetExposurePosition();
			if(bPos[1] < aPos[1])
				return false;
			else if(bPos[1] > aPos[1])
				return true;
			return bPos[0] > aPos[0];
		}
		bool sortByRYThenRX(Control *A, Control *B)
		{
			Vector2<int_canvas> aPos = A->GetExposurePosition();
			Vector2<int_canvas> bPos = B->GetExposurePosition();
			if(bPos[1] > aPos[1])
				return false;
			else if(bPos[1] < aPos[1])
				return true;
			return bPos[0] < aPos[0];
		}
		Control *Control::FindNextFocus(Control *reference, bool fromParent)
		{
			deque<Control *> ordered(ms_focusCtrls);
			sort(ordered.begin(), ordered.end(), sortByYThenX);

			deque<Control *>::iterator it = find(ordered.begin(), ordered.end(), ms_currentFocus);

			if(it == ordered.end())
				it = ordered.begin();
			else
				it++;
			if(it != ordered.end())
				while(!(*it)->IsVisible())
				{
					it++;
					if(it == ordered.end())
						break;
				}

			if(it == ordered.end())
			{
				it = ordered.begin();
				while(!(*it)->IsVisible())
				{
					it++;
					if(it == ordered.end())
						break;
				}
			}

			if(it == ordered.end())
				return 0;
			if((*it)->IsVisible())
				return *it;
			return 0;
		}
		Control *Control::FindPreviousFocus(Control *reference, bool fromParent)
		{
			deque<Control *> ordered(ms_focusCtrls);
			sort(ordered.begin(), ordered.end(), sortByRYThenRX);

			deque<Control *>::iterator it = find(ordered.begin(), ordered.end(), ms_currentFocus);

			if(it == ordered.end())
				it = ordered.begin();
			else
				it++;
			if(it != ordered.end())
				while(!(*it)->IsVisible())
				{
					it++;
					if(it == ordered.end())
						break;
				}

			if(it == ordered.end())
			{
				it = ordered.begin();
				while(!(*it)->IsVisible())
				{
					it++;
					if(it == ordered.end())
						break;
				}
			}

			if(it == ordered.end())
				return 0;
			if((*it)->IsVisible())
				return *it;
			return 0;
		}
		Control *Control::GetFocusFromPosition(Vector2<int_canvas> position)
		{
			for(vector<Control *>::reverse_iterator it = m_children.rbegin(); it != m_children.rend(); it++)
			{
				Control *ctrl = *it;

				Vector2<int_canvas> expPos = ctrl->GetExposurePosition();
				Vector2<int_canvas> expExt = ctrl->GetExposureExtent();
				if(position[0] < expPos[0] || position[1] < expPos[1] || position[0] > (expPos[0] + expExt[0]) || position[1] > (expPos[1] + expExt[1]))
					continue;
				return ctrl->GetFocusFromPosition(position);
			}
			if(m_acceptsFocus)
				return this;
			return 0;
		}
		void Control::SetFocusByPosition(Vector2<int_canvas> position)
		{
			Control *ctrl = GetFocusFromPosition(position);
			if(ctrl)
				ctrl->Focus();
		}

		//- Anchor -
		unsigned char Control::GetAnchor() const
		{
			return m_anchor;
		}
		void Control::SetAnchor(unsigned char anchor)
		{
			m_anchor = anchor;
		}

		//- Scaling -
		unsigned char Control::GetScaling() const
		{
			return m_scaling;
		}
		void Control::SetScaling(unsigned char scaling)
		{
			m_scaling = scaling;
		}
		void Control::ResetScaling()
		{
			if(m_parent)
				m_scalingReference = m_parent->GetExtent();
			else
				m_scalingReference = Vector2<int_canvas>(1, 1);

			m_scalingExtent = m_extent;

			//TODO: Properly reset scaling extent
		}
	}
}
