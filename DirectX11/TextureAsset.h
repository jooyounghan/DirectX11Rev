#pragma once
#include "AAssetFile.h"
#include "HeaderHelper.h"
#include <stack>

class TextureAsset : public AAssetFile
{
public:
	TextureAsset(const std::string& AssetNameIn);
	TextureAsset(
		const std::string& AssetNameIn, 
		uint8_t* ImageBufferIn, 
		const UINT& WidthIn, 
		const UINT& HeightIn
	);
	virtual ~TextureAsset();


protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				Texture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	SRV;
	MakeComPtrGetter(Texture2D);
	MakeComPtrGetter(SRV);

private:
	void CreateTexture(
		uint8_t* ImageBufferIn,
		const UINT& WidthIn,
		const UINT& HeightIn
	);

protected:
	int ImageSize = 0;
	uint8_t* CompressedBuffer = nullptr;

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

