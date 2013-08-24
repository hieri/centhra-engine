//- Centhra Engine 
#include <CE/Base.h>
#include <CE/Game2D/PhysicalObject.h>

//- Standard Library -
#include <algorithm>
#include <stdlib.h>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

using namespace std;

#ifdef _WIN32
	#include <GL/glext.h>
	#include <GL/wglext.h>
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
		void RenderSquare()
		{
			if(!g_squareVBO)
			{
				#ifdef _WIN32 // TODO: Remove this work around.
					glGenBuffers = (PFNGLGENBUFFERSPROC)glGetProcAddress("glGenBuffers");
					glBindBuffer = (PFNGLBINDBUFFERPROC)glGetProcAddress("glBindBuffer");
					glBufferData = (PFNGLBUFFERDATAPROC)glGetProcAddress("glBufferData");
					glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glGetProcAddress("glDeleteBuffers");
				#endif
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
		void PhysicalObject::Cleanup()
		{
			glDeleteBuffers(1, &g_squareEBO);
			glDeleteBuffers(1, &g_squareVBO);
		}

		unsigned int PhysicalObject::ms_lastID = -1;
		PhysicalObject::PhysicalObject(Vector2<float> position, Vector2<float> extent)
		{
			m_rotation = 0.f;
			m_position = position;
			m_extent = extent;
			m_color = Color(rand() % 256,  rand() % 256, rand() % 256);
			m_velocity = Vector2<float>(0.f, 0.f);
			m_collisionMask = 1;

			m_objectHandle = 0;

			ms_lastID++;
			m_id = ms_lastID;

			m_parentGroup = 0;
			m_isTrigger = m_isStatic = false;
		}
		PhysicalObject::~PhysicalObject()
		{
		}
		void PhysicalObject::Render()
		{
			glPushMatrix();
			DoRender();
			glPopMatrix();
		}
		void PhysicalObject::DoRender()
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glColor4ub(m_color[0], m_color[1], m_color[2], m_color[3]);
//			glTranslatef(m_position[0], m_position[1], -m_position[1]);
			glTranslatef(m_position[0], m_position[1], 0.f);
			glRotatef(m_rotation, 0.f, 0.f, 1.f);
			glScalef(m_extent[0], m_extent[1], 1.f);
			glTranslatef(-0.5f, -0.5f, 0.f);
			RenderSquare();
			glColor3ub(255, 255, 255);

			glDisable(GL_BLEND);
		}
		Vector2<float> PhysicalObject::GetExtent() const
		{
			return m_extent;
		}
		Vector2<float> PhysicalObject::GetPosition() const
		{
			return m_position;
		}
		Vector2<float> PhysicalObject::GetVelocity() const
		{
			return m_velocity;
		}
		float PhysicalObject::GetRotation() const
		{
			return m_rotation;
		}
		void PhysicalObject::SetExtent(Vector2<float> extent, bool updateHandle)
		{
			m_extent = extent;
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetExtent();
		}
		void PhysicalObject::SetPosition(Vector2<float> position, bool updateHandle)
		{
			m_position = position;

			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetPosition();
		}
		void PhysicalObject::SetVelocity(Vector2<float> velocity, bool updateHandle)
		{
			m_velocity = velocity;
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetVelocity();
		}
		void PhysicalObject::SetRotation(float rotation, bool updateHandle)
		{
			m_rotation = rotation;
			
			if(updateHandle)
				if(m_objectHandle)
					m_objectHandle->OnSetRotation();
		}
		PhysicsHandler::ObjectHandle *PhysicalObject::GetObjectHandle() const
		{
			return m_objectHandle;
		}
		bool PhysicalObject::IsStatic() const
		{
			return m_isStatic;
		}
		unsigned int PhysicalObject::GetCollisionMask() const
		{
			return m_collisionMask;
		}
		void PhysicalObject::SetCollisionMask(unsigned int mask)
		{
			m_collisionMask = mask;

			if(m_objectHandle)
				m_objectHandle->OnSetCollisionMask();
		}
		bool PhysicalObject::IsTrigger() const
		{
			return m_isTrigger;
		}
		bool PhysicalObject::OnCollision(PhysicalObject *collider, Vector2<float> pointOfContact)
		{
			return true;
		}
		void PhysicalObject::SetStatic(bool isStatic)
		{
			m_isStatic = isStatic;

			if(m_objectHandle)
				m_objectHandle->OnSetStatic();
		}
	}
}
