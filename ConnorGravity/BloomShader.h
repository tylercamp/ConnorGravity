
#pragma once

#include "OpenGL.h"
#include "RenderTarget.h"
#include <ORE\FileIO.h>

#include <iostream>
#include <vector>
#include <algorithm>

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

		glUseProgram (0);

		std::cout << "m_VertexShader status:\n" << CheckStatus (m_VertexShader) << std::endl;
		std::cout << "m_FragmentShader status:\n" << CheckStatus (m_FragmentShader) << std::endl;
		std::cout << "m_ShaderProgram status:\n" << CheckStatus (m_ShaderProgram) << std::endl;

		m_SourceTexture = -1;
	}

	~BloomShader ()
	{
		glDetachShader (m_ShaderProgram, m_VertexShader);
		glDetachShader (m_ShaderProgram, m_FragmentShader);

		glDeleteShader (m_VertexShader);
		glDeleteShader (m_FragmentShader);

		glDeleteProgram (m_ShaderProgram);

		FreeTargets ();
	}

	void SetSourceTexture (GLuint source)
	{
		m_SourceTexture = source;
	}

	void SetSampleCount (int samples)
	{
		FreeTargets ();

		m_Pass1Buffers.resize (samples);
		m_Pass2Buffers.resize (samples);

		std::fill (m_Pass1Buffers.begin (), m_Pass1Buffers.end (), (RenderTarget *)NULL);
		std::fill (m_Pass2Buffers.begin (), m_Pass2Buffers.end (), (RenderTarget *)NULL);
	}

	void SetKerningAccuracy (int kerningAccuracy)
	{
	}

	void ConfigureTargetsForResolution (unsigned int width, unsigned int height)
	{
		int sampleCount = m_Pass1Buffers.size ();
		if (sampleCount == 0)
			throw;

		if (width == 0 || height == 0)
			throw;

		FreeTargets ();
		SetSampleCount (sampleCount);

		for (int i = 0; i < sampleCount; i++)
		{
			if (width >> (1 + i) == 0 || height >> (1 + i) == 0) throw;

			m_Pass1Buffers [i] = new RenderTarget (width >> (1 + i), height >> (1 + i));
			m_Pass2Buffers [i] = new RenderTarget (width >> (1 + i), height >> (1 + i));
		}
	}

	void RenderBloom ()
	{
		if (m_Pass1Buffers.size () == 0) throw;

		if (m_SourceTexture == 0) throw;

		/* Preserve previous state */
		glPushAttrib (GL_TRANSFORM_BIT);

		glMatrixMode (GL_MODELVIEW);
		glPushMatrix ();
		glLoadIdentity ();

		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();
		gluOrtho2D (-1.0, 1.0, -1.0, 1.0);

		glUseProgram (0);

		/* Viewport must be preserved to render later */
		glPushAttrib (GL_VIEWPORT_BIT);

		/* Downscale */

		ore::Generic::BindTexture (m_SourceTexture);
		for (int i = 0; i < m_Pass1Buffers.size (); i++)
		{
			glViewport (0, 0, m_Pass1Buffers [i]->GetWidth (), m_Pass1Buffers [i]->GetHeight ());

			m_Pass1Buffers [i]->Bind ();
			glClear (GL_COLOR_BUFFER_BIT);

			glBegin (GL_QUADS);
				glVertex2i (-1, 1);
				glTexCoord2i (1, 0);

				glVertex2i (1, 1);
				glTexCoord2i (1, 1);

				glVertex2i (1, -1);
				glTexCoord2i (1, 0);

				glVertex2i (-1, -1);
				glTexCoord2i (0, 0);
			glEnd ();
		}

		RenderTarget::ResetRenderTarget ();

		/* Render over active view */
		glPopAttrib (); // Revert to old viewport
		for (int i = 0; i < m_Pass1Buffers.size (); i++)
		{
			ore::Generic::BindTexture (m_Pass1Buffers [i]->GetTexture ());
			glBegin (GL_QUADS);
				glVertex2i (-1, 1);
				glTexCoord2i (1, 0);

				glVertex2i (1, 1);
				glTexCoord2i (1, 1);

				glVertex2i (1, -1);
				glTexCoord2i (1, 0);

				glVertex2i (-1, -1);
				glTexCoord2i (0, 0);
			glEnd ();
		}

		/* Revert to previous state */
		glMatrixMode (GL_MODELVIEW);
		glPopMatrix ();

		glMatrixMode (GL_PROJECTION);
		glPopMatrix ();

		glPopAttrib ();
	}

private:
	GLuint m_VertexShader, m_FragmentShader, m_ShaderProgram;
	
	GLuint m_SourceTexture;

	std::vector <RenderTarget *> m_Pass1Buffers, m_Pass2Buffers;

	void FreeTargets ()
	{
		for (int i = 0; i < m_Pass1Buffers.size (); i++)
		{
			if (m_Pass1Buffers [i])
				delete m_Pass1Buffers [i];
			if (m_Pass2Buffers [i])
				delete m_Pass2Buffers [i];
		}
		
		m_Pass1Buffers.resize (0);
		m_Pass2Buffers.resize (0);
	}

	std::string CheckStatus (GLuint object)
	{
		static char infoBuffer [1000];
		infoBuffer [0] = 0;
		int infoBufferLen = 0;
		glGetInfoLogARB (object, 1000, &infoBufferLen, infoBuffer);

		return infoBuffer;
	}
};