#pragma once
#include "APlaceableObject.h"
#include "HeaderHelper.h"

class AMeshAsset;

class AActor : public APlaceableObject
{
public:
	AActor(
		MapAsset* MapAssetInstance, 
		const std::string& PlaceableKindIn
	) : APlaceableObject(MapAssetInstance, PlaceableKindIn) {};
	virtual ~AActor() {};

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) = 0;

public:
	virtual void Render() override;
};

