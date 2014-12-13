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
#include <CE/RenderPrimitives.h>
#include <CE/Base.h>

using namespace std;

//TODO: Use proper multiple-inheritance stuff for m_extent

namespace ce
{
	namespace ui
	{
		TextDropDownCtrl::TextDropDownSelectCtrl::TextDropDownSelectCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, TextDropDownCtrl *source, Color color)
			: ColorCtrl(position, extent, color), m_source(source), m_font(font), m_hoverId(255)
		{
			m_type = Type_TextDropDownCtrl;
			m_eventMask |= event::Mask_MouseButtonDown | event::Mask_MouseButtonUp | event::Mask_MouseMotion;

			m_hasControlZones = true;
		}
		bool TextDropDownCtrl::TextDropDownSelectCtrl::OnEvent(Event &event)
		{
			return true;
		}
		void TextDropDownCtrl::TextDropDownSelectCtrl::OnControlZoneSelect(ControlZone *zone)
		{
			m_source->Select(zone->id);
		}
		void TextDropDownCtrl::TextDropDownSelectCtrl::OnControlZoneHover(ControlZone *zone)
		{
			m_hoverId = zone->id;
		}
		void TextDropDownCtrl::TextDropDownSelectCtrl::UpdateControlZones()
		{
			m_controlZones.clear();
			m_controlZones.reserve(m_source->m_selections.size());

			ControlZone zone;
			memset(&zone, 0, sizeof(zone));
			
			unsigned short idx = 0;
			for(vector<pair<unsigned char, string> >::iterator it = m_source->m_selections.begin(); it != m_source->m_selections.end(); it++)
			{
				zone.id = it->first;
				zone.x = 2;
				zone.y = 2 + idx * 18;
				zone.width = m_extent[0];
				zone.height = 14;

				m_controlZones.push_back(zone);

				idx++;
			}
		}
		//TODO: Remove static padding of 2px
		void TextDropDownCtrl::TextDropDownSelectCtrl::DoRender()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPushMatrix();
				unsigned short idx = 0;
				glTranslatef(2.f, 2.f, 0.f);
				for(vector<pair<unsigned char, string> >::iterator it = m_source->m_selections.begin(); it != m_source->m_selections.end(); it++)
				{
					if(idx)
						glTranslatef(0.f, (float)m_font->GetCharHeight() + 4.f, 0.f);

					if(it->first == m_hoverId)
						glColor4ub(0, 255, 0, 255);
					m_font->DrawStringUI(it->second.c_str(), 0);

					if(it->first == m_hoverId)
						glColor4ub(255, 255, 255, 255);

					idx++;
				}

				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
			glDisable(GL_BLEND);
		}

		TextDropDownCtrl::TextDropDownCtrl(Vector2<int_canvas> position, Vector2<int_canvas> extent, Font *font, const char *text, Color color)
			: TextCtrl(position, extent, font, text, color), m_selectionIdx(65535), m_placeHolder(text), m_OnSelection(0)
		{
			m_type = Type_TextDropDownCtrl;
			m_eventMask |= event::Mask_MouseButtonDown | event::Mask_MouseButtonUp;

			m_selector = new TextDropDownSelectCtrl(position, extent + extent, font, this, color);
			m_selector->SetVisible(false);
		}
		TextDropDownCtrl::~TextDropDownCtrl()
		{
			delete m_selector;
		}
		void TextDropDownCtrl::OnAdded(Control *parent)
		{
			GetRoot()->Add(m_selector);
		}
		void TextDropDownCtrl::OnDimensionUpdate()
		{
			if(m_selector->IsVisible())
				m_selector->SetPosition(m_absolutePosition + Vector2<int_canvas>(0, m_extent[1]));
		}
		void TextDropDownCtrl::DoRender()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glPushMatrix();
				glColor4ub(255, 0, 0, 63);
				glPushMatrix();
					glScalef((float)m_extent[0], (float)m_extent[1], 0.f);
					RenderSquare();
				glPopMatrix();
				TextCtrl::DoRender();
				glColor4ub(255, 255, 255, 255);
			glPopMatrix();
			glDisable(GL_BLEND);
		}

		bool TextDropDownCtrl::OnEvent(Event &event)
		{
			switch(event.type)
			{
			case event::MouseButtonDown:
				if(event.mouseButton.button == event::MouseButtonLeft)
				{
					if(m_selector->IsVisible())
						m_selector->SetVisible(false);
					else
					{
						m_selector->SetPosition(m_absolutePosition + Vector2<int_canvas>(0, m_extent[1]));
						m_selector->SetVisible(true);
					}
				}

				break;
			}

			return true;
		}

		//- Selections -
		void TextDropDownCtrl::AddSelection(unsigned char id, string text)
		{
			m_selections.push_back(pair<unsigned char, string>(id, text));
			m_selector->UpdateControlZones();
		}
		void TextDropDownCtrl::Select(unsigned char id)
		{
			vector<pair<unsigned char, string> >::iterator it = m_selections.begin();
			for(; it != m_selections.end(); it++)
				if(it->first == id)
				{
					m_selectionIdx = it - m_selections.begin();
					m_text = it->second;
				}
			if(it == m_selections.end())
			{
				m_selectionIdx = 65535;
				m_text = m_placeHolder;
			}
			if(m_OnSelection)
				(*m_OnSelection)(this);
		}
	}
}
