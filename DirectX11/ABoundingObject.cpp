#include "ABoundingObject.h"
#include "GlobalVariable.h"
#include "Debugable.h"

#include "PSOManager.h"
#include "BoundingObjectPSO.h"
#include "PickingIDWireframePSO.h"

ABoundingObject::ABoundingObject()
	: AAttachableObject(), DebuggingColorBuffer()
{
	App::GPSOManager->AddObjectToPSO<ABoundingObject, BoundingObjectPSO>(EPSOType::BoundingComponent_Wireframe, this);
	App::GPSOManager->AddObjectToPSO<ABoundingObject, PickingIDWireframePSO>(EPSOType::BoundingComponent_ID_Wireframe, this);
}

ABoundingObject::~ABoundingObject()
{
	App::GPSOManager->RemoveObjectFromPSO<ABoundingObject, BoundingObjectPSO>(EPSOType::BoundingComponent_Wireframe, this);
	App::GPSOManager->RemoveObjectFromPSO<ABoundingObject, PickingIDWireframePSO>(EPSOType::BoundingComponent_ID_Wireframe, this);
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