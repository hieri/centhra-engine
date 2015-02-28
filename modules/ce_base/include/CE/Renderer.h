#ifndef _CE_BASE_RENDERER_H_
#define _CE_BASE_RENDERER_H_

//- Centhra Engine -
#include <CE/RenderContext.h>
#include <CE/Shader.h>

//- Standard Library -
#include <string>
#include <map>

#ifdef _WIN32
	//- Windows -
	#include <Windows.h>
#endif

//- OpenGL -
#include <GL/gl.h>

#ifdef _WIN32
	#include <GL/glext.h>
	#include <GL/wglext.h>
	#define glGetProcAddress wglGetProcAddress
	#define GLPROCPREFIX
#elif __linux__
	#include <GL/glx.h>
	#define glGetProcAddress glXGetProcAddress
	#define GLPROCPREFIX (const unsigned char *)
#endif

//- GLSL -
//extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
extern PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

namespace ce
{
	//- Rendering Primitives -
	void RenderPrimitiveCleanup();
	void RenderSquare(RenderContext &context);
	void RenderSquareTexturedFull(RenderContext &context);
	void LoadExtensions();
	void BindSquareEBO();

	//- Rendering Control -
	void BindVBO(unsigned int vbo);
	void BindEBO(unsigned int ebo);
	void ConfigGeneric();
	void ConfigTextured();

	ShaderProgram::GenericProgram *UseGenericProgram();
	ShaderProgram::TexturedProgram *UseTexturedProgram();
}

#endif
