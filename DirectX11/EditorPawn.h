#pragma once
#include "APawn.h"
#include "HeaderHelper.h"
#include "imgui.h"

class Camera;

class EditorPawn : public APawn
{
public:
	EditorPawn(MapAsset* MapAssetInstance);
	virtual ~EditorPawn();

protected:
	std::unique_ptr<Camera> CameraInstance = nullptr;
	MakeSmartPtrGetter(CameraInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void Render() override final;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

