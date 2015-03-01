//- Standard Library -
#include <cstring>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

//- Centhra Engine -
#include <CE/UI/TextDropDownCtrl.h>
#include <CE/Renderer.h>
#include <CE/Base.h>

using namespace std;

namespace ce
{
	namespace ui
	{
		TextDropDownCtrl::TextDropDownSelectCtrl::TextDropDownSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, TextDropDownCtrl *source, Color<float> textColor, Color<float> backgroundColor)
			: ColorCtrl(position, extent, backgroundColor), m_source(source), m_font(font), m_hoverIdx(65535)
		{
			m_type = Type_TextDropDownCtrl;
			m_eventMask |= event::Mask_MouseButtonDown | event::Mask_MouseButtonUp | event::Mask_MouseMotion;

			m_hasControlZones = true;
		}
		TextDropDownCtrl::TextDropDownSelectCtrl::~TextDropDownSelectCtrl()
		{
			if(m_source)
				m_source->m_selector = 0;
		}
		bool TextDropDownCtrl::TextDropDownSelectCtrl::OnEvent(Event &event)
		{
			//- Handle out of bounds -
			switch(event.type)
			{
			case event::MouseButtonDown:
				if(!Contains(Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y)))
				{
					SetVisible(false);
					ReleaseEvent(event::MouseButtonDown);
					ReleaseEvent(event::MouseMotion);
					return false;
				}
				break;
			case event::MouseMotion:
				if(!Contains(Vector2<int_canvas>(event.mouseButton.x, event.mouseButton.y)))
					if(m_hoverIdx != 65535)
						m_hoverIdx = 65535;
				break;
			}

			return true;
		}
		void TextDropDownCtrl::TextDropDownSelectCtrl::OnControlZoneSelect(ControlZone *zone)
		{
			if(m_source)
				m_source->Select(zone->id);
		}
		void TextDropDownCtrl::TextDropDownSelectCtrl::OnControlZoneHover(ControlZone *zone)
		{
			m_hoverIdx = (unsigned short)(zone - &m_controlZones[0]);
		}
		void TextDropDownCtrl::TextDropDownSelectCtrl::UpdateControlZones()
		{
			m_controlZones.clear();
			m_controlZones.reserve(m_source->m_selections.size());

			ControlZone zone;
			memset(&zone, 0, sizeof(zone));
			
			unsigned short selectionHeight = m_font->GetCharSize() + 4;
			unsigned short idx = 0;
			if(m_source->m_selections.empty() == false)
			{
				pair<unsigned char, string> *markOptions = &m_source->m_selections.front();
				pair<unsigned char, string> *endOptions = markOptions + m_source->m_selections.size();
				while(markOptions != endOptions)
				{
					zone.id = (markOptions++)->first;
					zone.x = 0;
					zone.y = idx * selectionHeight;
					zone.width = m_extent[0];
					zone.height = selectionHeight;

					m_controlZones.push_back(zone);

					idx++;
				}
			}
			if(m_controlZones.size())
				SetExtent(Vector2<int_canvas>(m_extent[0], 2 + m_controlZones.size() * selectionHeight));
			else
				SetExtent(Vector2<int_canvas>(m_extent[0], 0));
		}
		//TODO: Remove static padding of 2px
		void TextDropDownCtrl::TextDropDownSelectCtrl::DoRender(RenderContext &context)
		{
			//TODO: Refer to previous context mvp matrix
			RenderContext currentContext = context;

			//- Render Background -
			ColorCtrl::DoRender(context);

			//- Render Text -
			{
				unsigned short idx = 0;
				ShaderProgram::TexturedProgram *program = 0;
				if(context.useShaders)
					program = UseTexturedProgram();
				if(program != 0)
				{
					glUniform4f(program->color, 1.f, 1.f, 1.f, 1.f);
					currentContext.mvpMatrix = context.projectionMatrix * m_absoluteMatrix;
					currentContext.mvpMatrix *= Matrix4x4<float>::BuildFromTranslation(Vector2<int_canvas>(2, 2));
					if(m_source->m_selections.empty() == false)
					{
						pair<unsigned char, string> *markOptions = &m_source->m_selections.front();
						pair<unsigned char, string> *endOptions = markOptions + m_source->m_selections.size();
						while(markOptions != endOptions)
						{
							if(idx)
								currentContext.mvpMatrix *= Matrix4x4<float>::BuildFromTranslation(Vector2<int_canvas>(0, m_font->GetCharSize() + 4));
							if(idx == m_hoverIdx)
								glUniform4f(program->color, 0.f, 1.f, 0.f, 1.f);
							m_font->DrawStringUI(currentContext, (markOptions++)->second.c_str(), 0);
							if(idx == m_hoverIdx)
								glUniform4f(program->color, 1.f, 1.f, 1.f, 1.f);
							idx++;
						}
					}
				}
				else
				{
					glLoadMatrixf(&m_absoluteMatrix[0]);
					glPushMatrix();
					glColor4ub(255, 255, 255, 255);

					if(m_source->m_selections.empty() == false)
					{
						pair<unsigned char, string> *markOptions = &m_source->m_selections.front();
						pair<unsigned char, string> *endOptions = markOptions + m_source->m_selections.size();
						while(markOptions != endOptions)
						{
							if(idx)
								glTranslatef(0.f, (float)m_font->GetCharSize() + 4.f, 0.f);
							if(idx == m_hoverIdx)
								glColor4ub(0, 255, 0, 255);
							m_font->DrawStringUI(currentContext, (markOptions++)->second.c_str(), 0);
							if(idx == m_hoverIdx)
								glColor4ub(255, 255, 255, 255);
							idx++;
						}
					}
					glPopMatrix();
				}
			}
			/*
			glPushMatrix();
				unsigned short idx = 0;
				glTranslatef(2.f, 2.f, 0.f);
				if(m_source->m_selections.empty() == false)
				{
					pair<unsigned char, string> *markOptions = &m_source->m_selections.front();
					pair<unsigned char, string> *endOptions = markOptions + m_source->m_selections.size();
					while(markOptions != endOptions)
					{
						if(idx)
							glTranslatef(0.f, (float)m_font->GetCharSize() + 4.f, 0.f);
						if(idx == m_hoverIdx)
							glColor4ub(0, 255, 0, 255);
						m_font->DrawStringUI(context, (markOptions++)->second.c_str(), 0);
						if(idx == m_hoverIdx)
							glColor4ub(255, 255, 255, 255);
						idx++;
					}
				}

				glColor4ub(255, 255, 255, 255);
			glPopMatrix();*/
		}

		TextDropDownCtrl::TextDropDownCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, int_canvas selectorWidth, const char *text, Color<float> textColor, Color<float> backgroundColor)
			: TextCtrl(position, extent, font, text, textColor, backgroundColor), m_selectionIdx(65535), m_placeHolder(text),
			m_OnSelection(0), m_selectorWidth(selectorWidth)
		{
			m_type = Type_TextDropDownCtrl;
			m_eventMask |= event::Mask_MouseButtonDown | event::Mask_MouseButtonUp;

			m_selector = new TextDropDownSelectCtrl(position, Vector2<int_canvas>(selectorWidth, 0), font, this, textColor);
			m_selector->SetVisible(false);
		}
		TextDropDownCtrl::~TextDropDownCtrl()
		{
			if(m_selector)
			{
				m_selector->m_source = 0;
				delete m_selector;
			}
		}
		void TextDropDownCtrl::ReattachSelector()
		{
			if(m_selector)
				if(m_parent)
					GetRoot()->Add(m_selector);
		}
		void TextDropDownCtrl::OnAdded(Control *parent)
		{
			if(m_selector)
				GetRoot()->Add(m_selector);
		}
		void TextDropDownCtrl::OnDimensionUpdate()
		{
			if(m_selector)
				if(m_selector->IsVisible())
					m_selector->SetPosition(m_absolutePosition + Vector2<int_canvas>(0, m_extent[1]));
		}
		bool TextDropDownCtrl::OnEvent(Event &event)
		{
			switch(event.type)
			{
			case event::MouseButtonDown:
				if(event.mouseButton.button == event::MouseButtonLeft)
				{
					if(m_selector)
					{
						if(m_selector->IsVisible())
						{
							//TODO: Determine if this is redundant
							m_selector->SetVisible(false);
							m_selector->ReleaseEvent(event::MouseButtonDown);
							m_selector->ReleaseEvent(event::MouseMotion);
						}
						else
						{
							m_selector->SetPosition(m_absolutePosition + Vector2<int_canvas>(0, m_extent[1]));
							m_selector->SetVisible(true);
							m_selector->CaptureEvent(event::MouseButtonDown);
							m_selector->CaptureEvent(event::MouseMotion);
						}
					}
				}
				return false;
			}

			return true;
		}

		//- Selections -
		void TextDropDownCtrl::AddSelection(unsigned char id, string text)
		{
			m_selections.push_back(pair<unsigned char, string>(id, text));
			if(m_selector)
				m_selector->UpdateControlZones();
		}
		void TextDropDownCtrl::ClearSelections()
		{
			m_selections.clear();
			if(m_selector)
				m_selector->UpdateControlZones();
		}
		void TextDropDownCtrl::Select(unsigned char id)
		{
			if(m_selector)
				if(m_selector->IsVisible())
				{
					m_selector->SetVisible(false);
					m_selector->ReleaseEvent(event::MouseButtonDown);
					m_selector->ReleaseEvent(event::MouseMotion);
				}

			if(m_selections.empty() == false)
			{
				pair<unsigned char, string> *markOptions = &m_selections.front();
				pair<unsigned char, string> *endOptions = markOptions + m_selections.size();
				while(markOptions != endOptions)
				{
					if(markOptions->first == id)
					{
						m_selectionIdx = markOptions - &m_selections.front();
						m_text = markOptions->second;
						break;
					}
					markOptions++;
				}
				if(markOptions == endOptions)
				{
					m_selectionIdx = 65535;
					m_text = m_placeHolder;
				}
			}
			else
			{
				m_selectionIdx = 65535;
				m_text = m_placeHolder;
			}
			if(m_OnSelection)
				(*m_OnSelection)(this, id);
		}
		void TextDropDownCtrl::SetOnSelection(bool(*callback)(TextDropDownCtrl *, unsigned char id))
		{
			m_OnSelection = callback;
		}
	}
}
