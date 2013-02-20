
#pragma once

#include "OpenGL.h"
#include <ORE\FileIO.h>

#include <iostream>

class BloomShader
{
public:
	BloomShader ()
	{
		ore::t_UInteger vertexSize;
		ore::t_Byte * vertexData;
		vertexData = ore::FileIO::GetFileContents ("vertex.vert", 0, &vertexSize);

		ore::t_UInteger fragmentSize;
		ore::t_Byte * fragmentData;
		fragmentData = ore::FileIO::GetFileContents ("fragment.frag", 0, &fragmentSize);

		m_VertexShader = glCreateShader (GL_VERTEX_SHADER);
		m_FragmentShader = glCreateShader (GL_FRAGMENT_SHADER);

		glShaderSource (m_VertexShader, 1, (const GLchar **)&vertexData, (GLint *)&vertexSize);
		glShaderSource (m_FragmentShader, 1, (const GLchar **)&fragmentData, (GLint *)&fragmentSize);

		glCompileShader (m_VertexShader);
		glCompileShader (m_FragmentShader);

		delete [] fragmentData;
		delete [] vertexData;

		m_ShaderProgram = glCreateProgram ();
		glAttachShader (m_ShaderProgram, m_VertexShader);
		glAttachShader (m_ShaderProgram, m_FragmentShader);

		glLinkProgram (m_ShaderProgram);

		std::cout << "m_VertexShader status:\n" << CheckStatus (m_VertexShader) << std::endl;
		std::cout << "m_FragmentShader status:\n" << CheckStatus (m_FragmentShader) << std::endl;
		std::cout << "m_ShaderProgram status:\n" << CheckStatus (m_ShaderProgram) << std::endl;
	}

	~BloomShader ()
	{
		glDetachShader (m_ShaderProgram, m_VertexShader);
		glDetachShader (m_ShaderProgram, m_FragmentShader);

		glDeleteShader (m_VertexShader);
		glDeleteShader (m_FragmentShader);

		glDeleteProgram (m_ShaderProgram);
	}

	void Bind ()
	{
		glUseProgram (m_ShaderProgram);
	}

private:
	GLuint m_VertexShader, m_FragmentShader, m_ShaderProgram;

	std::string CheckStatus (GLuint object)
	{
		static char infoBuffer [1000];
		infoBuffer [0] = 0;
		int infoBufferLen = 0;
		glGetInfoLogARB (object, 1000, &infoBufferLen, infoBuffer);

		return infoBuffer;
	}
};