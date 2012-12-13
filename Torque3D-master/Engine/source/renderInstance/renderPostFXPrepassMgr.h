#ifndef _RENDERPOSTFXPREPASSMGR_H
#define _RENDERPOSTFXPREPASSMGR_H

#ifndef _TEXTARGETBIN_MGR_H_
#include "renderInstance/renderTexTargetBinManager.h"
#endif

#include "postFx/postEffect.h"
#include "renderInstance/renderPassManager.h"
#include "gfx/gfxPrimitiveBuffer.h"

class RenderPostFXPrepassMgr : public RenderTexTargetBinManager
{
	typedef RenderTexTargetBinManager Parent;
public:
	RenderPostFXPrepassMgr(const String& registerName = "defaultPostFXPrepassBuffer", const GFXFormat targetFormat = DefaultTargetFormat);
	virtual ~RenderPostFXPrepassMgr();

	virtual void addElement( RenderInst *inst ) {}

	DECLARE_CONOBJECT(RenderPostFXPrepassMgr);

protected:
	bool			PointInWater(Point3F& point);
	void			InitFX(const String& mainFXName, const String& blendFXName);
	void			SkipAll_NoCheck(bool skip) { MainPostFX->setSkip(skip); BlendFX->setSkip(skip); }

	// mesh property
	MeshRenderInst						MeshInst;
	GFXVertexBufferHandle<GFXVertexPT>	VertexBuffer;
	GFXPrimitiveBufferHandle			PrimitiveBuffer;
	BaseMatInstance*					MatInst;
	GFXPrimitive						PrimitiveState;

	SimObjectPtr<PostEffect>			MainPostFX;
	SimObjectPtr<PostEffect>			BlendFX;
};


#endif _RENDERPOSTFXPREPASSMGR_H