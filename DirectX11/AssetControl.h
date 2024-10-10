#pragma once
#include "IGuiControl.h"
#include "Delegation.h"

class AAssetFile;

typedef std::function<void(AAssetFile*)>	AssetControlStyleChangeDelegation;
typedef std::function<void(AAssetFile*)>	AssetControlBeginDragDropDelegate;


class AssetControl : public IGuiControl
{
public:
	AssetControl(AAssetFile* AssetFileIn);
	~AssetControl();

protected:
	AAssetFile* AssetFileCached = nullptr;

public:
	Delegation<AAssetFile*>		HilightedEvent;
	Delegation<AAssetFile*>		UnhilightedEvent;
	Delegation<AAssetFile*>		BeginDragDropEvent;

protected:
	bool IsMouseHovering = true;

public:
	virtual void RenderControl() override;
};

