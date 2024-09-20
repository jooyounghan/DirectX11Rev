#include "ABoundingObject.h"
#include "GlobalVariable.h"
#include "Debugable.h"

#include "PSOManager.h"
#include "PSOObject.h"

ABoundingObject::ABoundingObject(MapAsset* MapAssetInstance)
	: AAttachableObject(MapAssetInstance), DebuggingColorBuffer()
{
	BoundingObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::BoundingObject_Wireframe);
	PickingIDWireframePSOCached = App::GPSOManager->GetPSOObject(EPSOType::BoundingObject_ID_Wireframe);
}

ABoundingObject::~ABoundingObject()
{
}

void ABoundingObject::UpdateColor(const XMVECTOR& ColorIn, ID3D11DeviceContext* DeviceContextIn)
{
}

void ABoundingObject::SetCollisionColor()
{
	if (DebugObject != nullptr)
	{
		if (IsCollided)
		{
			DebuggingColorBuffer.Upload(XMVectorSet(0.f, 1.f, 0.f, 1.f));
		}
		else
		{
			DebuggingColorBuffer.Upload(XMVectorSet(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void ABoundingObject::Render()
{
}
