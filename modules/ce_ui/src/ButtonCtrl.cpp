//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Renderer.h>
#include <CE/UI/ButtonCtrl.h>

//- Standard Library -
#include <algorithm>

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
			//TODO: Consider generating absolute matrix w/ scale on position/extent change
			ConfigGeneric();
			ShaderProgram::GenericProgram *program = 0;
			if(context.useShaders)
				program = UseGenericProgram();
			if(program != 0)
			{
				Matrix4x4<float> mvpMatrix = context.mvpMatrix * m_absoluteMatrix;
				mvpMatrix *= Matrix4x4<float>::BuildFromScale(m_extent);
				glUniformMatrix4fv(program->mvpMatrix, 1, GL_FALSE, &mvpMatrix[0]);
				glUniform4f(program->color, 1.f, 1.f, 0.f, 1.f);
			}
			else
			{
				Matrix4x4<float> mvMatrix = context.modelViewMatrix * m_absoluteMatrix;
				mvMatrix *= Matrix4x4<float>::BuildFromScale(m_extent);
				glLoadMatrixf(&mvMatrix[0]);
				glColor4ub(255, 255, 0, 255);
			}
			RenderSquare(context);
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
