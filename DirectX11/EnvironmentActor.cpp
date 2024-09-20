#include "EnvironmentActor.h"

#include "AssetManager.h"

#include "BaseSphereMeshAsset.h"
#include "EXRTextureAsset.h"
#include "DDSTextureAsset.h"

#include "IGuiModelVisitor.h"

#include "PSOManager.h"
#include "PSOObject.h"

using namespace std;

EnvironmentActor::EnvironmentActor(MapAsset* MapAssetInstance)
	: AActor(MapAssetInstance)
{
	//App::GPSOManager->AddObjectToPSO<EnvironmentActor, EnvironmentActorPSO>(EPSOType::Static_Solid, this);
}

EnvironmentActor::~EnvironmentActor()
{
}

void EnvironmentActor::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void EnvironmentActor::Render()
{
	AActor::Render();
}

void EnvironmentActor::Update(const float& DeltaTimeIn)
{

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
