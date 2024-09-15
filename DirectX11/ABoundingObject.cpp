#include "ABoundingObject.h"
#include "GlobalVariable.h"
#include "Debugable.h"

#include "PSOManager.h"
#include "BoundingObjectPSO.h"
#include "PickingIDWireframePSO.h"

ABoundingObject::ABoundingObject()
	: AAttachableObject(), DebuggingColorBuffer()
{
	BoundingObjectPSO* Pso = reinterpret_cast<BoundingObjectPSO*>(App::GPSOManager->GetPSOObject(EPSOType::R8G8B8A8_BoundingComponent_Wireframe));
	if (Pso != nullptr) Pso->AddABoundingObject(this);
	PickingIDWireframePSO* PsoForID = reinterpret_cast<PickingIDWireframePSO*>(App::GPSOManager->GetPSOObject(EPSOType::R8G8B8A8_BoundingComponent_ID_Wireframe));
	if (PsoForID != nullptr) PsoForID->AddABoundingObject(this);
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