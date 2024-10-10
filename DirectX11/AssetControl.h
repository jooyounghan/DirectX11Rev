#pragma once
#include "IGuiControl.h"
#include "Delegation.h"
#include "HeaderHelper.h"

class AAssetFile;
class AssetControl;

typedef std::function<void(AAssetFile*)>	AssetControlBeginDragDropDelegate;
typedef std::function<void(AssetControl*)>	AssetControlStyleChangeDelegation;
typedef std::function<void(AssetControl*)>	AssetControlClickedDelegate;

class AssetControl : public IGuiControl
{
public:
	AssetControl(AAssetFile* AssetFileIn);
	~AssetControl();

protected:
	AAssetFile* AssetFileCached = nullptr;
	MakeGetter(AssetFileCached);

public:
	Delegation<AAssetFile*>		BeginDragDropEvent;

public:
	Delegation<AssetControl*>		HilightedEvent;
	Delegation<AssetControl*>		UnhilightedEvent;

public:
	Delegation<AssetControl*>	LeftMouseClickedEvent;
	Delegation<AssetControl*>	LeftMouseDBClickedEvent;

protected:
	bool bIsMouseHovering = false;
	bool bIsFocused = false;

public:
	void SetFocus(const bool& IsFocused) { bIsFocused = IsFocused; }

public:
	virtual void RenderControl() override;
};

