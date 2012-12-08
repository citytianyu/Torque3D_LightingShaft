#ifndef _RENDERUNDERWATERSCATTERINGMGR_H
#define _RENDERUNDERWATERSCATTERINGMGR_H

#ifndef _TEXTARGETBIN_MGR_H_
#include "renderInstance/renderTexTargetBinManager.h"
#endif

#include "postFx/postEffect.h"
#include "renderInstance/renderPassManager.h"
#include "gfx/gfxPrimitiveBuffer.h"

const U32 QUADS_PER_LAYER = 18;

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
	float								RotateInterval;
	float								RotateProcess;
	bool								Go_Back;

	float								RandomNums[QUADS_PER_LAYER];
	float*								ThetaBackup;
	bool								ThetaBackupInitialized;

	// mesh property
	GFXVertexBufferHandle<GFXVertexPT>	VertexBuffer;
	GFXPrimitiveBufferHandle			PrimitiveBuffer;
	BaseMatInstance*					MatInst;
	GFXPrimitive						PrimitiveState;

	SimObjectPtr<PostEffect>			UwScatteringFX;
};

#endif _RENDERUNDERWATERSCATTERINGMGR_H