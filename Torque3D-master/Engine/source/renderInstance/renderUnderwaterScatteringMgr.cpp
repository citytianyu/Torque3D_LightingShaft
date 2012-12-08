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
	RotateInterval = 0.1f;
	RotateProcess = 0.f;
	ThetaBackup = NULL;
	ThetaBackupInitialized = false;
	Go_Back =  true;
	MatInst = NULL;	// TODO : Load a specific material

	MRandomR250 randGen;
	for (int i = 0; i < QUADS_PER_LAYER; ++i)
	{
		RandomNums[i] = randGen.randF() * 5 / 180.f * M_PI_F;
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

#define SKIP_THIS_EFFECT			\
	UwScatteringFX->setSkip(true);	\
	return;

	GameConnection* connection = GameConnection::getLocalClientConnection();
	if( connection )
	{
		GameBase* base = connection->getControlObject();
		Player* player = dynamic_cast<Player*>(base);
		Camera* camera = dynamic_cast<Camera*>(base);
		if(player && player->pointInWater(cameraPos))
			UwScatteringFX->setSkip(false);
		else if (camera && camera->pointInWater(cameraPos))
			UwScatteringFX->setSkip(false);
		else
		{
			SKIP_THIS_EFFECT;
		}
	}
	else
	{
		SKIP_THIS_EFFECT;
	}

	RenderPassManager* renderPass = state->getRenderPass();

	// initialize the render instance resource
	// every 2 triangles make a quad
	U32 TriangleNum_PerLayer = 2 * QUADS_PER_LAYER;
	U32 LayerNum = 3;
	U32 TriangleCount = TriangleNum_PerLayer;
	U32 QuadCount = TriangleCount / 2;
	U32 TotalQuadCount = QuadCount * LayerNum;
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

	float OrgDistSun_Cam = (SunPos - cameraPos).len();
	
	MatrixF FinalMatrix;
	FinalMatrix.identity();
	// compute cam to sun rotation matrix
	MatrixF viewRotationMatrix;
	viewRotationMatrix.identity();
	{
		Point3F dirSun_Cam = cameraPos - SunPos;
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


	MatrixF translationMatrix;
	translationMatrix.identity();
	translationMatrix.setPosition(SunPos);

	MatrixF rotationMatrix;

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

	static U32 RandomOffset = 0;
	RotateProcess += deltaTime;
	LastTime = currentTime;

	const Point3F localSunPos(0, 0, 0);
	for (U32 layerIndex = 0; layerIndex < LayerNum; ++layerIndex)
	{
		float DistCam_Sun, HighPosPercentage, HighPosZ, LowPosZ;
		float ScatteringAngle, /*rotation,*/ RotationSpeed;
		float tanValue, lowRadius, highRadius, angleOffset, angleStep;
		U32 randomIndex;

		RotationSpeed = 0.f;
		rotationMatrix.identity();
		FinalMatrix.identity();

		if (layerIndex == 0)
		{
			DistCam_Sun = OrgDistSun_Cam + 100.f;
			HighPosPercentage = 0.05f;
			HighPosZ = localSunPos.z - HighPosPercentage * DistCam_Sun;
			LowPosZ = localSunPos.z - DistCam_Sun;
			ScatteringAngle = 5.f / 180.f * M_PI_F;

			RotationSpeed = 100.f;
			//rotation = deltaTime * RotationSpeed / 180.f * M_PI_F;
			//rotationMatrix = MatrixF(EulerF(0, 0, rotation));

			tanValue = mTan(ScatteringAngle);
			lowRadius = tanValue * DistCam_Sun;
			highRadius = lowRadius * HighPosPercentage;
			angleOffset = 10.f / 180.f * M_PI_F;		// just test
			angleStep = M_PI_F * 2.f / QuadCount;

			randomIndex = RandomOffset;
		}

		if (layerIndex == 1)
		{
			DistCam_Sun = OrgDistSun_Cam + 100.f;
			HighPosPercentage = 0.10f;
			HighPosZ = localSunPos.z - HighPosPercentage * DistCam_Sun;
			LowPosZ = localSunPos.z - DistCam_Sun;
			ScatteringAngle = 5.f / 180.f * M_PI_F;

			RotationSpeed = -50.f;
			//rotation = deltaTime * RotationSpeed / 180.f * M_PI_F;
			//rotationMatrix = MatrixF(EulerF(0, 0, rotation));

			tanValue = mTan(ScatteringAngle);
			lowRadius = tanValue * DistCam_Sun;
			highRadius = lowRadius * HighPosPercentage;
			angleOffset = 13.f / 180.f * M_PI_F;		// just test
			angleStep = M_PI_F * 2.f / QuadCount;

			randomIndex = RandomOffset + 1;
		}

		if (layerIndex == 2)
		{
			DistCam_Sun = OrgDistSun_Cam + 100.f;
			HighPosPercentage = 0.15f;
			HighPosZ = localSunPos.z - HighPosPercentage * DistCam_Sun;
			LowPosZ = localSunPos.z - DistCam_Sun;
			ScatteringAngle = 5.f / 180.f * M_PI_F;

			RotationSpeed = +20.f;
			//rotation = deltaTime * RotationSpeed / 180.f * M_PI_F;
			//rotationMatrix = MatrixF(EulerF(0, 0, rotation)); 

			tanValue = mTan(ScatteringAngle);
			lowRadius = tanValue * DistCam_Sun;
			highRadius = lowRadius * HighPosPercentage;
			angleOffset = 15.f / 180.f * M_PI_F;		// just test
			angleStep = M_PI_F * 2.f / QuadCount;

			randomIndex = RandomOffset + 2;
		}

		GFXVertexPT* vertPtr = VertexBuffer.lock();

		for (U32 i = 0; i < QuadCount; i++)
		{
			float x, y, theta;
			U32 backupIndex = layerIndex * QuadCount + i;

			if (!ThetaBackupInitialized)
			{
				ThetaBackup[backupIndex] = i * angleStep;
			}

			U32 curRanIndex = (randomIndex + i) % QUADS_PER_LAYER;

			//U32 preRanIndex = (curRanIndex + QUADS_PER_LAYER - 1) % QUADS_PER_LAYER;

			if (Go_Back)
			{
				theta = ThetaBackup[backupIndex] + deltaTime * RotationSpeed * RandomNums[curRanIndex];
			}
			else
			{
				theta = ThetaBackup[backupIndex] - deltaTime * RotationSpeed * RandomNums[curRanIndex];
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

	FinalMatrix = translationMatrix * viewRotationMatrix * rotationMatrix;

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

	if (RotateProcess > RotateInterval)
	{
		RandomOffset = ++RandomOffset % QUADS_PER_LAYER;
		Go_Back = !Go_Back;
		RotateProcess -= RotateInterval;
	}
}