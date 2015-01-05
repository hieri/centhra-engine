//- Standard Library -
#include <algorithm>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/UI/ButtonCtrl.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		map<int, vector<ButtonCtrl *> *> ButtonCtrl::ms_buttonGroupMap;

		ButtonCtrl::ButtonCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent) : Control(position, extent)
		{
			m_type = Type_ButtonCtrl;
			m_eventMask |= event::Mask_MouseButtonDown | event::Mask_MouseButtonUp;
			m_isEnabled = true;
			m_group = -1;
			m_OnButtonDown = 0;
			m_OnButtonUp = 0;
		}
		ButtonCtrl::~ButtonCtrl()
		{
			if(m_group >= 0)
			{
				vector<ButtonCtrl *>::iterator it = find(ms_buttonGroupMap[m_group]->begin(), ms_buttonGroupMap[m_group]->end(), this);
				if(it != ms_buttonGroupMap[m_group]->end())
				{
					ms_buttonGroupMap[m_group]->erase(it);
					if(!ms_buttonGroupMap[m_group]->size())
					{
						delete ms_buttonGroupMap[m_group];
						ms_buttonGroupMap.erase(m_group);
					}
				}
			}
		}
		void ButtonCtrl::DoRender(RenderContext &context)
		{
			glPushMatrix();
				glColor4ub(255, 255, 0, 255);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glScalef((float)m_extent[0], (float)m_extent[1], 0.f);
				glBegin(GL_QUADS);
					glVertex2i(0, 0);
					glVertex2i(1, 0);
					glVertex2i(1, 1);
					glVertex2i(0, 1);
				glEnd();
				glDisable(GL_BLEND);
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
		}
		int ButtonCtrl::GetGroup() const
		{
			return m_group;
		}
		bool ButtonCtrl::IsEnabled() const
		{
			return m_isEnabled;
		}
		void ButtonCtrl::SetOnButtonDown(bool (*callback)(ButtonCtrl *))
		{
			m_OnButtonDown = callback;
		}
		void ButtonCtrl::SetOnButtonUp(bool(*callback)(ButtonCtrl *))
		{
			m_OnButtonUp = callback;
		}
		void ButtonCtrl::SetEnabled(bool enabled)
		{
			m_isEnabled = enabled;
		}
		void ButtonCtrl::SetGroup(int group)
		{
			if(m_group != group)
			{
				if(m_group >= 0)
				{
					vector<ButtonCtrl *>::iterator it = find(ms_buttonGroupMap[m_group]->begin(), ms_buttonGroupMap[m_group]->end(), this);
					if(it != ms_buttonGroupMap[m_group]->end())
					{
						ms_buttonGroupMap[m_group]->erase(it);
						if(!ms_buttonGroupMap[m_group]->size())
						{
							delete ms_buttonGroupMap[m_group];
							ms_buttonGroupMap.erase(m_group);
						}
					}
				}

				if(group >= 0)
				{
					if(!ms_buttonGroupMap.count(group))
						ms_buttonGroupMap[group] = new vector<ButtonCtrl *>();
					ms_buttonGroupMap[group]->push_back(this);
				}

				m_group = group;
			}
		}
		bool ButtonCtrl::OnEvent(Event &event)
		{
			switch(event.type)
			{
			case event::MouseButtonDown:
				if(m_OnButtonDown)
					return (*m_OnButtonDown)(this);
				break;
			case event::MouseButtonUp:
				if(m_OnButtonUp)
					return (*m_OnButtonUp)(this);
				break;
			}
			return Control::OnEvent(event);
		}
	}
}
