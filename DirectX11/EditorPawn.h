#pragma once
#include "APawn.h"
#include "HeaderHelper.h"
#include "imgui.h"

class ACamera;

class EditorPawn : public APawn
{
public:
	EditorPawn();
	virtual ~EditorPawn();

public:
	static std::string EditorPawnKind;

protected:
	std::unique_ptr<ACamera> CameraInstance = nullptr;
	MakeSmartPtrGetter(CameraInstance);

public:
	virtual void Update(const float& DeltaTimeIn) override;

public:
	virtual void Render(MapAsset* MapAssetIn) override final;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

