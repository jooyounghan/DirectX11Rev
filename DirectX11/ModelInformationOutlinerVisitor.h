#pragma once
#include "IGuiTopLevelVisitor.h"
#include <DirectXMath.h>

class MapOutlinerWindow;

enum ETransfomationSelect : size_t
{
	Absolute,
	Relative,
	NumTransformationSelect
};

class ModelInformationOutlinerVisitor : public IGuiTopLevelVisitor
{
public:
	ModelInformationOutlinerVisitor(MapOutlinerWindow* MapOutlinerInstance);

protected:
	MapOutlinerWindow* MapOutlinerCached;

public:
	virtual void Visit(AttachableObject* AttachableInstance) override;
	virtual void Visit(PlaceableObject* PlaceableInstance) override;
	virtual void Visit(RelativePlaceableObject* RelativePlaceableObject) override;

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
		const T& ParentEntity
	);
};



