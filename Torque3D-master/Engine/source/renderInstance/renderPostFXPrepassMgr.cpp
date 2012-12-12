#include "platform/platform.h"
#include "renderInstance/renderPostFXPrepassMgr.h"

#include "scene/sceneManager.h"
#include "scene/sceneRenderState.h"
#include "materials/sceneData.h"
#include "materials/matInstance.h"
#include "materials/materialFeatureTypes.h"
#include "materials/processedMaterial.h"

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

IMPLEMENT_CONOBJECT(RenderPostFXPrepassMgr);

ConsoleDocClass(RenderPostFXPrepassMgr,
				"@brief A render bin for post fx prepass.\n\n"
				"TODO comments\n\n"
				"@ingroup RenderBin\n");

RenderPostFXPrepassMgr::RenderPostFXPrepassMgr(const String& registerName, const GFXFormat targetFormat)
: RenderTexTargetBinManager(RenderPassManager::RIT_ObjectTranslucent,
							1.f,
							1.f,
							targetFormat,
							Point2I(256,256))
{
	mNamedTarget.registerWithName(registerName);

	MeshInst.clear();

	MatInst = NULL;
}

RenderPostFXPrepassMgr::~RenderPostFXPrepassMgr()
{
	MeshInst.clear();
	SAFE_DELETE(MatInst);
}

bool RenderPostFXPrepassMgr::PointInWater(Point3F& point)
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