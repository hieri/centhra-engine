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
			m_type(Type_Control), m_parent(0), m_eventMask(0), m_activeControlZone(0), m_hoverControlZone(0),
			m_isVisible(true), m_isUpdatingDimensions(true), m_acceptsFocus(false),
			m_isFocused(false), m_hasOverlay(false), m_hasControlZones(false),
			m_anchor(Anchor_None), m_isAnchorValid(false), m_scaling(Scaling_None)
		{
			m_position = position;
			m_extent = extent;
			UpdateDimensions();
			UpdateRelativeMatrix();
		}
		Control::~Control()
		{
			ClearEventCapture();
			if(m_parent)
				m_parent->Remove(this);
			DeleteChildren();
		}
		unsigned short Control::GetType() const
		{
			return m_type;
		}
		//- Dimension Update -
		void Control::UpdateDimensions()
		{
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

				UpdateRelativeMatrix(false);
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

			UpdateAbsoluteMatrix();

			OnDimensionUpdate();

			if(m_children.empty() == false)
			{
				Control **markControls = &m_children.front();
				Control **endControls = markControls + m_children.size();
				while(markControls != endControls)
					(*markControls++)->UpdateDimensions();
			}
		}
		void Control::OnDimensionUpdate()
		{
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
			if(m_children.empty() == false)
			{
				Control **markControls = &m_children.front();
				Control **endControls = markControls + m_children.size();
				while(markControls != endControls)
					if((*markControls++)->Contains(control))
						return true;
			}
			return false;
		}
		bool Control::Contains(Vector2<int_canvas> position)
		{
			if(!m_isVisible)
				return false;
			Vector2<int_canvas> exposureEnd = m_exposedPosition + m_exposedExtent;
			return !(position[0] < m_exposedPosition[0] || position[1] < m_exposedPosition[1] || position[0] > exposureEnd[0] || position[1] > exposureEnd[1]);
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
		void Control::DeleteChildren()
		{
			while(m_children.size())
				delete m_children.front();
		}
		//- Rendering -
		void Control::UpdateRelativeMatrix(bool updateAbsolute)
		{
			m_relativeMatrix = Matrix4x4<float>::BuildFromTranslation(m_position);
			if(updateAbsolute)
				UpdateAbsoluteMatrix();
		}
		void Control::UpdateAbsoluteMatrix()
		{
			if(m_parent)
				m_absoluteMatrix = (m_parent->m_absoluteMatrix * Matrix4x4<float>::BuildFromTranslation(m_parent->m_childOffset)) * m_relativeMatrix;
			else
				m_absoluteMatrix = m_relativeMatrix;
			if(m_children.empty() == false)
			{
				Control **markControls = &m_children.front();
				Control **endControls = markControls + m_children.size();
				while(markControls != endControls)
					(*markControls++)->UpdateAbsoluteMatrix();
			}
		}
		void Control::Render(RenderContext &context)
		{
			bool noParent = !m_parent;
			if(context.isCanvas && noParent)
				glEnable(GL_SCISSOR_TEST);
			if(m_isVisible)
			{
				if(context.isCanvas)
					glScissor(m_exposedPosition[0], context.height - m_exposedPosition[1] - m_exposedExtent[1], m_exposedExtent[0], m_exposedExtent[1]);
				DoRender(context);
				if(m_children.empty() == false)
				{
					Control **markControls = &m_children.front();
					Control **endControls = markControls + m_children.size();
					while(markControls != endControls)
						(*markControls++)->Render(context);
				}
				if(m_hasOverlay)
				{
					if(context.isCanvas)
						glScissor(m_exposedPosition[0], context.height - m_exposedPosition[1] - m_exposedExtent[1], m_exposedExtent[0], m_exposedExtent[1]);
					DoOverlay();
				}
			}
			if(context.isCanvas && noParent)
				glDisable(GL_SCISSOR_TEST);
		}
		void Control::Render(Canvas *canvas)
		{
			RenderContext context;
			context.width = canvas->GetWidth();
			context.height = canvas->GetHeight();
			context.isCanvas = true;
			//TODO: Move this config to the app implementation
			context.useShaders = false;
			context.projectionMatrix = canvas->GetProjectionMatrix();
			context.modelViewMatrix = Matrix4x4<float>();
			//TODO: Use projection matrix as default for mvpMatrix
			context.mvpMatrix = Matrix4x4<float>();
			if(context.useShaders == false)
				glEnableClientState(GL_VERTEX_ARRAY);
			Render(context);
			if(context.useShaders == false)
				glDisableClientState(GL_VERTEX_ARRAY);
		}
		void Control::DoRender(RenderContext &context)
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
			UpdateRelativeMatrix();
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
			if(m_children.empty() == false)
			{
				Control **markControls = &m_children.back();
				Control **endControls = markControls - m_children.size();
				while(markControls != endControls)
				{
					Control *ctrl = *markControls--;
					Vector2<int_canvas> expPos = ctrl->GetExposurePosition();
					Vector2<int_canvas> expExt = ctrl->GetExposureExtent();
					if(position[0] < expPos[0] || position[1] < expPos[1] || position[0] > (expPos[0] + expExt[0]) || position[1] > (expPos[1] + expExt[1]))
						continue;
					if(recurse)
						return ctrl->GetFromPosition(position, recurse);
					return ctrl;
				}
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
		map<unsigned char, deque<Control *> > Control::ms_eventCaptures;
		void Control::CaptureEvent(unsigned char eventId)
		{
			deque<unsigned char>::iterator itA = find(m_eventCapture.begin(), m_eventCapture.end(), eventId);
			if(itA == m_eventCapture.end())
			{
				m_eventCapture.push_back(eventId);
				if(ms_eventCaptures.count(eventId))
				{
					deque<Control *>::iterator itB = find(ms_eventCaptures[eventId].begin(), ms_eventCaptures[eventId].end(), this);
					if(itB != ms_eventCaptures[eventId].end())
						return;
				}
				ms_eventCaptures[eventId].push_back(this);
			}
		}
		void Control::ReleaseEvent(unsigned char eventId)
		{
			deque<unsigned char>::iterator itA = find(m_eventCapture.begin(), m_eventCapture.end(), eventId);
			if(itA != m_eventCapture.end())
			{
				if(ms_eventCaptures.count(eventId))
				{
					deque<Control *>::iterator itB = find(ms_eventCaptures[eventId].begin(), ms_eventCaptures[eventId].end(), this);
					if(itB != ms_eventCaptures[eventId].end())
						ms_eventCaptures[eventId].erase(itB);
				}
				m_eventCapture.erase(itA);
			}
		}
		void Control::ClearEventCapture()
		{
			while(m_eventCapture.size())
				ReleaseEvent(*m_eventCapture.begin());
		}
		unsigned short Control::GetEventMask() const
		{
			return m_eventMask;
		}
		void Control::SetEventMask(unsigned short mask)
		{
			m_eventMask = mask;
		}
		unsigned short g_childEventTransferMask = event::Mask_MouseButtonDown | event::Mask_MouseButtonUp | event::Mask_MouseScroll | event::Mask_MouseMotion; //TODO: Use mouse motion
		unsigned short g_controlZoneEventMask = event::Mask_MouseButtonDown | event::Mask_MouseButtonUp | event::Mask_MouseMotion;
		bool Control::ProcessEvent(Event &event)
		{
			//- Process Event Capture w/ Root Element -
			if(!m_parent)
				if(ms_eventCaptures.count(event.type))
					for(deque<Control *>::iterator it = ms_eventCaptures[event.type].begin(); it != ms_eventCaptures[event.type].end(); it++)
					{
						if(!(*it)->ProcessEvent(event))
							return false;
					}

			//- Handle the event for its children first -
			if(m_children.size())
				if(event.base.mask & g_childEventTransferMask)
				{
					Vector2<int_canvas> position(event.mouse.x, event.mouse.y);
					if(m_children.empty() == false)
					{
						Control **markControls = &m_children.front();
						Control **endControls = markControls + m_children.size();
						while(markControls != endControls)
						{
							Control *ctrl = *markControls++;
							if(!ctrl->IsVisible())
								continue;
							Vector2<int_canvas> expPos = ctrl->GetExposurePosition();
							Vector2<int_canvas> expExt = ctrl->GetExposureExtent();
							if(position[0] < expPos[0] || position[1] < expPos[1] || position[0] > (expPos[0] + expExt[0]) || position[1] > (expPos[1] + expExt[1]))
								continue;
							bool ret = ctrl->ProcessEvent(event);
							if(!ret)
								return false;
						}
					}
				}

			if(m_hasControlZones)
				if(event.base.mask & g_controlZoneEventMask)
				{
					if(m_activeControlZone)
					{
						if(event.type == event::MouseButtonUp)
						{
							if(event.mouseButton.button == event::MouseButtonLeft)
							{
								m_activeControlZone = 0;
								ReleaseEvent(event::MouseButtonUp);
								ReleaseEvent(event::MouseMotion);
								return false;
							}
						}
						else if(event.type == event::MouseMotion && m_activeControlZone->canMove)
						{
							Vector2<int_canvas> position(event.mouse.x, event.mouse.y);
							position -= m_absolutePosition;
							position[0] += m_activeControlZone->dragOffsetX;
							position[1] += m_activeControlZone->dragOffsetY;

							if(position[0] < m_activeControlZone->minX)
								position[0] = m_activeControlZone->minX;
							if(position[1] < m_activeControlZone->minY)
								position[1] = m_activeControlZone->minY;

							//TODO: Include this in the control zone definition
							int_canvas maxX = m_activeControlZone->maxX - m_activeControlZone->width;
							int_canvas maxY = m_activeControlZone->maxY - m_activeControlZone->height;
							if(position[0] > maxX)
								position[0] = maxX;
							if(position[1] > maxY)
								position[1] = maxY;

							m_activeControlZone->x = position[0];
							m_activeControlZone->y = position[1];
							OnControlZoneMove(m_activeControlZone);
							return false;
						}
					}
					else if(event.type == event::MouseButtonDown)
					{
						if(event.mouseButton.button == event::MouseButtonLeft)
						{
							Vector2<int_canvas> position(event.mouse.x, event.mouse.y);
							position -= m_absolutePosition;
							ControlZone *zone = GetControlZoneFromPosition(position);
							if(zone)
							{
								zone->dragOffsetX = zone->x - position[0];
								zone->dragOffsetY = zone->y - position[1];
								m_activeControlZone = zone;
								CaptureEvent(event::MouseButtonUp);
								if(zone->canMove)
									CaptureEvent(event::MouseMotion);
								OnControlZoneSelect(zone);
								return false;
							}
						}
					}
					else if(event.type == event::MouseMotion)
					{
						Vector2<int_canvas> position(event.mouse.x, event.mouse.y);
						position -= m_absolutePosition;
						ControlZone *zone = GetControlZoneFromPosition(position);

						if(m_hoverControlZone != zone)
						{
							if(m_hoverControlZone)
								OnControlZoneHoverLost(m_hoverControlZone);
							m_hoverControlZone = zone;
							if(zone)
								OnControlZoneHover(zone);
						}
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
			if(m_children.empty() == false)
			{
				Control **markControls = &m_children.back();
				Control **endControls = markControls - m_children.size();
				while(markControls != endControls)
				{
					Control *ctrl = *markControls--;
					Vector2<int_canvas> expPos = ctrl->GetExposurePosition();
					Vector2<int_canvas> expExt = ctrl->GetExposureExtent();
					if(position[0] < expPos[0] || position[1] < expPos[1] || position[0] > (expPos[0] + expExt[0]) || position[1] > (expPos[1] + expExt[1]))
						continue;
					return ctrl->GetFocusFromPosition(position);
				}
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

		//- Control Zones -
		Control::ControlZone *Control::GetControlZoneFromPosition(Vector2<int_canvas> position)
		{
			if(m_controlZones.empty() == false)
			{
				ControlZone *markControlZones = &m_controlZones.front();
				ControlZone *endControlZones = markControlZones + m_controlZones.size();
				while(markControlZones != endControlZones)
				{
					if(markControlZones->x > position[0] || (markControlZones->x + markControlZones->width) < position[0] || markControlZones->y > position[1] || (markControlZones->y + markControlZones->height) < position[1])
					{
						markControlZones++;
						continue;
					}
					return &(*markControlZones);
				}
			}
			return 0;
		}
		void Control::OnControlZoneMove(ControlZone *zone)
		{
		}
		void Control::OnControlZoneSelect(ControlZone *zone)
		{
		}
		void Control::OnControlZoneHover(ControlZone *zone)
		{
		}
		void Control::OnControlZoneHoverLost(ControlZone *zone)
		{
		}
	}
}
