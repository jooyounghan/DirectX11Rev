#pragma once
#include "AActor.h"

class BaseMeshAsset;
class EXRTextureAsset;
class DDSTextureAsset;
class PSOObject;

struct SHDRToneMappingConstant
{
	float Exposure;
	float Gamma;
	float Dummy1;
	float Dummy2;
};

class EnvironmentActor : public AActor
{
public:
	EnvironmentActor();
	virtual ~EnvironmentActor();

public:
	static std::string EnvironmentActorKind;

protected:
	BaseMeshAsset* EnvironmentMeshAsset = nullptr;
	MakeSetterGetter(EnvironmentMeshAsset);

protected:
	std::shared_ptr<EXRTextureAsset> EnvironmentBackgroundEXRTextureAsset = nullptr;
	std::shared_ptr<DDSTextureAsset> EnvironmentSpecularDDSTextureAsset = nullptr;
	std::shared_ptr<DDSTextureAsset> EnvironmentDiffuseDDSTextureAsset = nullptr;
	std::shared_ptr<DDSTextureAsset> EnvironmentBRDFDDSTextureAsset = nullptr;
	MakeSmartPtrSetterGetter(EnvironmentBackgroundEXRTextureAsset);
	MakeSmartPtrSetterGetter(EnvironmentSpecularDDSTextureAsset);
	MakeSmartPtrSetterGetter(EnvironmentDiffuseDDSTextureAsset);
	MakeSmartPtrSetterGetter(EnvironmentBRDFDDSTextureAsset);

public:
	void SetToneMappingConstant(const float& ExposureIn, const float& GammaIn);

protected:
	std::shared_ptr<UploadBuffer<SHDRToneMappingConstant>> HDRToneMappingConstantBuffer;
	MakeSmartPtrGetter(HDRToneMappingConstantBuffer);

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

protected:
	PSOObject* EnvironmentActorPSOCached = nullptr;

public:
	virtual void Render(MapAsset* MapAssetIn) override final;

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn) override;

private:
	template<typename T>
	void AssetNameSerializeHelper(T AssetIn, FILE* FileIn);

};

