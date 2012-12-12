#ifndef _RENDERWATERGLOWMGR_H
#define _RENDERWATERGLOWMGR_H

#include "renderInstance/renderPostFXPrepassMgr.h"

class RenderWaterGlowMgr : public RenderPostFXPrepassMgr
{
	typedef RenderPostFXPrepassMgr Parent;
public:
	RenderWaterGlowMgr();
	virtual ~RenderWaterGlowMgr();

	virtual void render(SceneRenderState* state);

	DECLARE_CONOBJECT(RenderWaterGlowMgr);

protected:

	bool								RenderInstInitialized;
};

#endif _RENDERWATERGLOWMGR_H