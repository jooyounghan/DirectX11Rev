#pragma once
#include "AAttachableObject.h"

class AMeshAsset;

class AMeshObject : public AAttachableObject
{
public:
	AMeshObject(MapAsset* MapAssetInstance);
	virtual ~AMeshObject();

public:
	virtual AMeshAsset* GetMeshAssetInstance() = 0;
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	size_t GetLODLevel(
		const DirectX::XMFLOAT3& From, 
		const float& MaxDistance, 
		const size_t& MaxLODCount, 
		const size_t SteepLevel
	);

public:
	virtual void Render() override;
};

