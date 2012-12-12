#include "platform/platform.h"
#include "renderInstance/renderWaterGlowMgr.h"

#include "scene/sceneManager.h"
#include "scene/sceneRenderState.h"
#include "materials/sceneData.h"
#include "materials/matInstance.h"
#include "materials/materialFeatureTypes.h"
#include "materials/processedMaterial.h"
//#include "postFx/postEffect.h"
#include "gfx/gfxTransformSaver.h"
#include "gfx/gfxDebugEvent.h"
#include "math/util/matrixSet.h"
#include "materials/materialParameters.h"
#include "materials/baseMatInstance.h"
#include "materials/materialManager.h"
#include "materials/customMaterialDefinition.h"

IMPLEMENT_CONOBJECT(RenderWaterGlowMgr);

ConsoleDocClass(RenderWaterGlowMgr,
				"@brief A render bin for water glow prepass.\n\n"
				"TODO comments\n\n"
				"@ingroup RenderBin\n");

RenderWaterGlowMgr::RenderWaterGlowMgr()
: RenderPostFXPrepassMgr("waterGlowBuffer",
							GFXFormatR8G8B8A8)
{
	mTargetSizeType = WindowSizeScaled;
	mTargetScale.set(0.5f, 0.5f);

	RenderInstInitialized = false;
}

RenderWaterGlowMgr::~RenderWaterGlowMgr()
{

}

void RenderWaterGlowMgr::render(SceneRenderState* state)
{
	PROFILE_SCOPE(RenderWaterGlowMgr_Render);

	if (!MainPostFX)
		MainPostFX = dynamic_cast<PostEffect*>( Sim::findObject( "WaterGlowFx" ) );
	if (!MainPostFX)
		return;

	// TODO : culled in the water
	//Point3F cameraPos = state->getCameraPosition();

	//if (!PointInWater(cameraPos))
	//{
	//	MainPostFX->setSkip(false);
	//}
	//else
	//{
	//	MainPostFX->setSkip(true);
	//	return;
	//}

	//RenderPassManager* renderPass = state->getRenderPass();

	if (!RenderInstInitialized)
	{
		U32 TriangleCount = 2;
		U32 VertexCount = 4;
		U32 IndexCount = 6;

		CustomMaterial *custMat;
		if ( Sim::findObject( "WaterGlowMat", custMat ) && custMat->mShaderData )
			MatInst = custMat->createMatInstance();
		else
			MatInst = MATMGR->createMatInstance("WaterGlowMat");

		const GFXVertexFormat* flags = getGFXVertexFormat<GFXVertexPT>();
		if (MatInst && MatInst->init(MATMGR->getDefaultFeatures(), flags))
		{
		}

		// initialize buffers
		VertexBuffer.set(GFX, VertexCount, GFXBufferTypeStatic);
		PrimitiveBuffer.set(GFX, IndexCount, 0, GFXBufferTypeStatic);

		{
			U16* indexBuffer;
			PrimitiveBuffer.lock(&indexBuffer);

			U32 indexIndex = 0;
			U32 vertexIndex = 0;
			indexBuffer[indexIndex++] = vertexIndex;
			indexBuffer[indexIndex++] = vertexIndex + 2;
			indexBuffer[indexIndex++] = vertexIndex + 1;
			indexBuffer[indexIndex++] = vertexIndex;
			indexBuffer[indexIndex++] = vertexIndex + 3;
			indexBuffer[indexIndex] = vertexIndex + 2;

			PrimitiveBuffer.unlock();
		}

		{
			GFXVertexPT* vertPtr = VertexBuffer.lock();

			U32 vertIndex = 0;
			vertPtr[vertIndex].point.set(-1, 1, 1);
			vertPtr[vertIndex++].texCoord.set(0, 0);

			vertPtr[vertIndex].point.set(-1, -1, 1);
			vertPtr[vertIndex++].texCoord.set(0.f, 1.f);

			vertPtr[vertIndex].point.set(1, -1, 1);
			vertPtr[vertIndex++].texCoord.set(1.f, 1.f);

			vertPtr[vertIndex].point.set(1, 1, 1);
			vertPtr[vertIndex].texCoord.set(1.f, 0.f);

			VertexBuffer.unlock();
		}

		MeshInst.objectToWorld = &MatrixF::Identity;
		MeshInst.worldToCamera = &MatrixF::Identity;
		MeshInst.projection = &MatrixF::Identity;

		MeshInst.type = RenderPassManager::RIT_ObjectTranslucent;
		MeshInst.vertBuff = &VertexBuffer;
		MeshInst.primBuff = &PrimitiveBuffer;
		MeshInst.matInst = MatInst;
		MeshInst.prim = &PrimitiveState;
		MeshInst.prim->type = GFXTriangleList;
		MeshInst.prim->minIndex = 0;
		MeshInst.prim->startIndex = 0;
		MeshInst.prim->numPrimitives = TriangleCount;
		MeshInst.prim->startVertex = 0;
		MeshInst.prim->numVertices = VertexCount;
		MeshInst.defaultKey = 0;
		MeshInst.defaultKey2 = 0;

		RenderInstInitialized = true;
	}

	// Tell the superclass we're about to render, preserve contents
	const bool isRenderingToTarget = _onPreRender( state, true );

	// clear
	GFX->clear(GFXClearTarget, ColorI::BLACK, 1.f, 0);

	// Restore transforms
	MatrixSet &matrixSet = getRenderPass()->getMatrixSet();
	matrixSet.restoreSceneViewProjection();

	SceneData sgData;
	sgData.init(state);

	setupSGData(&MeshInst, sgData);

	while (MeshInst.matInst->setupPass(state, sgData))
	{
		matrixSet.setWorld(*MeshInst.objectToWorld);
		matrixSet.setView(*MeshInst.worldToCamera);
		matrixSet.setProjection(*MeshInst.projection);
		MeshInst.matInst->setTransforms(matrixSet, state);
		MeshInst.matInst->setSceneInfo(state, sgData);
		MeshInst.matInst->setBuffers(MeshInst.vertBuff, MeshInst.primBuff);

		if (MeshInst.prim)
			GFX->drawPrimitive(*MeshInst.prim);
		else
			GFX->drawPrimitive(MeshInst.primBuffIndex);
	}

	// Finish up.
	if ( isRenderingToTarget )
		_onPostRender();
}