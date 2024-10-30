#include "SkeletalMeshObject.h"

#include "IGuiModelVisitor.h"
#include "GlobalVariable.h"

#include "AssetManager.h"
#include "SkeletalMeshAsset.h"
#include "BaseTextureAsset.h"
#include "DDSTextureAsset.h"
#include "MaterialAsset.h"
#include "MapAsset.h"

#include "ACamera.h"
#include "EnvironmentActor.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "StructuredBuffer.h"

using namespace std;

string SkeletalMeshObject::SkeletalMeshObjectKind = "Skeletal Mesh Object";

SkeletalMeshObject::SkeletalMeshObject(MapAsset* MapAssetInstance, std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstanceIn)
	: AMeshObject(MapAssetInstance, SkeletalMeshObject::SkeletalMeshObjectKind)
{
	static size_t SkeletalMeshObjectCount = 0;

	SkeletalMeshObjectCount++;
	ObjectName = SkeletalMeshObject::SkeletalMeshObjectKind + to_string(SkeletalMeshObjectCount);

	MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_Solid);
	PickingIDSolidPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Skeletal_ID_Solid);

	SetSkeletalMeshAssetInstance(SkeletalMeshAssetInstanceIn);
}

SkeletalMeshObject::~SkeletalMeshObject()
{
}

void SkeletalMeshObject::SetSkeletalMeshAssetInstance(const std::shared_ptr<SkeletalMeshAsset>& SkeletalMeshAssetInstanceIn)
{
	SkeletalMeshAssetInstance = SkeletalMeshAssetInstanceIn;
	if (SkeletalMeshAssetInstance != nullptr)
	{
		MaterialAssetInstances = SkeletalMeshAssetInstance->GetDefaultMaterialAssets();
		AnimationPlayerInstance.SetBoneAsset(SkeletalMeshAssetInstance->GetLinkedBoneAsset());
	}
}

AMeshAsset* SkeletalMeshObject::GetMeshAssetInstance()
{
	return SkeletalMeshAssetInstance.get();
}

void SkeletalMeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void SkeletalMeshObject::Update(const float& DeltaTimeIn)
{
	AMeshObject::Update(DeltaTimeIn);
	AnimationPlayerInstance.Update(DeltaTimeIn);
}

void SkeletalMeshObject::Render()
{
	AMeshObject::Render();
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectVSConstants(const size_t& MaterialIdx)
{
	ACamera* CurrentCamera = MapAssetCached->GetCurrentCamera();
	return std::vector<ID3D11Buffer*>{ CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer() };
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectHSConstants(const size_t& MaterialIdx)
{
	return std::vector<ID3D11Buffer*>();
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectDSConstants(const size_t& MaterialIdx)
{
	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	return std::vector<ID3D11Buffer*>{ MaterialInstance->GetMaterialDataBuffer()->GetBuffer() };
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectPSConstants(const size_t& MaterialIdx)
{
	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	ACamera* CurrentCamera = MapAssetCached->GetCurrentCamera();
	return std::vector<ID3D11Buffer*>{ CurrentCamera->GetViewProjBuffer()->GetBuffer(), MaterialInstance->GetMaterialDataBuffer()->GetBuffer() };
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectVSSRVs(const size_t& MaterialIdx)
{
	return std::vector<ID3D11ShaderResourceView*>{ AnimationPlayerInstance.GetAnimationTransformationBuffer()->GetStructuredBufferSRV() };
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectHSSRVs(const size_t& MaterialIdx)
{
	return std::vector<ID3D11ShaderResourceView*>();
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectDSSRVs(const size_t& MaterialIdx)
{
	vector<ID3D11ShaderResourceView*> Result;

	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	const shared_ptr<BaseTextureAsset> Height = MaterialInstance->GetHeightTextureAsset();
	Result.push_back(Height != nullptr ? Height->GetSRV() : nullptr);
	return Result;
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectPSSRVs(const size_t& MaterialIdx)
{
	vector<ID3D11ShaderResourceView*> Result;

	EnvironmentActor* CurrentEnvironment = MapAssetCached->GetEnvironmentActorInstance();
	DDSTextureAsset* SpecularDDS = CurrentEnvironment->GetEnvironmentSpecularDDSTextureAsset();
	DDSTextureAsset* DiffuseDDS = CurrentEnvironment->GetEnvironmentDiffuseDDSTextureAsset();
	DDSTextureAsset* BRDFDDS = CurrentEnvironment->GetEnvironmentBRDFDDSTextureAsset();

	Result.push_back(SpecularDDS != nullptr ? SpecularDDS->GetSRV() : nullptr);
	Result.push_back(DiffuseDDS != nullptr ? DiffuseDDS->GetSRV() : nullptr);
	Result.push_back(BRDFDDS != nullptr ? BRDFDDS->GetSRV() : nullptr);

	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	const shared_ptr<BaseTextureAsset> AO = MaterialInstance->GetAmbientOcculusionTextureAsset();
	const shared_ptr<BaseTextureAsset> Specualr = MaterialInstance->GetSpecularTextureAsset();
	const shared_ptr<BaseTextureAsset> Diffuse = MaterialInstance->GetDiffuseTextureAsset();
	const shared_ptr<BaseTextureAsset> Roughness = MaterialInstance->GetRoughnessTextureAsset();
	const shared_ptr<BaseTextureAsset> Metalic = MaterialInstance->GetMetalicTextureAsset();
	const shared_ptr<BaseTextureAsset> Normal = MaterialInstance->GetNormalTextureAsset();
	const shared_ptr<BaseTextureAsset> Emissive = MaterialInstance->GetEmissiveTextureAsset();

	Result.push_back(AO != nullptr ? AO->GetSRV() : nullptr);
	Result.push_back(Specualr != nullptr ? Specualr->GetSRV() : nullptr);
	Result.push_back(Diffuse != nullptr ? Diffuse->GetSRV() : nullptr);
	Result.push_back(Roughness != nullptr ? Roughness->GetSRV() : nullptr);
	Result.push_back(Metalic != nullptr ? Metalic->GetSRV() : nullptr);
	Result.push_back(Normal != nullptr ? Normal->GetSRV() : nullptr);
	Result.push_back(Emissive != nullptr ? Emissive->GetSRV() : nullptr);

	return Result;
}

void SkeletalMeshObject::OnSerializeFromMap(FILE* FileIn)
{
	AMeshObject::OnSerializeFromMap(FileIn);

	// Mesh Asset Name
	AAssetFile::SerializeString(SkeletalMeshAssetInstance != nullptr ? SkeletalMeshAssetInstance->GetAssetName() : "", FileIn);
}

void SkeletalMeshObject::OnDeserializeToMap(FILE* FileIn, AssetManager* AssetManagerIn)
{
	AMeshObject::OnDeserializeToMap(FileIn, AssetManagerIn);

	string MeshAssetName;
	AAssetFile::DeserializeString(MeshAssetName, FileIn);

	SkeletalMeshAssetInstance = AssetManagerIn->GetManagingSkeletalMesh(MeshAssetName);
	if (SkeletalMeshAssetInstance != nullptr)
	{
		AnimationPlayerInstance.SetBoneAsset(SkeletalMeshAssetInstance->GetLinkedBoneAsset());
	}
}
