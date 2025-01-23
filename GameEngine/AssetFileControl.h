#pragma once
#include "AUserControl.h"

#include <d3d11.h>
#include <string>

class AAsset;
enum class EAssetType;

class AssetFileControl : public AUserControl
{
public:
	AssetFileControl(const EAssetType& assetType, AAsset* asset, ID3D11ShaderResourceView* thumbnailSRV);
	~AssetFileControl() override = default;

public:
	static std::string AssetFileControlDragAndDropID;

protected:
	const EAssetType m_assetType;
	AAsset* m_assetCached = nullptr;
	ID3D11ShaderResourceView* m_thumbanilCached = nullptr;

protected:
	float m_width = 0.f;
	float m_height = 0.f;
	
protected:
	bool m_isHilighted = false;

public:
	virtual void RenderControlImpl() override;
	virtual bool IsPointIn(const float& pointX, const float& pointY) const;

public:
	virtual void OnMouseEnter(MouseEventArgs& args) override;
	virtual void OnMouseLeave(MouseEventArgs& args) override;
	
public:
	virtual void OnBeginDrag() override;
	virtual void OnMouseDoubleClicked(MouseClickEventArgs& args) override;
};

