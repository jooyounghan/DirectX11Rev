#pragma once
#include "AAddObjectHelper.h"

class AddAttachableObjectDrawer : public AAddObjectHelper
{
private:
	enum EAttachableObjectKind : int
	{
		MeshObjectKind,
		BoundingSphereKind,
		OrientedBoundingBoxKind,
	};

public:
	AddAttachableObjectDrawer(EditorWorld* EditorWorldIn, Map* CurrentMap);

public:
	virtual void DrawAddObjectPopup() override;
};

