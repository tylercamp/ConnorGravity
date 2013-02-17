
#include "RenderModule.h"
#include <ORE/Diagnostics.h>

#ifdef ORE_PLATFORM_WINDOWS
#include <Windows.h>
#include <gl/GL.h>
#else
#error Unsupported platform for ORGUI RenderDefaults OpenGL.cpp!
#endif

//	For glEnable (GL_TEXTURE_2D), we use this instead of glGet to prevent the overhead
static bool g_TexturingEnabled = false;

namespace gui
{

	//	TODO: Use VBOs for rendering, right now we're doing everything in immediate mode
	namespace module
	{
		void RM_Clear (const Color & c)
		{
			glClearColor (c.r, c.g, c.b, c.a);
			glClear (GL_COLOR_BUFFER_BIT);
		}

		void RM_SetActiveRenderColor (const Color & c)
		{
			glColor4ub (c.r, c.g, c.b, c.a);
		}

		void RM_RenderPoint (const Point & p)
		{
			glBegin (GL_POINTS);
			glVertex2f (p.x, p.y);
			glEnd ();
		}

		void RM_RenderTriangle (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3)
		{
			glBegin (GL_TRIANGLES);
				glTexCoord2f (p1.tx, p1.ty);
				glVertex2f   (p1.x,  p1.y);
				glTexCoord2f (p2.tx, p2.ty);
				glVertex2f   (p2.x,  p2.y);
				glTexCoord2f (p3.tx, p3.ty);
				glVertex2f   (p3.x,  p3.y);
			glEnd ();
		}

		void RM_RenderQuad (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3, const ComplexPoint & p4)
		{
			glBegin (GL_QUADS);
				glTexCoord2f (p1.tx, p1.ty);
				glVertex2f (p1.x, p1.y);
				glTexCoord2f (p2.tx, p2.ty);
				glVertex2f (p2.x, p2.y);
				glTexCoord2f (p3.tx, p3.ty);
				glVertex2f (p3.x, p3.y);
				glTexCoord2f (p4.tx, p4.ty);
				glVertex2f (p4.x, p4.y);
			glEnd ();
		}

		void RM_RenderCircle (const Point & position, float r, float precision)
		{
			//	TODO: ... implement
		}

		void RM_PushTransformState ()
		{
			glPushMatrix ();
		}

		void RM_PopTransformState ()
		{
			glPopMatrix ();
		}

		void RM_Translate (float x, float y)
		{
			glTranslatef (x, y, 0.f);
		}

		void RM_Rotate (float z)
		{
			glRotatef (z, 0.f, 0.f, 1.f);
		}

		void RM_Scale (float x, float y)
		{
			glScalef (x, y, 0.f);
		}

		ore::t_Handle RM_CreateTextureFromPixels (int * pixels, int width, int height)
		{
			GLuint tex;
			//	TODO: Checks
			glGenTextures (1, &tex);
			glBindTexture (GL_TEXTURE_2D, tex);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexImage2D (GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			return tex;
		}

		void RM_SetCurrentTexture (ore::t_Handle texture)
		{
			if (!g_TexturingEnabled)
			{
				g_TexturingEnabled = true;
				glEnable (GL_TEXTURE_2D);
			}

			glBindTexture (GL_TEXTURE_2D, (GLuint)texture);
		}

		void RM_RenderWithoutTexture ()
		{
			if (!g_TexturingEnabled)
				return;

			g_TexturingEnabled = false;
			glDisable (GL_TEXTURE_2D);
		}

		void RM_FreeTexture (ore::t_Handle texture)
		{
			//	TODO: Checks
			GLuint texTranslaton = texture;
			glDeleteTextures (1, &texTranslaton);
		}

		void RM_BeginRender ()
		{
			//	Don't do anything yet, will most likely be used for VBO implementation
		}

		void RM_FinalizeRender ()
		{
		}

		GLuint InterpretBlendMode (t_BlendMode src)
		{
			switch (src)
			{
			case (BM_DST_ALPHA): return GL_DST_ALPHA;
			case (BM_DST_COLOR): return GL_DST_COLOR;
			case (BM_ONE): return GL_ONE;
			case (BM_ZERO): return GL_ZERO;
			case (BM_ONE_MINUS_DST_ALPHA): return GL_ONE_MINUS_DST_ALPHA;
			case (BM_ONE_MINUS_DST_COLOR): return GL_ONE_MINUS_DST_COLOR;
			case (BM_ONE_MINUS_SRC_ALPHA): return GL_ONE_MINUS_SRC_ALPHA;
			case (BM_ONE_MINUS_SRC_COLOR): return GL_ONE_MINUS_SRC_COLOR;
			case (BM_SRC_COLOR): return GL_SRC_COLOR;
			case (BM_SRC_ALPHA): return GL_SRC_ALPHA;
			default:
				{
					ore::Diagnostics::Warning (true)
						.Append ("gui::module::InterpretBlendMode - Unrecognized blend mode code ").Append (src)
						.Display ();

					return -1;
				}
			}
		}

		void RM_SetBlendMode (t_BlendMode src, t_BlendMode dst)
		{
			glBlendFunc (InterpretBlendMode (src), InterpretBlendMode (dst));
		}
	}

}