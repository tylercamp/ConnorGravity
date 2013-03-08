
#pragma once

#include "OpenGL.h"
#include <ORE\Types.h>

class RenderTarget
{
public:

	RenderTarget ()
	{
		m_BufferWidth = 0;
		m_BufferHeight = 0;
		m_FrameBuffer = 0;
		m_RenderBuffer = 0;
	}

	RenderTarget (int width, int height)
	{
		m_FrameBuffer = 0;
		m_RenderBuffer = 0;

		SetSize (width, height);
	}

	void SetSize (int width, int height)
	{
		m_BufferWidth = width;
		m_BufferHeight = height;

		ConfigureBuffers ();
	}

	int GetWidth ()
	{
		return m_BufferWidth;
	}

	int GetHeight ()
	{
		return m_BufferHeight;
	}

	static void ResetRenderTarget ()
	{
		glBindFramebuffer (GL_FRAMEBUFFER, 0);
	}

	GLuint GetTexture ()
	{
		return m_RenderBuffer;
	}

	void ConfigureBuffers ()
	{
		if (m_FrameBuffer != 0)
		{
			glDeleteFramebuffers (1, &m_FrameBuffer);
			m_FrameBuffer = 0;
		}

		if (m_RenderBuffer != 0)
		{
			glDeleteTextures (1, &m_RenderBuffer);
		}

		glGenTextures (1, &m_RenderBuffer);
		ore::Generic::BindTexture (m_RenderBuffer);


		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


		ore::t_Byte * blankTexture = new ore::t_Byte [4 * m_BufferWidth * m_BufferHeight];
		memset (blankTexture, 0, 4 * m_BufferWidth * m_BufferHeight);
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, m_BufferWidth, m_BufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, blankTexture);
		delete [] blankTexture;

		glGenFramebuffers (1, &m_FrameBuffer);
		glBindFramebuffer (GL_FRAMEBUFFER, m_FrameBuffer);
		glFramebufferTexture2D (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderBuffer, 0);
		ResetRenderTarget ();
	}

	void Bind ()
	{
		glBindFramebuffer (GL_FRAMEBUFFER, m_FrameBuffer);
		//glBindBuffer (GL_FRAMEBUFFER, m_FrameBuffer);
	}

private:
	GLuint m_FrameBuffer, m_RenderBuffer;
	int m_BufferWidth, m_BufferHeight;
};