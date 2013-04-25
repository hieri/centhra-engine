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
		Control::Control(Vector2<int> position, Vector2<int> extent)
		{
			m_isVisible = true;
			m_parent = 0;
			m_position = position;
			m_extent = extent;
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
					(*it)->m_parent = 0;
					m_children.erase(it);
				}
			}
		}
		void Control::Render()
		{
			if(m_isVisible)
			{
				glPushMatrix();
				DoRender();
				for(vector<Control *>::iterator it = m_children.begin(); it != m_children.end(); it++)
					(*it)->Render();
				glPopMatrix();
			}
		}
		void Control::DoRender()
		{
		}
		void Control::SetVisible(bool isVisible)
		{
			m_isVisible = isVisible;
		}
		Vector2<int> Control::GetExtent() const
		{
			return m_extent;
		}
		Vector2<int> Control::GetPosition() const
		{
			return m_position;
		}
		void Control::SetExtent(Vector2<int> extent)
		{
			m_extent = extent;
		}
		void Control::SetPosition(Vector2<int> position)
		{
			m_position = position;
		}
		bool Control::OnEvent(Event &event)
		{
			return true;
		}
	}
}
