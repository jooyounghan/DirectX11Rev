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

SkeletalMeshObject::SkeletalMeshObject(std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetInstanceIn)
	: AMeshObject(SkeletalMeshObject::SkeletalMeshObjectKind)
{
	static size_t SkeletalMeshObjectCount = 0;

	SkeletalMeshObjectCount++;
	ObjectName = SkeletalMeshObject::SkeletalMeshObjectKind + to_string(SkeletalMeshObjectCount);

	//MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Forward_Skeletal_Solid);
	MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Deffered_Skeletal_Solid);

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

void SkeletalMeshObject::SetAnimationAssetInstance(const std::shared_ptr<AnimationAsset>& AnimationAssetInstanceIn)
{
	AnimationPlayerInstance.PlayAnimation(AnimationAssetInstanceIn, INFINITE_PLAY);
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

void SkeletalMeshObject::Render(MapAsset* MapAssetIn)
{
	AMeshObject::Render(MapAssetIn);
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectVSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	return std::vector<ID3D11Buffer*>{ CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer() };
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectHSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	return std::vector<ID3D11Buffer*>();
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectDSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];

	return std::vector<ID3D11Buffer*>{
		CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer(),
		MaterialInstance != nullptr ? MaterialInstance->GetMaterialDataBuffer()->GetBuffer() : nullptr
	};
}

std::vector<ID3D11Buffer*> SkeletalMeshObject::GetMeshObjectPSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	return std::vector<ID3D11Buffer*>{
		PickingIDBufferCached,
		CurrentCamera->GetViewProjBuffer()->GetBuffer(),
			MaterialInstance != nullptr ? MaterialInstance->GetMaterialDataBuffer()->GetBuffer() : nullptr
	};
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectVSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	return std::vector<ID3D11ShaderResourceView*>{ AnimationPlayerInstance.GetAnimationTransformationBuffer()->GetStructuredBufferSRV() };
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectHSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	return std::vector<ID3D11ShaderResourceView*>();
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectDSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	vector<ID3D11ShaderResourceView*> Result;

	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	const shared_ptr<BaseTextureAsset> Height =
		MaterialInstance != nullptr ? MaterialInstance->GetHeightTextureAsset() : nullptr;
	Result.push_back(Height != nullptr ? Height->GetSRV() : nullptr);
	return Result;
}

std::vector<ID3D11ShaderResourceView*> SkeletalMeshObject::GetMeshObjectPSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	vector<ID3D11ShaderResourceView*> Result;

	EnvironmentActor* CurrentEnvironment = MapAssetIn->GetEnvironmentActorInstance();
	DDSTextureAsset* SpecularDDS = CurrentEnvironment->GetEnvironmentSpecularDDSTextureAsset();
	DDSTextureAsset* DiffuseDDS = CurrentEnvironment->GetEnvironmentDiffuseDDSTextureAsset();
	DDSTextureAsset* BRDFDDS = CurrentEnvironment->GetEnvironmentBRDFDDSTextureAsset();

	Result.push_back(SpecularDDS != nullptr ? SpecularDDS->GetSRV() : nullptr);
	Result.push_back(DiffuseDDS != nullptr ? DiffuseDDS->GetSRV() : nullptr);
	Result.push_back(BRDFDDS != nullptr ? BRDFDDS->GetSRV() : nullptr);

	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];

	if (MaterialInstance != nullptr)
	{
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
	}
	else
	{
		Result.insert(Result.end(), { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr });
	}

	return Result;
}




void SkeletalMeshObject::OnSerialize(FILE* FileIn)
{
	AMeshObject::OnSerialize(FileIn);

	// Mesh Asset Name
	AAssetFile::SerializeString(SkeletalMeshAssetInstance != nullptr ? SkeletalMeshAssetInstance->GetAssetName() : "", FileIn);
}

void SkeletalMeshObject::OnDeserialize(FILE* FileIn)
{
	AMeshObject::OnDeserialize(FileIn);

	string MeshAssetName;
	AAssetFile::DeserializeString(MeshAssetName, FileIn);

	SkeletalMeshAssetInstance = App::GAssetManager->GetManagingSkeletalMesh(MeshAssetName);
	if (SkeletalMeshAssetInstance != nullptr)
	{
		AnimationPlayerInstance.SetBoneAsset(SkeletalMeshAssetInstance->GetLinkedBoneAsset());
	}
}
