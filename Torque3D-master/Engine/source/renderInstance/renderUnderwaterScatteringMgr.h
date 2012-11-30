#ifndef _RENDERUNDERWATERSCATTERINGMGR_H
#define _RENDERUNDERWATERSCATTERINGMGR_H

#ifndef _TEXTARGETBIN_MGR_H_
#include "renderInstance/renderTexTargetBinManager.h"
#endif

#include "renderInstance/renderPassManager.h"
#include "gfx/gfxPrimitiveBuffer.h"

class RenderUnderwaterScatteringMgr : public RenderTexTargetBinManager
{
	typedef RenderTexTargetBinManager Parent;
public:
	RenderUnderwaterScatteringMgr();
	virtual ~RenderUnderwaterScatteringMgr();

	virtual void addElement( RenderInst *inst ) {}
	virtual void render( SceneRenderState *state );

	DECLARE_CONOBJECT(RenderUnderwaterScatteringMgr);

protected:
	MeshRenderInst						ScatteringMeshRenderInst;
	bool								RenderInstInitialized;
	float								LastTime;

	// mesh property
	GFXVertexBufferHandle<GFXVertexPT>	VertexBuffer;
	GFXPrimitiveBufferHandle			PrimitiveBuffer;
	BaseMatInstance*					MatInst;
	GFXPrimitive						PrimitiveState;
};

#endif _RENDERUNDERWATERSCATTERINGMGR_H