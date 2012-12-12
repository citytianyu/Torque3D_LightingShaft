#ifndef _RENDERUNDERWATERSCATTERINGMGR_H
#define _RENDERUNDERWATERSCATTERINGMGR_H

#include "renderInstance/renderPostFXPrepassMgr.h"

const U32 QUADS_PER_LAYER = 18;
const U32 LAYER_NUM = 3;

class RenderUnderwaterScatteringMgr : public RenderPostFXPrepassMgr
{
	typedef RenderPostFXPrepassMgr Parent;
public:
	RenderUnderwaterScatteringMgr();
	virtual ~RenderUnderwaterScatteringMgr();

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

	ShaftInfo							ShaftInfos[LAYER_NUM];

	bool								RenderInstInitialized;
	float								LastTime;

	float								RandomNums[QUADS_PER_LAYER];
	float*								ThetaBackup;
	bool								ThetaBackupInitialized;
};

#endif _RENDERUNDERWATERSCATTERINGMGR_H