#ifndef _CE_BASE_SHADER_H_
#define _CE_BASE_SHADER_H_

//- Standard Library -
#include <string>
#include <map>

namespace ce
{
	class Shader
	{
		int m_type, m_glShaderObject;
		std::string m_source;

	public:
		enum Types
		{
			Fragment = 0x8B30,
			Vertex = 0x8B31
		};

		static Shader *CreateFromFile(const char *file, int type);
		static Shader *CreateFromMemory(const char *memory, unsigned int size, int type);

		~Shader();

		int GetShaderObject() const;
		std::string GetSource() const;
		int GetType() const;
	};

	class ShaderProgram
	{
		int m_glProgramObject;
		std::map<std::string, int> m_attribLocations, m_uniformLocations;

	public:
		static void Unuse();

		ShaderProgram();
		~ShaderProgram();

		void AttachShader(Shader *shader);
		int GetAttribLocation(const char *attribute);
		int GetProgramObject() const;
		int GetUniformLocation(const char *uniform);
		void Link();
		void Use();

		typedef struct GenericProgram
		{
			Shader *vertex, *fragment;
			ShaderProgram *program;
			int mvpMatrix, position, color;
		} GenericProgram;

		typedef struct TexturedProgram
		{
			Shader *vertex, *fragment;
			ShaderProgram *program;
			int mvpMatrix, position, color, uv;
		} TexturedProgram;

		//- Generic Rendering -
		static void SetGenericProgram(GenericProgram *program);
		static GenericProgram *GetGenericProgram();

		//- Textured Rendering -
		static void SetTexturedProgram(TexturedProgram *program);
		static TexturedProgram *GetTexturedProgram();
	};
}

#endif
