//- Standard Library -
#include <fstream>
#include <streambuf>
#include <vector>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/BufferObject.h>

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

using namespace std;

namespace ce
{
	void BufferObject::unbind(int type)
	{
		glBindBuffer(type, 0);
	}

	BufferObject::BufferObject(int type)
	{
		m_type = type;
		m_glBufferObject = 0;
		glGenBuffers(1, &m_glBufferObject);
	}
	BufferObject::~BufferObject()
	{
		glDeleteBuffers(1, &m_glBufferObject);
	}
	void BufferObject::bind()
	{
		glBindBuffer(m_type, m_glBufferObject);
	}
	void BufferObject::setData(void *data, unsigned int size, int mode)
	{
		bind();

		glBufferData(m_type, size, data, mode);
	}
}
