
#include "RenderModule.h"
#include "Hash.h"

namespace gui
{

	using namespace module;

	RenderModule::RenderModule ()
	{
		OverloadFunction (RM_CLEAR,						MODULE_FUNCTION (RM_Clear));
		OverloadFunction (RM_SET_ACTIVE_RENDER_COLOR,	MODULE_FUNCTION (RM_SetActiveRenderColor));
		OverloadFunction (RM_RENDER_POINT,				MODULE_FUNCTION (RM_RenderPoint));
		OverloadFunction (RM_RENDER_TRIANGLE,			MODULE_FUNCTION (RM_RenderTriangle));
		OverloadFunction (RM_RENDER_QUAD,				MODULE_FUNCTION (RM_RenderQuad));
		OverloadFunction (RM_RENDER_CIRCLE,				MODULE_FUNCTION (RM_RenderCircle));
		OverloadFunction (RM_PUSH_TRANSFORM_STATE,		MODULE_FUNCTION (RM_PushTransformState));
		OverloadFunction (RM_POP_TRANSFORM_STATE,		MODULE_FUNCTION (RM_PopTransformState));
		OverloadFunction (RM_TRANSLATE,					MODULE_FUNCTION (RM_Translate));
		OverloadFunction (RM_ROTATE,					MODULE_FUNCTION (RM_Rotate));
		OverloadFunction (RM_SCALE,						MODULE_FUNCTION (RM_Scale));
		OverloadFunction (RM_BEGIN_RENDER,				MODULE_FUNCTION (RM_BeginRender));
		OverloadFunction (RM_FINALIZE_RENDER,			MODULE_FUNCTION (RM_FinalizeRender));
		OverloadFunction (RM_CREATE_TEXTURE_FROM_PIXELS,MODULE_FUNCTION (RM_CreateTextureFromPixels));
		OverloadFunction (RM_SET_CURRENT_TEXTURE,		MODULE_FUNCTION (RM_SetCurrentTexture));
		OverloadFunction (RM_RENDER_WITHOUT_TEXTURE,	MODULE_FUNCTION (RM_RenderWithoutTexture));
		OverloadFunction (RM_FREE_TEXTURE,				MODULE_FUNCTION (RM_FreeTexture));
		OverloadFunction (RM_SET_BLEND_MODE,			MODULE_FUNCTION (RM_SetBlendMode));
	}

	RenderModule::~RenderModule ()
	{
		for (auto iter = m_Textures.begin (); iter != m_Textures.end (); iter++)
		{
			((void (*)(ore::t_Handle))GetFunction (RM_FREE_TEXTURE)) (iter->second->texture);
		}
	}

	RenderModule * RenderModule::GetRenderModule ()
	{
		static RenderModule * renderModule = NULL;
		if (!renderModule)
		{
			renderModule = new RenderModule;
		}
		return renderModule;
	}

	void RenderModule::Clear (const Color & c)
	{
		((void (*)(const Color &))GetFunction (RM_CLEAR)) (c);
	}

	void RenderModule::SetActiveRenderColor (const Color & c)
	{
		((void (*)(const Color &))GetFunction (RM_SET_ACTIVE_RENDER_COLOR)) (c);
	}

	void RenderModule::RenderPoint (const Point & p)
	{
		((void (*)(const Point &))GetFunction (RM_RENDER_POINT)) (p);
	}

	void RenderModule::RenderTriangle (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3)
	{
		((void (*)(const ComplexPoint &, const ComplexPoint &, const ComplexPoint &))GetFunction (RM_RENDER_TRIANGLE)) (p1, p2, p3);
	}

	void RenderModule::RenderQuad (const ComplexPoint & p1, const ComplexPoint & p2, const ComplexPoint & p3, const ComplexPoint & p4)
	{
		((void (*)(const ComplexPoint &, const ComplexPoint &, const ComplexPoint &, const ComplexPoint &))GetFunction (RM_RENDER_QUAD)) (p1, p2, p3, p4);
	}

	void RenderModule::RenderCircle (const Point & p, float r, float precision)
	{
		((void (*)(const Point &,float,float))GetFunction (RM_RENDER_CIRCLE)) (p, r, precision);
	}

	void RenderModule::PushTransformState ()
	{
		((void (*)())GetFunction (RM_PUSH_TRANSFORM_STATE)) ();
	}

	void RenderModule::PopTransformState ()
	{
		((void (*)())GetFunction (RM_POP_TRANSFORM_STATE)) ();
	}

	void RenderModule::Translate (float x, float y)
	{
		((void (*)(float,float))GetFunction (RM_TRANSLATE)) (x, y);
	}

	void RenderModule::Rotate (float z)
	{
		((void (*)(float))GetFunction (RM_ROTATE)) (z);
	}

	void RenderModule::Scale (float x, float y)
	{
		((void (*)(float,float))GetFunction (RM_SCALE)) (x, y);
	}

	ore::t_UInt64 RenderModule::CreateTexture (int * pixels, int width, int height, ore::t_String textureName)
	{
		ore::t_UInt64 hash = Hash (textureName);
		//	TODO: Existing texture checks

		Texture * newTexture = new Texture;
		newTexture->w = width;
		newTexture->h = height;
		newTexture->texture = ((ore::t_Handle (*)(int *, int, int))GetFunction (RM_CREATE_TEXTURE_FROM_PIXELS)) (pixels, width, height);

		m_Textures [hash] = newTexture;

		return hash;
	}

	bool RenderModule::TextureExists (ore::t_String textureName)
	{
		return TextureExists (Hash (textureName));
	}

	bool RenderModule::TextureExists (ore::t_UInt64 textureId)
	{
		return m_Textures.find (textureId) != m_Textures.end ();
	}

	void RenderModule::GetTextureDimensions (ore::t_String textureName, int * w, int * h)
	{
		//	TODO: Existing texture checks
		Texture * tex = m_Textures [Hash (textureName)];
		if (w)
			*w = tex->w;
		if (h)
			*h = tex->h;
	}

	ore::t_UInt64 RenderModule::GetTextureID (ore::t_String textureName)
	{
		return Hash (textureName);
	}

	void RenderModule::SetCurrentTexture (ore::t_UInt64 textureId)
	{
		//	TODO: Existing texture checks
		((void (*)(ore::t_Handle))GetFunction (RM_SET_CURRENT_TEXTURE)) (m_Textures [textureId]->texture);
	}

	void RenderModule::SetCurrentTexture (ore::t_String texture)
	{
		SetCurrentTexture (Hash (texture));
	}

	void RenderModule::RenderWithoutTexturing ()
	{
		((void (*)())GetFunction (RM_RENDER_WITHOUT_TEXTURE)) ();
	}

	void RenderModule::BeginRender ()
	{
		((void (*)())GetFunction (RM_BEGIN_RENDER)) ();
	}

	void RenderModule::FinalizeRender ()
	{
		((void (*)())GetFunction (RM_FINALIZE_RENDER)) ();
	}

	void RenderModule::SetBlendMode (t_BlendMode srcMode, t_BlendMode dstMode)
	{
		((void (*)(t_BlendMode, t_BlendMode))GetFunction (RM_SET_BLEND_MODE)) (srcMode, dstMode);
	}
}