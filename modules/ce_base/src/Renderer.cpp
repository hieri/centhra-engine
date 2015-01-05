//- Standard Library -
#include <fstream>
#include <streambuf>
#include <vector>
#include <cstring>

//- Centhra Engine -
#include <CE/Base.h>
#include <CE/Renderer.h>
#include <CE/Shader.h>

PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = 0;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = 0;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = 0;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = 0;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB = 0;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = 0;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = 0;
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLUNIFORM1IPROC glUniform1i = 0;
PFNGLUNIFORM4FPROC glUniform4f = 0;
PFNGLUNIFORM4FVPROC glUniform4fv = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB = 0;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = 0;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;

using namespace std;

//TODO: Generic rendering initialization and cleanup
//TODO: Figure out a way to default to GL_TEXTURE0 within the shader

namespace ce
{
	//----------------------------- Globals -----------------------------
	ShaderProgram::GenericProgram *g_defaultGenericProgram = 0, *g_genericProgram = 0;
	ShaderProgram::TexturedProgram *g_defaultTexturedProgram = 0, *g_texturedProgram = 0;
	ShaderProgram *g_lastProgram = 0;
	bool g_extensionsLoaded = false, g_useGeneric = true;
	GLuint g_squareVBO = 0, g_squareEBO = 0, g_lastVBO = 65535, g_lastEBO = 65535;
	GLuint g_squareTexturedVBO = 0;

	//----------------------------- OpenGL Extensions -----------------------------
	void LoadExtensions()
	{
		if(!g_extensionsLoaded)
		{
			glActiveTexture = (PFNGLACTIVETEXTUREPROC)glGetProcAddress(GLPROCPREFIX"glActiveTexture");
			glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)glGetProcAddress(GLPROCPREFIX"glCreateProgramObjectARB");
			glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)glGetProcAddress(GLPROCPREFIX"glCreateShaderObjectARB");
			glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)glGetProcAddress(GLPROCPREFIX"glShaderSourceARB");
			glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)glGetProcAddress(GLPROCPREFIX"glCompileShaderARB");
			glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)glGetProcAddress(GLPROCPREFIX"glAttachObjectARB");
			glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)glGetProcAddress(GLPROCPREFIX"glLinkProgramARB");
			glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)glGetProcAddress(GLPROCPREFIX"glUseProgramObjectARB");
			glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)glGetProcAddress(GLPROCPREFIX"glDeleteObjectARB");
			glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)glGetProcAddress(GLPROCPREFIX"glGetAttribLocation");
			glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glGetProcAddress(GLPROCPREFIX"glGetUniformLocation");
			glUniform1i = (PFNGLUNIFORM1IPROC)glGetProcAddress(GLPROCPREFIX"glUniform1i");
			glUniform4f = (PFNGLUNIFORM4FPROC)glGetProcAddress(GLPROCPREFIX"glUniform4f");
			glUniform4fv = (PFNGLUNIFORM4FVPROC)glGetProcAddress(GLPROCPREFIX"glUniform4fv");
			glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glGetProcAddress(GLPROCPREFIX"glUniformMatrix4fv");
			glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)glGetProcAddress(GLPROCPREFIX"glGetInfoLogARB");
			glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)glGetProcAddress(GLPROCPREFIX"glVertexAttrib1f");
			glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)glGetProcAddress(GLPROCPREFIX"glVertexAttrib4fv");
			glGetShaderiv = (PFNGLGETSHADERIVPROC)glGetProcAddress(GLPROCPREFIX"glGetShaderiv");
			glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glGetProcAddress(GLPROCPREFIX"glEnableVertexAttribArray");
			glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glGetProcAddress(GLPROCPREFIX"glDisableVertexAttribArray");
			glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glGetProcAddress(GLPROCPREFIX"glVertexAttribPointer");
			glGenBuffers = (PFNGLGENBUFFERSPROC)glGetProcAddress(GLPROCPREFIX"glGenBuffers");
			glBindBuffer = (PFNGLBINDBUFFERPROC)glGetProcAddress(GLPROCPREFIX"glBindBuffer");
			glBufferData = (PFNGLBUFFERDATAPROC)glGetProcAddress(GLPROCPREFIX"glBufferData");
			glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glGetProcAddress(GLPROCPREFIX"glDeleteBuffers");
			g_extensionsLoaded = true;
		}
	}

	//----------------------------- Rendering Control -----------------------------
	void BindVBO(unsigned int vbo)
	{
		if(g_lastVBO != vbo)
			glBindBuffer(0x8892, vbo);
	}
	void BindEBO(unsigned int ebo)
	{
		if(g_lastEBO != ebo)
			glBindBuffer(0x8893, ebo);
	}
	void ConfigGeneric()
	{
		if(g_useGeneric == false)
		{
//			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
			g_useGeneric = true;
		}
	}
	void ConfigTextured()
	{
		if(g_useGeneric == true)
		{
			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			g_useGeneric = false;
		}
	}

	//----------------------------- Rendering Primitives -----------------------------
	void RenderPrimitiveCleanup()
	{
		if(g_squareEBO)
			glDeleteBuffers(1, &g_squareEBO);
		if(g_squareVBO)
			glDeleteBuffers(1, &g_squareVBO);
		if(g_squareTexturedVBO)
			glDeleteBuffers(1, &g_squareTexturedVBO);
		if(g_defaultGenericProgram)
		{
			delete g_defaultGenericProgram->program;
			delete g_defaultGenericProgram->vertex;
			delete g_defaultGenericProgram->fragment;
			delete g_defaultGenericProgram;
		}
		if(g_defaultTexturedProgram)
		{
			delete g_defaultTexturedProgram->program;
			delete g_defaultTexturedProgram->vertex;
			delete g_defaultTexturedProgram->fragment;
			delete g_defaultTexturedProgram;
		}
	}

	void BindSquareEBO()
	{
		if(!g_squareEBO)
		{
			glGenBuffers(1, &g_squareEBO);
			BindEBO(g_squareEBO);

			unsigned int size = 6; // 2 * 3
			unsigned char *ebo = new unsigned char[size];

			ebo[0] = 0;
			ebo[1] = 1;
			ebo[2] = 2;

			ebo[3] = 0;
			ebo[4] = 2;
			ebo[5] = 3;

			glBufferData(0x8893, size, ebo, 0x88E4);
			delete[] ebo;
		}
		BindEBO(g_squareEBO);
	}

	void RenderSquare(RenderContext &context)
	{
		if(!g_squareVBO)
		{
			if(!g_extensionsLoaded)
				LoadExtensions();

			glGenBuffers(1, &g_squareVBO);
			BindVBO(g_squareVBO);

			unsigned int size = 8; // 4 * 2
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
		BindVBO(g_squareVBO);
		BindSquareEBO();

		if(context.useShaders)
		{
			int posLoc;
			if(g_lastProgram == g_genericProgram->program)
				posLoc = ShaderProgram::GetGenericProgram()->position;
			else
				posLoc = ShaderProgram::GetTexturedProgram()->position;
			glVertexAttribPointer(posLoc, 2, GL_FLOAT, 0, 8, 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		}
		else
		{
			glVertexPointer(2, GL_FLOAT, 0, 0);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		}
	}
	void RenderSquareTexturedFull(RenderContext &context)
	{
		if(!g_squareTexturedVBO)
		{
			if(!g_extensionsLoaded)
				LoadExtensions();

			glGenBuffers(1, &g_squareTexturedVBO);
			BindVBO(g_squareTexturedVBO);

			unsigned int size = 16; // 4 * 4
			float *vbo = new float[size];

			vbo[0] = 0.f;
			vbo[1] = 0.f;
			vbo[2] = 0.f;
			vbo[3] = 1.f;

			vbo[4] = 1.f;
			vbo[5] = 0.f;
			vbo[6] = 1.f;
			vbo[7] = 1.f;

			vbo[8] = 1.f;
			vbo[9] = 1.f;
			vbo[10] = 1.f;
			vbo[11] = 0.f;

			vbo[12] = 0.f;
			vbo[13] = 1.f;
			vbo[14] = 0.f;
			vbo[15] = 0.f;

			glBufferData(0x8892, size * 4, vbo, 0x88E4);
			delete[] vbo;
		}
		BindVBO(g_squareTexturedVBO);
		BindSquareEBO();

		glEnable(GL_TEXTURE_2D);
		if(context.useShaders)
		{
			ShaderProgram::TexturedProgram *program = ShaderProgram::GetTexturedProgram();
			glVertexAttribPointer(program->position, 2, GL_FLOAT, 0, 16, 0);
			glVertexAttribPointer(program->uv, 2, GL_FLOAT, 0, 16, (GLvoid *)8);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
		}
		else
		{
			glVertexPointer(2, GL_FLOAT, 16, 0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 16, (GLvoid *)8);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		glDisable(GL_TEXTURE_2D);
	}

	//----------------------------- Shader -----------------------------
	Shader *Shader::CreateFromFile(const char *file, int type)
	{
		if(!g_extensionsLoaded)
			LoadExtensions();

		if(!fileExists(file))
		{
			setError("Shader::CreateFromFile: File does not exist <%s>.", file);
			return 0;
		}

		ifstream fileStream(file);
		string fileBuffer;

		fileStream.seekg(0, ios::end);   
		fileBuffer.reserve((unsigned int)fileStream.tellg());
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
	Shader *Shader::CreateFromMemory(const char *memory, unsigned int size, int type)
	{
		if(!g_extensionsLoaded)
			LoadExtensions();

		if(!memory)
		{
			setError("Shader::CreateFromMemory: Empty memory pointer");
			return 0;
		}

		int glShaderObject = glCreateShaderObjectARB(type);

		string source(memory);

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
	int Shader::GetShaderObject() const
	{
		return m_glShaderObject;
	}
	string Shader::GetSource() const
	{
		return m_source;
	}
	int Shader::GetType() const
	{
		return m_type;
	}

	//----------------------------- ShaderProgram -----------------------------
	void ShaderProgram::SetGenericProgram(GenericProgram *program)
	{
		if(program == 0)
			g_genericProgram = g_defaultGenericProgram;
		else
			g_genericProgram = program;
	}
	ShaderProgram::GenericProgram *ShaderProgram::GetGenericProgram()
	{
		if(g_genericProgram == 0)
			if(g_defaultGenericProgram == 0)
			{
				const char *vertex = "\
					uniform mat4 mvp;\
					attribute vec2 pos;\
					void main()\
					{\
						gl_Position = mvp * vec4(pos, 0.0, 1.0);\
					}\
				";
				const char *fragment = "\
					uniform vec4 col;\
					void main()\
					{\
						gl_FragColor = col;\
					}\
				";

				g_defaultGenericProgram = new GenericProgram;
				g_defaultGenericProgram->vertex = Shader::CreateFromMemory(vertex, strlen(vertex), Shader::Vertex);
				g_defaultGenericProgram->fragment = Shader::CreateFromMemory(fragment, strlen(fragment), Shader::Fragment);

				ShaderProgram *program = new ShaderProgram;
				program->AttachShader(g_defaultGenericProgram->fragment);
				program->AttachShader(g_defaultGenericProgram->vertex);
				program->Link();
				g_defaultGenericProgram->program = program;

				program->Use();
					g_defaultGenericProgram->mvpMatrix = program->GetUniformLocation("mvp");
					g_defaultGenericProgram->position = program->GetAttribLocation("pos");
					g_defaultGenericProgram->color = program->GetUniformLocation("col");
				g_genericProgram = g_defaultGenericProgram;
			}
		return g_genericProgram;
	}
	void ShaderProgram::SetTexturedProgram(TexturedProgram *program)
	{
		if(program == 0)
			g_texturedProgram = g_defaultTexturedProgram;
		else
			g_texturedProgram = program;
	}
	ShaderProgram::TexturedProgram *ShaderProgram::GetTexturedProgram()
	{
		if(g_texturedProgram == 0)
			if(g_defaultTexturedProgram == 0)
			{
				const char *vertex = "\
					uniform mat4 mvp;\
					attribute vec2 pos;\
					attribute vec2 uv;\
					void main()\
					{\
						gl_Position = mvp * vec4(pos, 0.0, 1.0);\
						gl_TexCoord[0].st = uv;\
					}\
				";
				const char *fragment = "\
					uniform sampler2D tex;\
					void main()\
					{\
						gl_FragColor = texture2D(tex, gl_TexCoord[0].st);\
					}\
				";

				g_defaultTexturedProgram = new TexturedProgram;
				g_defaultTexturedProgram->vertex = Shader::CreateFromMemory(vertex, strlen(vertex), Shader::Vertex);
				g_defaultTexturedProgram->fragment = Shader::CreateFromMemory(fragment, strlen(fragment), Shader::Fragment);

				ShaderProgram *program = new ShaderProgram;
				program->AttachShader(g_defaultTexturedProgram->fragment);
				program->AttachShader(g_defaultTexturedProgram->vertex);
				program->Link();
				g_defaultTexturedProgram->program = program;

				program->Use();
					g_defaultTexturedProgram->mvpMatrix = program->GetUniformLocation("mvp");
					g_defaultTexturedProgram->position = program->GetAttribLocation("pos");
					g_defaultTexturedProgram->uv = program->GetAttribLocation("uv");

					//- Default texture to GL_TEXTURE0, we will probably never use another texture slot -
					glUniform1i(program->GetAttribLocation("tex"), 0);
				g_texturedProgram = g_defaultTexturedProgram;
			}
		return g_texturedProgram;
	}
	void ShaderProgram::Unuse()
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
	void ShaderProgram::AttachShader(Shader *shader)
	{
		if(shader)
			glAttachObjectARB(m_glProgramObject, shader->GetShaderObject());
	}
	int ShaderProgram::GetAttribLocation(const char *attribute)
	{
		if(!m_attribLocations.count(attribute))
			m_attribLocations[attribute] = glGetAttribLocation(m_glProgramObject, attribute);
		return m_attribLocations[attribute];
	}
	int ShaderProgram::GetProgramObject() const
	{
		return m_glProgramObject;
	}
	int ShaderProgram::GetUniformLocation(const char *uniform)
	{
		if(!m_uniformLocations.count(uniform))
			m_uniformLocations[uniform] = glGetUniformLocation(m_glProgramObject, uniform);
		return m_uniformLocations[uniform];
	}
	void ShaderProgram::Link()
	{
		glLinkProgramARB(m_glProgramObject);
		m_attribLocations.clear();
		m_uniformLocations.clear();
	}
	void ShaderProgram::Use()
	{
		glUseProgramObjectARB(m_glProgramObject);
	}

	//- Program Swap -
	ShaderProgram::GenericProgram *UseGenericProgram()
	{
		ShaderProgram::GenericProgram *program = ShaderProgram::GetGenericProgram();
		if(g_lastProgram != program->program)
		{
			if(g_texturedProgram != 0)
				if(g_lastProgram == g_texturedProgram->program)
				{
					glDisableVertexAttribArray(g_texturedProgram->position);
					glDisableVertexAttribArray(g_texturedProgram->uv);
				}
			program->program->Use();
			glEnableVertexAttribArray(program->position);
		}
		return program;
	}
	ShaderProgram::TexturedProgram *UseTexturedProgram()
	{
		ShaderProgram::TexturedProgram *program = ShaderProgram::GetTexturedProgram();
		if(g_lastProgram != program->program)
		{
			if(g_genericProgram != 0)
				if(g_lastProgram == g_genericProgram->program)
					glDisableVertexAttribArray(g_genericProgram->position);
			program->program->Use();
			glEnableVertexAttribArray(program->position);
			glEnableVertexAttribArray(program->uv);
		}
		return program;
	}
}
