//- Standard Library -
#include <algorithm>

//- Centhra Engine -
#include <CE/Game2D/Plane.h>
#include <CE/Game2D/Zone.h>
#include <CE/Base.h>

//- OpenGL -
#include <GL/gl.h>

using namespace std;

namespace ce
{
	namespace game2d
	{
		Zone::Zone(float minX, float minY, float maxX, float maxY)
		{
			m_minX = minX;
			m_minY = minY;
			m_maxX = maxX;
			m_maxY = maxY;
			m_plane = 0;
		}
		Zone::~Zone()
		{
		}
		void Zone::Render()
		{
			for(vector<ZoneEntity *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->Render();

			glPushMatrix();
				if(m_children.size())
					glColor3ub(255, 0, 0);
				glBegin(GL_LINE_LOOP);
					glVertex2f(m_minX + 0.5f, m_minY + 0.5f);
					glVertex2f(m_maxX - 0.5f, m_minY + 0.5f);
					glVertex2f(m_maxX - 0.5f, m_maxY - 0.5f);
					glVertex2f(m_minX + 0.5f, m_maxY - 0.5f);
					glVertex2f(m_minX + 0.5f, m_minY + 0.5f);
				glEnd();
				glColor3ub(255, 255, 255);
			glPopMatrix();
		}
		void Zone::Add(ZoneEntity *entity)
		{
			if(!IsMember(entity))
			{
				m_children.push_back(entity);
				entity->AddZone(this);
			}
		}
		bool Zone::IsMember(ZoneEntity *entity) const
		{
			return find(m_children.begin(), m_children.end(), entity) != m_children.end();
		}
		void Zone::Remove(ZoneEntity *entity)
		{
			vector<ZoneEntity *>::iterator it = find(m_children.begin(), m_children.end(), entity);
			if(it != m_children.end())
			{
				(*it)->RemoveZone(this);
				m_children.erase(it);
			}
		}
		void Zone::FinishRender()
		{
			for(vector<ZoneEntity *>::iterator it = m_children.begin(); it != m_children.end(); it++)
				(*it)->FinishRender();
		}
		void Zone::SetPlane(Plane *plane)
		{
			m_plane = plane;
		}
		Plane *Zone::GetPlane() const
		{
			return m_plane;
		}
	}
}