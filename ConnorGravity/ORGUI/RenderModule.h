
#ifndef _ORGUI_RENDER_MODULE_H_
#define _ORGUI_RENDER_MODULE_H_

//	Manages lower-level, primitive rendering

#include "Module.h"
#include "color.h"
#include "Point.h"

namespace gui
{
	enum t_BlendMode
	{
		BM_ZERO,
		BM_ONE,
		BM_SRC_ALPHA,
		BM_DST_ALPHA,
		BM_ONE_MINUS_DST_ALPHA,
		BM_ONE_MINUS_SRC_ALPHA,
		BM_SRC_COLOR,
		BM_ONE_MINUS_SRC_COLOR,
		BM_DST_COLOR,
		BM_ONE_MINUS_DST_COLOR
	};

	namespace module
	{
		const t_ModuleFunctionID
			RM_CLEAR					= 0,
			RM_SET_ACTIVE_RENDER_COLOR	= 1,
			RM_RENDER_POINT				= 2,
			RM_RENDER_TRIANGLE			= 3,
			RM_RENDER_QUAD				= 4,
			RM_RENDER_CIRCLE			= 5,
			RM_PUSH_TRANSFORM_STATE		= 6,
			RM_POP_TRANSFORM_STATE		= 7,
			RM_TRANSLATE				= 8,
			RM_ROTATE					= 9,
			RM_SCALE					= 10,
			RM_BEGIN_RENDER				= 11,
			RM_FINALIZE_RENDER			= 12,
			RM_CREATE_TEXTURE_FROM_PIXELS = 13,
			RM_SET_CURRENT_TEXTURE		= 14,
			RM_RENDER_WITHOUT_TEXTURE	= 15,
			RM_FREE_TEXTURE				= 16,
			RM_SET_BLEND_MODE			= 17;

		void RM_Clear (const Color & c);
		void RM_SetActiveRenderColor (const Color & c);

		void RM_RenderPoint (const Point & p);
		void RM_RenderTriangle (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3);

		void RM_RenderQuad (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3, const ComplexPoint & p4);
		void RM_RenderCircle (const Point & position, float r, float precision);

		void RM_PushTransformState ();
		void RM_PopTransformState ();
		void RM_Translate (float x, float y);
		void RM_Rotate (float z);
		void RM_Scale (float x, float y);

		//	Requires that pixels be in 32-bit, RGBA format.
		ore::t_Handle RM_CreateTextureFromPixels (int * pixels, int width, int height);
		void RM_SetCurrentTexture (ore::t_Handle texture);
		void RM_RenderWithoutTexture ();
		void RM_FreeTexture (ore::t_Handle texture);

		void RM_SetBlendMode (t_BlendMode srcMode, t_BlendMode dstMode);

		void RM_BeginRender ();
		void RM_FinalizeRender ();

		struct Texture;
	}

	//	Only maintains rendering functions. This does not maintain the window, nor does it maintain
	//		the actual contexts used for rendering. 
	class RenderModule
		: public Module
	{
	public:
		//	Configures for the default OpenGL rendering functions for now
		RenderModule ();
		~RenderModule ();

		static RenderModule * GetRenderModule ();

		void Clear (const Color & c);
		void SetActiveRenderColor (const Color & c);
		void RenderPoint (const Point & p);
		void RenderTriangle (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3);
		void RenderQuad (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3, const ComplexPoint & p4);
		void RenderCircle (const Point & position, float r, float precision);


		void PushTransformState ();
		void PopTransformState ();
		void Translate (float x, float y);
		void Rotate (float z);
		void Scale (float x, float y);


		ore::t_UInt64  CreateTexture (int * pixels, int width, int height, ore::t_String textureName);

		bool TextureExists (ore::t_String textureName);
		bool TextureExists (ore::t_UInt64 textureId);

		void GetTextureDimensions (ore::t_String textureName, int * width, int * height);

		ore::t_UInt64  GetTextureID (ore::t_String textureName);

		void SetCurrentTexture (ore::t_String textureName);
		void SetCurrentTexture (ore::t_UInt64 textureId);

		void RenderWithoutTexturing ();

		void SetBlendMode (t_BlendMode srcMode, t_BlendMode dstMode);

		void BeginRender ();
		void FinalizeRender ();

	private:
		std::map <ore::t_UInt64, module::Texture *> m_Textures;
	};

	namespace module
	{
		struct Texture
		{
			ore::t_Handle texture;
			int w, h;
		};
	}
}

#endif