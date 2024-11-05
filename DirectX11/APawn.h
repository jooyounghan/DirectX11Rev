#pragma once
#include "AActor.h"

class AController;
class InputEventManager;

class APawn : public AActor
{
public:
	APawn(
		MapAsset* MapAssetInstance,
		const std::string& PlaceableKindIn
	);
	virtual ~APawn();

protected:
	std::unique_ptr<AController> Controller;

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Render() override;

public:
	virtual void OnSerialize(FILE* FileIn) = 0;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) = 0;
};

