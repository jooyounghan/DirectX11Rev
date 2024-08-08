#pragma once
#include "IGuiLowLevelVisitor.h"

#include <DirectXMath.h>

class AttachableObject;
class PlaceableObject;
class RelativePlaceableObject;

class MapOutlinerWindow;

enum ETransfomationSelect : size_t
{
	Absolute,
	Relative,
	NumTransformationSelect
};

class ModelInformationOutlinerVisitor : public IGuiLowLevelVisitor
{
public:
	ModelInformationOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance);

protected:
	MapOutlinerWindow* MapOutlinerCached;

public:
	virtual void Visit(MeshObject* AttachableInstance) override;

public:
	virtual void Visit(BoundingSphere* BoundingSphereInstance) override;
	virtual void Visit(OrientedBoundingBox* OBBInstance) override;
	virtual void Visit(BoundingFrustum* BoundingFrustumInstance) override;

public:
	virtual void Visit(Viewable* BoundingFrustumInstance) override;

private:
	void DrawAttachableInformation(AttachableObject* Attachable);
	void DrawPlaceableInformation(PlaceableObject* Placeable);
	void DrawRelativePlaceableInformation(RelativePlaceableObject* RelativePlaceableObject);

private:
	static const char* TransformationSelect[NumTransformationSelect];

private:
	void DrawTransformationForAttachable(AttachableObject* Attachable);
	void DrawTransformationForPlaceables(PlaceableObject* Placeable);
	void DrawTransformationForRelativePlaceable(RelativePlaceableObject* RelativePlaceableObject);

private:
	void DrawTransformationEntitySelection(const bool& IsOnlyAttached, const char* EntityName, ETransfomationSelect& SelectedIndexOut);

private:
	template<typename T>
	void DrawTransformationEntity(
		const ETransfomationSelect& SelectedIndex, 
		T& Entity, 
		const T& ParentEntity,
		const bool& Disabled
	);
};



