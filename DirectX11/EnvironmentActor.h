#pragma once
#include "AActor.h"

class BaseMeshAsset;
class EXRTextureAsset;
class DDSTextureAsset;

class EnvironmentActor : public AActor
{
public:
	EnvironmentActor();
	virtual ~EnvironmentActor();

protected:
	BaseMeshAsset* EnvironmentMeshAsset = nullptr;
	MakeSetterGetter(EnvironmentMeshAsset);

protected:
	std::shared_ptr<EXRTextureAsset> EnvironmentBackgroundEXRTextureAsset = nullptr;
	std::shared_ptr<DDSTextureAsset> EnvironmentSpecularDDSTextureAsset = nullptr;
	std::shared_ptr<DDSTextureAsset> EnvironmentDiffuseDDSTextureAsset = nullptr;
	std::shared_ptr<DDSTextureAsset> EnvironmentBRDFDDSTextureAsset = nullptr;
	MakeSmartPtrGetter(EnvironmentBackgroundEXRTextureAsset);
	MakeSmartPtrGetter(EnvironmentSpecularDDSTextureAsset);
	MakeSmartPtrGetter(EnvironmentDiffuseDDSTextureAsset);
	MakeSmartPtrGetter(EnvironmentBRDFDDSTextureAsset);

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void OnSerializeFromMap(FILE* FileIn) override;
	virtual void OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn) override;

private:
	template<typename T>
	void AssetNameSerializeHelper(T AssetIn, FILE* FileIn);

};

