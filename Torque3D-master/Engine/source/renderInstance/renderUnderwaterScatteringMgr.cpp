#include "platform/platform.h"
#include "renderInstance/renderUnderwaterScatteringMgr.h"

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
#include "T3D/gameBase/gameConnection.h"
#include "T3D/player.h"
#include "T3D/camera.h"

IMPLEMENT_CONOBJECT(RenderUnderwaterScatteringMgr);

ConsoleDocClass(RenderUnderwaterScatteringMgr,
	"@brief A render bin for underwater sun lighting scattering prepass.\n\n"
	"TODO comments\n\n"
	"@ingroup RenderBin\n");

RenderUnderwaterScatteringMgr::RenderUnderwaterScatteringMgr()
: RenderTexTargetBinManager(RenderPassManager::RIT_ObjectTranslucent,
							1.f,
							1.f,
							GFXFormatR8G8B8A8,
							Point2I(256, 256))
{
	mNamedTarget.registerWithName("underwaterScatteringBuffer");
	mTargetSizeType = WindowSize;

	ScatteringMeshRenderInst.clear();
	RenderInstInitialized = false;
	LastTime = 0.f;
	ShaftInfos[0].RotateInterval = 0.25f;
	ShaftInfos[1].RotateInterval = 0.2f;
	ShaftInfos[2].RotateInterval = 0.1f;

	ShaftInfos[0].RotateSpeed = 200.f;
	ShaftInfos[1].RotateSpeed = -150.f;
	ShaftInfos[2].RotateSpeed = 100.f;

	//RotateProcess[0] = RotateProcess[1] = RotateProcess[2] = 0.f;
	ThetaBackup = NULL;
	ThetaBackupInitialized = false;

	MatInst = NULL;	// TODO : Load a specific material

	MRandomR250 randGen;
	for (int i = 0; i < QUADS_PER_LAYER; ++i)
	{
		RandomNums[i] = randGen.randF() / 180.f * M_PI_F;
	}
}

RenderUnderwaterScatteringMgr::~RenderUnderwaterScatteringMgr()
{
	ScatteringMeshRenderInst.clear();
	SAFE_DELETE(MatInst);
	if (ThetaBackup)
	{
		delete[] ThetaBackup;
	}
}

inline void CalcPosition(float& x, float& y, float theta, float radius)
{
	x = radius * mCos(theta);
	y = radius * mSin(theta);
}

void RenderUnderwaterScatteringMgr::render(SceneRenderState* state)
{
	PROFILE_SCOPE(RenderUnderwaterScatteringMgr_Render);

	if (!UwScatteringFX)
		UwScatteringFX = dynamic_cast<PostEffect*>( Sim::findObject( "UwScatteringFx" ) );
	if (!UwScatteringFX)
		return;

	Point3F cameraPos = state->getCameraPosition();

	if (PointInWater(cameraPos))
	{
		UwScatteringFX->setSkip(false);
	}
	else
	{
		UwScatteringFX->setSkip(true);
		return;
	}

	RenderPassManager* renderPass = state->getRenderPass();

	// initialize the render instance resource
	// every 2 triangles make a quad
	U32 TriangleNum_PerLayer = 2 * QUADS_PER_LAYER;
	U32 TriangleCount = TriangleNum_PerLayer;
	U32 QuadCount = TriangleCount / 2;
	U32 TotalQuadCount = QuadCount * LAYER_NUM;
	U32 VertexCount = TotalQuadCount * 4;
	U32 IndexCount = TotalQuadCount * 6;
	if (!RenderInstInitialized)
	{
		LastTime = Sim::getCurrentTime();
		//MatInst = dynamic_cast<BaseMatInstance*>(Sim::findObject("TestScattering"));
		CustomMaterial *custMat;
		if ( Sim::findObject( "TestScattering", custMat ) && custMat->mShaderData )
			MatInst = custMat->createMatInstance();
		else
			MatInst = MATMGR->createMatInstance("TestScattering");

		const GFXVertexFormat* flags = getGFXVertexFormat<GFXVertexPT>();
		if (MatInst && MatInst->init(MATMGR->getDefaultFeatures(), flags))
		{
		}

		// initialize buffers
		VertexBuffer.set(GFX, VertexCount, GFXBufferTypeDynamic);
		PrimitiveBuffer.set(GFX, IndexCount, 0, GFXBufferTypeDynamic);

		{
			U16* indexBuffer;
			PrimitiveBuffer.lock(&indexBuffer);

			for (U32 i = 0; i < TotalQuadCount; ++i)
			{
				U32 indexIndex = i * 6;
				U32 vertexIndex = i * 4;
				indexBuffer[indexIndex++] = vertexIndex;
				indexBuffer[indexIndex++] = vertexIndex + 2;
				indexBuffer[indexIndex++] = vertexIndex + 1;
				indexBuffer[indexIndex++] = vertexIndex;
				indexBuffer[indexIndex++] = vertexIndex + 3;
				indexBuffer[indexIndex] = vertexIndex + 2;
			}
			
			PrimitiveBuffer.unlock();
		}

		ScatteringMeshRenderInst.objectToWorld = &MatrixF::Identity;
		ScatteringMeshRenderInst.worldToCamera = renderPass->allocSharedXform(RenderPassManager::View);
		ScatteringMeshRenderInst.projection = renderPass->allocSharedXform(RenderPassManager::Projection);

		ScatteringMeshRenderInst.type = RenderPassManager::RIT_ObjectTranslucent;
		ScatteringMeshRenderInst.vertBuff = &VertexBuffer;
		ScatteringMeshRenderInst.primBuff = &PrimitiveBuffer;
		ScatteringMeshRenderInst.matInst = MatInst;
		ScatteringMeshRenderInst.prim = &PrimitiveState;
		ScatteringMeshRenderInst.prim->type = GFXTriangleList;
		ScatteringMeshRenderInst.prim->minIndex = 0;
		ScatteringMeshRenderInst.prim->startIndex = 0;
		ScatteringMeshRenderInst.prim->numPrimitives = TotalQuadCount * 2;
		ScatteringMeshRenderInst.prim->startVertex = 0;
		ScatteringMeshRenderInst.prim->numVertices = VertexCount;
		ScatteringMeshRenderInst.defaultKey = 0;
		ScatteringMeshRenderInst.defaultKey2 = 0;

		RenderInstInitialized = true;

		if (!ThetaBackup)
		{
			ThetaBackup = new float[TotalQuadCount];
		}
	}

	// get sun pos
	LightInfo *sunLight = LIGHTMGR->getSpecialLight( LightManager::slSunLightType );
	Point3F SunPos = sunLight->getPosition();

	Point3F vecSun_Cam = SunPos - cameraPos;
	float DistSun_Cam = vecSun_Cam.len();
	
	MatrixF FinalMatrix;
	FinalMatrix.identity();

	// compute cam to sun rotation matrix
	MatrixF viewRotationMatrix;
	GetViewRotationMatrix(-vecSun_Cam, viewRotationMatrix);

	MatrixF translationMatrix;
	translationMatrix.identity();
	translationMatrix.setPosition(SunPos);

	// get advance time
	// but this is not corresponding to the time in real world
	float currentTime = Sim::getCurrentTime();
	float deltaTime = (currentTime - LastTime) / TickMs / 1000.f;

	// Tell the superclass we're about to render, preserve contents
	const bool isRenderingToTarget = _onPreRender( state, true );

	// clear
	GFX->clear(GFXClearTarget, ColorI::BLACK, 1.f, 0);

	// Restore transforms
	MatrixSet &matrixSet = getRenderPass()->getMatrixSet();
	matrixSet.restoreSceneViewProjection();

	SceneData sgData;
	sgData.init(state);

	for (U32 i = 0; i < LAYER_NUM; ++i)
	{
		ShaftInfos[i].AdvanceTime(deltaTime);
	}

	LastTime = currentTime;

	const Point3F localSunPos(0, 0, 0);
	for (U32 layerIndex = 0; layerIndex < LAYER_NUM; ++layerIndex)
	{
		float DistCam_Sun, HighPosPercentage, HighPosZ, LowPosZ;
		float ScatteringAngle;
		float tanValue, lowRadius, highRadius, angleOffset, angleStep;

		FinalMatrix.identity();

		if (layerIndex == 0)
		{
			DistCam_Sun = DistSun_Cam + 100.f;
			HighPosPercentage = 0.05f;
			HighPosZ = localSunPos.z - HighPosPercentage * DistCam_Sun;
			LowPosZ = localSunPos.z - DistCam_Sun;
			ScatteringAngle = 5.f / 180.f * M_PI_F;

			tanValue = mTan(ScatteringAngle);
			lowRadius = tanValue * DistCam_Sun;
			highRadius = lowRadius * HighPosPercentage;
			angleOffset = 10.f / 180.f * M_PI_F;		// just test
			angleStep = M_PI_F * 2.f / QuadCount;
		}

		if (layerIndex == 1)
		{
			DistCam_Sun = DistSun_Cam + 100.f;
			HighPosPercentage = 0.10f;
			HighPosZ = localSunPos.z - HighPosPercentage * DistCam_Sun;
			LowPosZ = localSunPos.z - DistCam_Sun;
			ScatteringAngle = 5.f / 180.f * M_PI_F;

			tanValue = mTan(ScatteringAngle);
			lowRadius = tanValue * DistCam_Sun;
			highRadius = lowRadius * HighPosPercentage;
			angleOffset = 13.f / 180.f * M_PI_F;		// just test
			angleStep = M_PI_F * 2.f / QuadCount;
		}

		if (layerIndex == 2)
		{
			DistCam_Sun = DistSun_Cam + 100.f;
			HighPosPercentage = 0.15f;
			HighPosZ = localSunPos.z - HighPosPercentage * DistCam_Sun;
			LowPosZ = localSunPos.z - DistCam_Sun;
			ScatteringAngle = 5.f / 180.f * M_PI_F;

			tanValue = mTan(ScatteringAngle);
			lowRadius = tanValue * DistCam_Sun;
			highRadius = lowRadius * HighPosPercentage;
			angleOffset = 15.f / 180.f * M_PI_F;		// just test
			angleStep = M_PI_F * 2.f / QuadCount;
		}

		GFXVertexPT* vertPtr = VertexBuffer.lock();

		for (U32 i = 0; i < QuadCount; i++)
		{
			float x, y, theta;
			U32 backupIndex = layerIndex * QuadCount + i;
			const ShaftInfo& info = ShaftInfos[layerIndex];

			if (!ThetaBackupInitialized)
			{
				ThetaBackup[backupIndex] = i * angleStep;
			}

			U32 curRanIndex = (info.RandomOffset + i) % QUADS_PER_LAYER;

			if (info.GoBackState)
			{
				theta = ThetaBackup[backupIndex] + deltaTime * info.RotateSpeed * RandomNums[curRanIndex];
			}
			else
			{
				theta = ThetaBackup[backupIndex] - deltaTime * info.RotateSpeed * RandomNums[curRanIndex];
			}

			ThetaBackup[backupIndex] = theta;

			CalcPosition(x, y, theta, highRadius);

			U32 vertIndex = i * 4 + layerIndex * QuadCount * 4;
			vertPtr[vertIndex].point.set(localSunPos.x + x, localSunPos.y + y, HighPosZ);
			vertPtr[vertIndex++].texCoord.set(0, 0);

			CalcPosition(x, y, theta, lowRadius);
			vertPtr[vertIndex].point.set(localSunPos.x + x, localSunPos.y + y, LowPosZ);
			vertPtr[vertIndex++].texCoord.set(0.f, 1.f);

			theta -= angleOffset;
			CalcPosition(x, y, theta, lowRadius);
			vertPtr[vertIndex].point.set(localSunPos.x + x, localSunPos.y + y, LowPosZ);
			vertPtr[vertIndex++].texCoord.set(1.f, 1.f);

			CalcPosition(x, y, theta, highRadius);
			vertPtr[vertIndex].point.set(localSunPos.x + x, localSunPos.y + y, HighPosZ);
			vertPtr[vertIndex].texCoord.set(1.f, 0.f);
		}

		VertexBuffer.unlock();
	}

	FinalMatrix = translationMatrix * viewRotationMatrix;

	setupSGData(&ScatteringMeshRenderInst, sgData);

	while (ScatteringMeshRenderInst.matInst->setupPass(state, sgData))
	{
		matrixSet.setWorld(FinalMatrix);
		matrixSet.setView(*ScatteringMeshRenderInst.worldToCamera);
		matrixSet.setProjection(*ScatteringMeshRenderInst.projection);
		ScatteringMeshRenderInst.matInst->setTransforms(matrixSet, state);
		ScatteringMeshRenderInst.matInst->setSceneInfo(state, sgData);
		ScatteringMeshRenderInst.matInst->setBuffers(ScatteringMeshRenderInst.vertBuff, ScatteringMeshRenderInst.primBuff);

		if (ScatteringMeshRenderInst.prim)
			GFX->drawPrimitive(*ScatteringMeshRenderInst.prim);
		else
			GFX->drawPrimitive(ScatteringMeshRenderInst.primBuffIndex);
	}

	// Finish up.
	if ( isRenderingToTarget )
		_onPostRender();

	if (!ThetaBackupInitialized)
	{
		ThetaBackupInitialized = true;
	}

	for (U32 i = 0; i < LAYER_NUM; ++i)
	{
		ShaftInfos[i].OnPostRender();
	}
}

void RenderUnderwaterScatteringMgr::GetViewRotationMatrix(const Point3F& distSun_Cam, MatrixF& viewRotationMatrix)
{
	viewRotationMatrix.identity();

	Point3F dirSun_Cam = distSun_Cam;
	dirSun_Cam.normalize();

	float lengthSun_Cam = dirSun_Cam.len();
	float lengthSun_Cam_XY = Point2F(dirSun_Cam.x, dirSun_Cam.y).len();

	float eulerSun_Cam_X, eulerSun_Cam_Z;
	if (fabs(lengthSun_Cam) > 0.001f)
	{
		eulerSun_Cam_X = mAsin(dirSun_Cam.z / lengthSun_Cam);
	}
	else
		eulerSun_Cam_X = 0.f;
	eulerSun_Cam_X = -eulerSun_Cam_X;

	if (fabs(lengthSun_Cam_XY) > 0.001f)
	{
		eulerSun_Cam_Z = mAcos(dirSun_Cam.x / lengthSun_Cam_XY);
		if (dirSun_Cam.y < 0)
			eulerSun_Cam_Z = M_PI_F * 2.f - eulerSun_Cam_Z;
	}
	else
		eulerSun_Cam_Z = 0.f;

	eulerSun_Cam_Z = -eulerSun_Cam_Z;


	viewRotationMatrix *= MatrixF(EulerF(0, 0, eulerSun_Cam_Z));
	viewRotationMatrix *= MatrixF(EulerF(0, 0, M_PI_F / 2.f));
	viewRotationMatrix *= MatrixF(EulerF(eulerSun_Cam_X, 0, 0));
	viewRotationMatrix *= MatrixF(EulerF(-M_PI_F / 2.f, 0, 0));
}

bool RenderUnderwaterScatteringMgr::PointInWater(Point3F& point)
{
	GameConnection* connection = GameConnection::getLocalClientConnection();
	if( connection )
	{
		GameBase* base = connection->getControlObject();
		Player* player = dynamic_cast<Player*>(base);
		Camera* camera = dynamic_cast<Camera*>(base);
		if(player && player->pointInWater(point))
			return true;
		else if (camera && camera->pointInWater(point))
			return true;
	}

	return false;
}