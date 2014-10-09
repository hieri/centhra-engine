//- Centhra Engine -
#include <CE/UI/Control.h>
#include <CE/Base.h>

//- Standard Library -
#include <algorithm>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		Control::Control(Vector2<short> position, Vector2<short> extent)
		{
			m_type = 0;
			m_isVisible = true;
			m_parent = 0;
			m_position = position;
			m_extent = extent;
			m_children = vector<Control *>();
			UpdatePosition();
		}
		Control::~Control()
		{
			if(m_parent)
				m_parent->Remove(this);
			while(m_children.size())
				delete m_children.back();
		}
		void Control::UpdatePosition()
		{
			m_absolutePosition = m_position;
			if(m_parent)
				m_absolutePosition += m_parent->m_absolutePosition;
			
			short cx = m_position[0];
			short cy = m_position[1];
			short cw = m_extent[0];
			short ch = m_extent[1];
			
			if(m_parent)
			{
				cx = m_absolutePosition[0];
				cy = m_absolutePosition[1];

				if(cx > (m_parent->m_exposurePosition[0] + m_parent->m_exposureExtent[0]))
					cx = m_parent->m_exposurePosition[0] + m_parent->m_exposureExtent[0];
				if(cy > (m_parent->m_exposurePosition[1] + m_parent->m_exposureExtent[1]))
					cy = m_parent->m_exposurePosition[1] + m_parent->m_exposureExtent[1];
				if(cx < m_parent->m_exposurePosition[0])
					cx = m_parent->m_exposurePosition[0];
				if(cy < m_parent->m_exposurePosition[1])
					cy = m_parent->m_exposurePosition[1];
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
				cw = m_parent->m_exposurePosition[0] + m_parent->m_exposureExtent[0] - cx;
				ch = m_parent->m_exposurePosition[1] + m_parent->m_exposureExtent[1] - cy;
				
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

			m_exposurePosition = Vector2<short>(cx, cy);
			m_exposureExtent = Vector2<short>(cw, ch);
			
			for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->UpdatePosition();
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
						control->UpdatePosition();
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
		bool Control::IsVisible() const
		{
			return m_isVisible;
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
					m_children.erase(it);
					control->UpdatePosition();
				}
			}
		}
		void Control::Render()
		{
			bool noParent = !m_parent;
			if(noParent)
				glEnable(GL_SCISSOR_TEST);
			if(m_isVisible)
			{
				glPushMatrix();
				glScissor(m_exposurePosition[0], m_exposurePosition[1], m_exposureExtent[0], m_exposureExtent[1]);
				glTranslatef((float)m_position[0], (float)m_position[1], 0);
				DoRender();
				for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
					(*it)->Render();
				glPopMatrix();
			}
			if(noParent)
				glDisable(GL_SCISSOR_TEST);
		}
		void Control::DoRender()
		{
		}
		void Control::SetVisible(bool isVisible)
		{
			m_isVisible = isVisible;
		}
		Vector2<short> Control::GetExtent() const
		{
			return m_extent;
		}
		Vector2<short> Control::GetPosition() const
		{
			return m_position;
		}
		Vector2<short> Control::GetAbsolutePosition() const
		{
			return m_absolutePosition;
		}
		Vector2<short> Control::GetExposurePosition() const
		{
			return m_exposurePosition;
		}
		Vector2<short> Control::GetExposureExtent() const
		{
			return m_exposureExtent;
		}
		void Control::SetExtent(Vector2<short> extent)
		{
			m_extent = extent;
			UpdatePosition();
			OnSetExtent();
		}
		void Control::SetPosition(Vector2<short> position)
		{
			m_position = position;
			UpdatePosition();
			OnSetPosition();
		}
		bool Control::OnEvent(Event &event)
		{
			return true;
		}
		void Control::OnSetExtent()
		{
		}
		void Control::OnSetPosition()
		{
		}
		Control *Control::GetFromPosition(Vector2<short> position)
		{
			for(vector<Control *>::reverse_iterator it = m_children.rbegin(); it != m_children.rend(); it++)
			{
				Control *ctrl = *it;
				
				Vector2<short> expPos = ctrl->GetExposurePosition();
				Vector2<short> expExt = ctrl->GetExposureExtent();
				if(position[0] < expPos[0] || position[1] < expPos[1] || position[0] > (expPos[0] + expExt[0]) || position[1] > (expPos[1] + expExt[1]))
					continue;

				return ctrl->GetFromPosition(position);
			}
			return this;
		}
	}
}
