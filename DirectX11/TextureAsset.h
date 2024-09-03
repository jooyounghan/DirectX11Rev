#pragma once
#include "AAssetFile.h"
#include "HeaderHelper.h"
#include <wrl/client.h>
#include <d3d11.h>

class TextureAsset : public AAssetFile
{
public:
	TextureAsset(const std::string& AssetNameIn);
	TextureAsset(
		const std::string& AssetNameIn, 
		uint8_t* ImageBufferIn, 
		const UINT& WidthIn, 
		const UINT& HeightIn,
		const bool& IsHDR
	);
	virtual ~TextureAsset();

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>	Texture2D;
	MakeComPtrGetter(Texture2D);

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

