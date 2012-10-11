//- Standard Library -
#include <fstream>
#include <streambuf>
#include <vector>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Shader.h>

#ifdef _WIN32
	#define glGetProcAddress wglGetProcAddress
#elif __linux__
	#include <GL/glx.h>
	#define glGetProcAddress glXGetProcAddress
#endif

//- GLSL -
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
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
extern PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

#ifdef _WIN32
	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)glGetProcAddress("glCreateProgramObjectARB");
	PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)glGetProcAddress("glCreateShaderObjectARB");
	PFNGLSHADERSOURCEARBPROC glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)glGetProcAddress("glShaderSourceARB");
	PFNGLCOMPILESHADERARBPROC glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)glGetProcAddress("glCompileShaderARB");
	PFNGLATTACHOBJECTARBPROC glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)glGetProcAddress("glAttachObjectARB");
	PFNGLLINKPROGRAMARBPROC glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)glGetProcAddress("glLinkProgramARB");
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)glGetProcAddress("glUseProgramObjectARB");
	PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)glGetProcAddress("glDeleteObjectARB");
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glGetProcAddress("glGetAttribLocation");
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glGetProcAddress("glGetUniformLocation");
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glGetProcAddress("glUniformMatrix4fv");
	PFNGLGETINFOLOGARBPROC glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)glGetProcAddress("glGetInfoLogARB");
	PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)glGetProcAddress("glVertexAttrib1f");
	PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)glGetProcAddress("glVertexAttrib4fv");
	PFNGLGETSHADERIVPROC glGetShaderiv = (PFNGLGETSHADERIVPROC)glGetProcAddress("glGetShaderiv");
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glGetProcAddress("glEnableVertexAttribArray");
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glGetProcAddress("glDisableVertexAttribArray");
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glGetProcAddress("glVertexAttribPointer");
#elif __linux__
	PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)glGetProcAddress((const unsigned char *)"glCreateProgramObjectARB");
	PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)glGetProcAddress((const unsigned char *)"glCreateShaderObjectARB");
	PFNGLSHADERSOURCEARBPROC glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)glGetProcAddress((const unsigned char *)"glShaderSourceARB");
	PFNGLCOMPILESHADERARBPROC glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)glGetProcAddress((const unsigned char *)"glCompileShaderARB");
	PFNGLATTACHOBJECTARBPROC glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)glGetProcAddress((const unsigned char *)"glAttachObjectARB");
	PFNGLLINKPROGRAMARBPROC glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)glGetProcAddress((const unsigned char *)"glLinkProgramARB");
	PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)glGetProcAddress((const unsigned char *)"glUseProgramObjectARB");
	PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)glGetProcAddress((const unsigned char *)"glDeleteObjectARB");
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glGetProcAddress((const unsigned char *)"glGetAttribLocation");
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glGetProcAddress((const unsigned char *)"glGetUniformLocation");
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glGetProcAddress((const unsigned char *)"glUniformMatrix4fv");
	PFNGLGETINFOLOGARBPROC glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)glGetProcAddress((const unsigned char *)"glGetInfoLogARB");
	PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)glGetProcAddress((const unsigned char *)"glVertexAttrib1f");
	PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)glGetProcAddress((const unsigned char *)"glVertexAttrib4fv");
	PFNGLGETSHADERIVPROC glGetShaderiv = (PFNGLGETSHADERIVPROC)glGetProcAddress((const unsigned char *)"glGetShaderiv");
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glGetProcAddress((const unsigned char *)"glEnableVertexAttribArray");
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glGetProcAddress((const unsigned char *)"glDisableVertexAttribArray");
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glGetProcAddress((const unsigned char *)"glVertexAttribPointer");
#endif

using namespace std;

namespace ce
{
	Shader *Shader::createFromFile(const char *file, int type)
	{
		if(!fileExists(file))
		{
			setError("Shader::createFromFile: File does not exist <%s>.", file);
			return 0;
		}

		ifstream fileStream(file);
		string fileBuffer;

		fileStream.seekg(0, ios::end);   
		fileBuffer.reserve(fileStream.tellg());
		fileStream.seekg(0, ios::beg);

		fileBuffer.assign((istreambuf_iterator<char>(fileStream)), istreambuf_iterator<char>());
		fileStream.close();

		int glShaderObject = glCreateShaderObjectARB(type);

		//- Define and Compile Shader -
		const char *c_str = fileBuffer.c_str();
		glShaderSourceARB(glShaderObject, 1, (const GLcharARB **)&c_str, 0);
		glCompileShaderARB(glShaderObject);

		//- Check for Compiliation -
		int compiled;
		glGetShaderiv(glShaderObject, GL_COMPILE_STATUS, &compiled);

		//- Error Handling -
		if(compiled == GL_FALSE)
		{
			char infoLogBuffer[1024];
			int infoLogSize = 0;
			glGetInfoLogARB(glShaderObject, 1023, &infoLogSize, infoLogBuffer);
			infoLogBuffer[infoLogSize] = 0;

			glDeleteObjectARB(glShaderObject);
			setError("glCompileShader: %s", infoLogBuffer);
			return 0;
		}

		//- Shader Creation -
		Shader *shader = new Shader;
		shader->m_glShaderObject = glShaderObject;
		shader->m_source = fileBuffer;
		shader->m_type = type;

		return shader;
	}
	Shader *Shader::createFromMemory(const unsigned char *memory, unsigned int size, int type)
	{
		if(!memory)
		{
			setError("Shader::createFromMemory: Empty memory pointer");
			return 0;
		}

		int glShaderObject = glCreateShaderObjectARB(type);

		string source((const char *)memory);

		//- Define and Compile Shader -
		const char *c_str = source.c_str();
		glShaderSourceARB(glShaderObject, 1, (const GLcharARB **)&c_str, 0);
		glCompileShaderARB(glShaderObject);

		//- Check for Compiliation -
		int compiled;
		glGetShaderiv(glShaderObject, GL_COMPILE_STATUS, &compiled);

		//- Error Handling -
		if(compiled == GL_FALSE)
		{
			char infoLogBuffer[1024];
			int infoLogSize = 0;
			glGetInfoLogARB(glShaderObject, 1023, &infoLogSize, infoLogBuffer);
			infoLogBuffer[infoLogSize] = 0;

			glDeleteObjectARB(glShaderObject);
			setError("glCompileShader: %s", infoLogBuffer);
			return 0;
		}

		//- Shader Creation -
		Shader *shader = new Shader;
		shader->m_glShaderObject = glShaderObject;
		shader->m_source = source;
		shader->m_type = type;
		return shader;
	}
	Shader::~Shader()
	{
		glDeleteObjectARB(m_glShaderObject);
	}
	int Shader::getShaderObject() const
	{
		return m_glShaderObject;
	}
	string Shader::getSource() const
	{
		return m_source;
	}
	int Shader::getType() const
	{
		return m_type;
	}

	void ShaderProgram::unuse()
	{
		glUseProgramObjectARB(0);
	}
	ShaderProgram::ShaderProgram()
	{
		m_glProgramObject = glCreateProgramObjectARB();
	}
	ShaderProgram::~ShaderProgram()
	{
		glDeleteObjectARB(m_glProgramObject);
	}
	void ShaderProgram::attachShader(Shader *shader)
	{
		if(shader)
			glAttachObjectARB(m_glProgramObject, shader->getShaderObject());
	}
	int ShaderProgram::getAttribLocation(const char *attribute)
	{
		if(!m_attribLocations.count(attribute))
			m_attribLocations[attribute] = glGetAttribLocation(m_glProgramObject, attribute);
		return m_attribLocations[attribute];
	}
	int ShaderProgram::getProgramObject() const
	{
		return m_glProgramObject;
	}
	int ShaderProgram::getUniformLocation(const char *uniform)
	{
		if(!m_uniformLocations.count(uniform))
			m_uniformLocations[uniform] = glGetUniformLocation(m_glProgramObject, uniform);
		return m_uniformLocations[uniform];
	}
	void ShaderProgram::link()
	{
		glLinkProgramARB(m_glProgramObject);
		m_attribLocations.clear();
		m_uniformLocations.clear();
	}
	void ShaderProgram::use()
	{
		glUseProgramObjectARB(m_glProgramObject);
	}
	void ShaderProgram::enableVertexAttribArray(int attrib)
	{
		glEnableVertexAttribArray(attrib);
	}
	void ShaderProgram::disableVertexAttribArray(int attrib)
	{
		glDisableVertexAttribArray(attrib);
	}
	void ShaderProgram::vertexAttribPointer(int attrib, int a, int b, int c, int d, void *e)
	{
		glVertexAttribPointer(attrib, a, b, c, d, e);
	}
	void ShaderProgram::uniformMatrix4fv(int uniform, int a, int b, float *c)
	{
		glUniformMatrix4fv(uniform, a, b, c);
	}
}
