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

#ifdef _WIN32
	#define glGetProcAddress wglGetProcAddress
#elif __linux__
	#include <GL/glx.h>
	#define glGetProcAddress glXGetProcAddress
#endif

//- GLSL -
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

#ifdef _WIN32
	PFNGLGENBUFFERSPROC glGenBuffers = (PFNGLGENBUFFERSPROC)glGetProcAddress("glGenBuffers");
	PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)glGetProcAddress("glBindBuffer");
	PFNGLBUFFERDATAPROC glBufferData = (PFNGLBUFFERDATAPROC)glGetProcAddress("glBufferData");
	PFNGLDELETEBUFFERSPROC glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glGetProcAddress("glDeleteBuffers");
#elif __linux__
	PFNGLGENBUFFERSPROC glGenBuffers = (PFNGLGENBUFFERSPROC)glGetProcAddress((const unsigned char *)"glGenBuffers");
	PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)glGetProcAddress((const unsigned char *)"glBindBuffer");
	PFNGLBUFFERDATAPROC glBufferData = (PFNGLBUFFERDATAPROC)glGetProcAddress((const unsigned char *)"glBufferData");
	PFNGLDELETEBUFFERSPROC glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glGetProcAddress((const unsigned char *)"glDeleteBuffers");
#endif

namespace ce
{
	namespace game2d
	{
		GLuint g_squareVBO = 0, g_squareEBO = 0;
//		GLint g_squareList = 0;
		void RenderSquare()
		{
			/*
			if(!g_squareList)
			{
				g_squareList = glGenLists(1);
				glNewList(g_squareList, GL_COMPILE);
					glBegin(GL_QUADS);
						glVertex2f(0.f, 0.f);
						glVertex2f(1.f, 0.f);
						glVertex2f(1.f, 1.f);
						glVertex2f(0.f, 1.f);
					glEnd();
				glEndList();
			}
			glCallLists(1, GL_UNSIGNED_BYTE, &g_squareList);
			*/
			if(!g_squareVBO)
			{
				glGenBuffers(1, &g_squareVBO);
				glBindBuffer(0x8892, g_squareVBO);

				unsigned int size = 4 * 2;
				float *vbo = new float[size];

				vbo[0] = 0.f;
				vbo[1] = 0.f;

				vbo[2] = 1.f;
				vbo[3] = 0.f;

				vbo[4] = 1.f;
				vbo[5] = 1.f;

				vbo[6] = 0.f;
				vbo[7] = 1.f;

				glBufferData(0x8892, size * 4, vbo, 0x88E4);
				delete [] vbo;
			}

			if(!g_squareEBO)
			{
				glGenBuffers(1, &g_squareEBO);
				glBindBuffer(0x8893, g_squareEBO);

				unsigned int size = 2 * 3;
				unsigned char *ebo = new unsigned char[size];

				ebo[0] = 0;
				ebo[1] = 1;
				ebo[2] = 2;

				ebo[3] = 0;
				ebo[4] = 2;
				ebo[5] = 3;

				glBufferData(0x8893, size, ebo, 0x88E4);
				delete [] ebo;
			}


			glBindBuffer(0x8892, g_squareVBO);
			glBindBuffer(0x8893, g_squareEBO);

			glVertexPointer(2, GL_FLOAT, 0, 0);
			glEnableClientState(GL_VERTEX_ARRAY);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

			glDisableClientState(GL_VERTEX_ARRAY);
		}
		void ZoneEntity::Cleanup()
		{
//			glDeleteLists(g_squareList, 1);
			glDeleteBuffers(1, &g_squareVBO);
		}

		unsigned int ZoneEntity::ms_lastID = -1;
		ZoneEntity::ZoneEntity(Vector2<float> position, Vector2<float> extent)
		{
			m_position = position;
			m_extent = extent;
			m_isRendered = m_startedPhysics = m_finishedPhysics = false;
			m_color = Color(rand() % 256,  rand() % 256, rand() % 256);
			m_velocity = Vector2<float>(0.f, 0.f);
			m_movePadding = Vector2<float>(1.f, 1.f);
			m_canMove[0] = m_canMove[1] = true;
			m_collisionMask |= 1;

			ms_lastID++;
			m_id = ms_lastID;
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
			glScalef(m_extent[0], m_extent[1], 1.f);
			RenderSquare();
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
		Vector2<float> ZoneEntity::GetVelocity() const
		{
			return m_velocity;
		}
		void ZoneEntity::SetExtent(Vector2<float> extent)
		{
			m_extent = extent;
		}
		void ZoneEntity::SetPosition(Vector2<float> position)
		{
			m_position = position;
		}
		void ZoneEntity::SetVelocity(Vector2<float> velocity)
		{
			m_velocity = velocity;
		}
		vector<Zone *> &ZoneEntity::GetZones()
		{
			return m_zones;
		}
		void ZoneEntity::Move(Vector2<float> movement)
		{
			if(m_zones.size())
				m_zones[0]->MoveEntity(this, movement);
			else
				m_position += movement;
		}
		bool ZoneEntity::CollidesWith(ZoneEntity *entity, Vector2<float> offsetA, Vector2<float> offsetB)
		{
			Vector2<float> aMin, aMax, bMin, bMax;
			aMin = m_position + offsetA;
			aMax = aMin + m_extent;
			bMin = entity->m_position + offsetB;
			bMax = bMin + entity->m_extent;

			return aMin[0] < bMax[0] && aMin[1] < bMax[1] && bMin[0] < aMax[0] && bMin[1] < aMax[1];
		}
		bool ZoneEntity::CollidesWith(Vector2<float> boxMin, Vector2<float> boxMax, Vector2<float> offset)
		{
			Vector2<float> min, max;
			min = m_position + offset;
			max = min + m_extent;

			return min[0] < boxMax[0] && min[1] < boxMax[1] && boxMin[0] < max[0] && boxMin[1] < max[1];
		}
		unsigned int ZoneEntity::GetCollisionMask() const
		{
			return m_collisionMask;
		}
		void ZoneEntity::SetCollisionMask(unsigned int mask)
		{
			m_collisionMask = mask;
		}
	}
}
