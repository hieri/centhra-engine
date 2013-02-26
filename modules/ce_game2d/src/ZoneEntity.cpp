//- Centhra Engine -
#include <CE/Game2D/Zone.h>
#include <CE/Game2D/ZoneEntity.h>
#include <CE/Game2D/Plane.h>
#include <CE/Base.h>

//- Standard Library -
#include <algorithm>
#include <stdlib.h>

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		ZoneEntity::ZoneEntity(Vector2<float> position, Vector2<float> extent)
		{
			m_position = position;
			m_extent = extent;
			m_isRendered = false;
			m_color = Color(rand() % 256,  rand() % 256, rand() % 256);
		}
		ZoneEntity::~ZoneEntity()
		{
			for(vector<Zone *>::iterator it = m_zones.begin(); it != m_zones.end(); it++)
				(*it)->Remove(this);
		}
		void ZoneEntity::Render()
		{
			if(!m_isRendered)
			{
				glPushMatrix();
				DoRender();
				glPopMatrix();
				m_isRendered = true;
			}
		}
		void ZoneEntity::DoRender()
		{
			glColor3ub(m_color[0], m_color[1], m_color[2]);
			glTranslatef(m_position[0], m_position[1], 0.f);
			glBegin(GL_QUADS);
				glVertex2f(0.f, 0.f);
				glVertex2f(m_extent[0], 0.f);
				glVertex2f(m_extent[0], m_extent[1]);
				glVertex2f(0.f, m_extent[1]);
			glEnd();
			glColor3ub(255, 255, 255);
		}
		void ZoneEntity::FinishRender()
		{
			m_isRendered = false;
		}
		void ZoneEntity::AddZone(Zone *zone)
		{
			if(!ContainsZone(zone))
				m_zones.push_back(zone);
		}
		bool ZoneEntity::ContainsZone(Zone *zone) const
		{
			return find(m_zones.begin(), m_zones.end(), zone) != m_zones.end();
		}
		void ZoneEntity::RemoveZone(Zone *zone)
		{
			vector<Zone *>::iterator it = find(m_zones.begin(), m_zones.end(), zone);
			if(it != m_zones.end())
				m_zones.erase(it);
		}
		Vector2<float> ZoneEntity::GetExtent() const
		{
			return m_extent;
		}
		Vector2<float> ZoneEntity::GetPosition() const
		{
			return m_position;
		}
		void ZoneEntity::SetExtent(Vector2<float> extent)
		{
			m_extent = extent;
		}
		void ZoneEntity::SetPosition(Vector2<float> position)
		{
			m_position = position;
		}
		vector<Zone *> &ZoneEntity::GetZones()
		{
			return m_zones;
		}
		void ZoneEntity::Move(Vector2<float> movement)
		{
			bool partOfPlane = false;
			game2d::Plane *plane = 0;
			if(m_zones.size())
			{
				plane = m_zones[0]->GetPlane();
				if(plane)
					partOfPlane = true;
			}

			if(partOfPlane)
			{
				plane->MoveEntity(this, movement);

			}
			else
			{

			}
		}
	}
}