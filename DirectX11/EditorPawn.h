#pragma once
#include "APawn.h"
#include "HeaderHelper.h"
#include "imgui.h"

class ACamera;

class EditorPawn : public APawn
{
public:
	EditorPawn(MapAsset* MapAssetInstance);
	virtual ~EditorPawn();

public:
	static std::string EditorPawnKind;

protected:
	std::unique_ptr<ACamera> CameraInstance = nullptr;
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

