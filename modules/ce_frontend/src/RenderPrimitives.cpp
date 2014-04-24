//- Centhra Engine 
#include <CE/Base.h>
#include <CE/RenderPrimitives.h>

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
	void RenderPrimitiveCleanup()
	{
		glDeleteBuffers(1, &g_squareEBO);
		glDeleteBuffers(1, &g_squareVBO);
	}
}
