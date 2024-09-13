#include "EnvironmentActor.h"

#include "AssetManager.h"

#include "BaseSphereMeshAsset.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

#include "ARenderer.h"
#include "IGuiModelVisitor.h"

using namespace std;

EnvironmentActor::EnvironmentActor()
	: AActor()
{
}

EnvironmentActor::~EnvironmentActor()
{
}

void EnvironmentActor::AcceptRenderer(ARenderer* Renderer)
{
	AcceptChildrenRenderer(Renderer);
	Renderer->Render(this);
}

void EnvironmentActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void EnvironmentActor::OnSerializeFromMap(FILE* FileIn)
{
	AObject::OnSerializeFromMap(FileIn);

	AssetNameSerializeHelper(EnvironmentMeshAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentBackgroundEXRTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentSpecularDDSTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentDiffuseDDSTextureAsset, FileIn);
	AssetNameSerializeHelper(EnvironmentBRDFDDSTextureAsset, FileIn);
}

void EnvironmentActor::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string EnvironmentMeshAssetName;
	string EnvironmentBackgroundEXRTextureAssetName;
	string EnvironmentSpecularDDSTextureAssetName;
	string EnvironmentDiffuseDDSTextureAssetName;
	string EnvironmentBRDFDDSTextureAssetName;

	DeserializeString(EnvironmentMeshAssetName, FileIn);
	DeserializeString(EnvironmentBackgroundEXRTextureAssetName, FileIn);
	DeserializeString(EnvironmentSpecularDDSTextureAssetName, FileIn);
	DeserializeString(EnvironmentDiffuseDDSTextureAssetName, FileIn);
	DeserializeString(EnvironmentBRDFDDSTextureAssetName, FileIn);

	EnvironmentMeshAsset = AssetManagerIn->GetManagingBaseMesh(EnvironmentMeshAssetName);
	EnvironmentBackgroundEXRTextureAsset = AssetManagerIn->GetManagingEXRTexture(EnvironmentMeshAssetName);
	EnvironmentSpecularDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentMeshAssetName);
	EnvironmentDiffuseDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentMeshAssetName);
	EnvironmentBRDFDDSTextureAsset = AssetManagerIn->GetManagingDDSTexture(EnvironmentMeshAssetName);
}

template<typename T>
void EnvironmentActor::AssetNameSerializeHelper(T AssetIn, FILE* FileIn)
{
	const string AssetName = AssetIn != nullptr ? AssetIn->GetAssetName() : "";
	SerializeString(AssetName, FileIn);
}
