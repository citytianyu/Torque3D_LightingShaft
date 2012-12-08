#ifndef _RENDERUNDERWATERSCATTERINGMGR_H
#define _RENDERUNDERWATERSCATTERINGMGR_H

#ifndef _TEXTARGETBIN_MGR_H_
#include "renderInstance/renderTexTargetBinManager.h"
#endif

#include "postFx/postEffect.h"
#include "renderInstance/renderPassManager.h"
#include "gfx/gfxPrimitiveBuffer.h"

const U32 QUADS_PER_LAYER = 18;
const U32 LAYER_NUM = 3;

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
	class ShaftInfo
	{
	public:
		ShaftInfo()
		{
			RotateInterval = 0.1f;
			RotateProcess = 0.f;
			RandomOffset = 0;
			RotateSpeed = 3.f;
			GoBackState = true;
		}

		void	AdvanceTime(float deltaTime) { RotateProcess += deltaTime; }
		void	OnPostRender()
		{
			if (RotateProcess > RotateInterval)
			{
				RandomOffset = ++RandomOffset % QUADS_PER_LAYER;
				GoBackState = !GoBackState;
				RotateProcess -= RotateInterval;
			}
		}

		float		RotateInterval;
		float		RotateProcess;
		U32			RandomOffset;
		float		RotateSpeed;
		bool		GoBackState;

	private:

	};

	void			GetViewRotationMatrix(const Point3F& distSun_Cam, MatrixF& viewRotationMatrix);
	bool			PointInWater(Point3F& point);

	ShaftInfo							ShaftInfos[LAYER_NUM];

	bool								RenderInstInitialized;
	float								LastTime;

	float								RandomNums[QUADS_PER_LAYER];
	float*								ThetaBackup;
	bool								ThetaBackupInitialized;

	// mesh property
	MeshRenderInst						ScatteringMeshRenderInst;
	GFXVertexBufferHandle<GFXVertexPT>	VertexBuffer;
	GFXPrimitiveBufferHandle			PrimitiveBuffer;
	BaseMatInstance*					MatInst;
	GFXPrimitive						PrimitiveState;

	SimObjectPtr<PostEffect>			UwScatteringFX;
};

#endif _RENDERUNDERWATERSCATTERINGMGR_H