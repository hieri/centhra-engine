#ifndef CE_SHADER_H
#define CE_SHADER_H

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

		static Shader *createFromFile(const char *file, int type);
		static Shader *createFromMemory(const unsigned char *memory, unsigned int size, int type);

		~Shader();

		int getShaderObject() const;
		std::string getSource() const;
		int getType() const;
	};

	class ShaderProgram
	{
		int m_glProgramObject;
		std::map<std::string, int> m_attribLocations, m_uniformLocations;

	public:
		static void disableVertexAttribArray(int attrib);
		static void enableVertexAttribArray(int attrib);
		static void uniformMatrix4fv(int uniform, int a, int b, float *c);
		static void unuse();
		static void vertexAttribPointer(int attrib, int a, int b, int c, int d, void *e);

		ShaderProgram();
		~ShaderProgram();

		void attachShader(Shader *shader);
		int getAttribLocation(const char *attribute);
		int getProgramObject() const;
		int getUniformLocation(const char *uniform);
		void link();
		void use();
	};
}

#endif
