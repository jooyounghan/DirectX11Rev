#pragma once
#include "IGuiControl.h"
#include "Delegation.h"
#include "HeaderHelper.h"

class AAssetFile;
class AssetControl;

struct ID3D11ShaderResourceView;

typedef std::function<void(AAssetFile*)>	AssetControlBeginDragDropHandler;
typedef std::function<void(AssetControl*)>	AssetControlStyleChangeHandler;
typedef std::function<void(AssetControl*)>	AssetControlClickedHandler;

class AssetControl : public IGuiControl
{
public:
	AssetControl(AAssetFile* AssetFileIn, ID3D11ShaderResourceView* ThumbnailSRVIn);
	~AssetControl();

protected:
	AAssetFile* AssetFileCached = nullptr;
	MakeGetter(AssetFileCached);

protected:
	ID3D11ShaderResourceView* ThumbnailSRV = nullptr;

public:
	Delegation<AAssetFile*>		BeginDragDropEvent;

public:
	Delegation<AssetControl*>		StartHilightedEvent;
	Delegation<AssetControl*>		EndHilightedEvent;

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

