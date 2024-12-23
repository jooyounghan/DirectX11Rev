#include "StaticMeshObject.h"

#include "GlobalVariable.h"
#include "IGuiModelVisitor.h"

#include "AssetManager.h"
#include "StaticMeshAsset.h"	
#include "BaseTextureAsset.h"
#include "DDSTextureAsset.h"
#include "MaterialAsset.h"
#include "MapAsset.h"

#include "ACamera.h"
#include "EnvironmentActor.h"

#include "PSOManager.h"
#include "PSOObject.h"

using namespace std;

string StaticMeshObject::StaticMeshObjectKind = "Static Mesh Object";

StaticMeshObject::StaticMeshObject(std::shared_ptr<StaticMeshAsset> StaticMeshAssetInstanceIn)
	: AMeshObject(StaticMeshObject::StaticMeshObjectKind)
{
	static size_t StaticMeshObjectCount = 0;

	StaticMeshObjectCount++;
	ObjectName = StaticMeshObject::StaticMeshObjectKind + to_string(StaticMeshObjectCount);

	//MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Foward_Static_Solid);
	MeshObjectPSOCached = App::GPSOManager->GetPSOObject(EPSOType::Deffered_Static_Solid);

	SetStaticMeshAssetInstance(StaticMeshAssetInstanceIn);
}

StaticMeshObject::~StaticMeshObject()
{
}

void StaticMeshObject::SetStaticMeshAssetInstance(const std::shared_ptr<StaticMeshAsset>& StaticMeshAssetInstanceIn)
{
	StaticMeshAssetInstance = StaticMeshAssetInstanceIn;
	if (StaticMeshAssetInstanceIn != nullptr)
	{
		MaterialAssetInstances = StaticMeshAssetInstanceIn->GetDefaultMaterialAssets();
	}
}

AMeshAsset* StaticMeshObject::GetMeshAssetInstance()
{
	return StaticMeshAssetInstance.get();
}

void StaticMeshObject::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void StaticMeshObject::Render(MapAsset* MapAssetIn)
{
	AMeshObject::Render(MapAssetIn);
}

std::vector<ID3D11Buffer*> StaticMeshObject::GetMeshObjectVSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	return std::vector<ID3D11Buffer*>{ CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer() };
}

std::vector<ID3D11Buffer*> StaticMeshObject::GetMeshObjectHSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	return std::vector<ID3D11Buffer*>();
}

std::vector<ID3D11Buffer*> StaticMeshObject::GetMeshObjectDSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];

	return std::vector<ID3D11Buffer*>{
		CurrentCamera->GetViewProjBuffer()->GetBuffer(), TransformationBuffer->GetBuffer(),
		MaterialInstance != nullptr ? MaterialInstance->GetMaterialDataBuffer()->GetBuffer() : nullptr
	};
}

std::vector<ID3D11Buffer*> StaticMeshObject::GetMeshObjectPSConstants(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	ACamera* CurrentCamera = MapAssetIn->GetCurrentCamera();
	return std::vector<ID3D11Buffer*>{ 
		PickingIDBufferCached,
		CurrentCamera->GetViewProjBuffer()->GetBuffer(),
		MaterialInstance != nullptr ? MaterialInstance->GetMaterialDataBuffer()->GetBuffer() : nullptr
	};
}

std::vector<ID3D11ShaderResourceView*> StaticMeshObject::GetMeshObjectVSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	return std::vector<ID3D11ShaderResourceView*>();
}

std::vector<ID3D11ShaderResourceView*> StaticMeshObject::GetMeshObjectHSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	return std::vector<ID3D11ShaderResourceView*>();
}

std::vector<ID3D11ShaderResourceView*> StaticMeshObject::GetMeshObjectDSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
{
	vector<ID3D11ShaderResourceView*> Result;

	const shared_ptr<MaterialAsset>& MaterialInstance = MaterialAssetInstances[MaterialIdx];
	const shared_ptr<BaseTextureAsset> Height = 
		MaterialInstance != nullptr ? MaterialInstance->GetHeightTextureAsset() : nullptr;
	Result.push_back(Height != nullptr ? Height->GetSRV() : nullptr);
	return Result;
}

std::vector<ID3D11ShaderResourceView*> StaticMeshObject::GetMeshObjectPSSRVs(MapAsset* MapAssetIn, const size_t& MaterialIdx)
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

void StaticMeshObject::OnSerialize(FILE* FileIn)
{
	AMeshObject::OnSerialize(FileIn);

	// Mesh Asset Name
	AAssetFile::SerializeString(StaticMeshAssetInstance != nullptr ? StaticMeshAssetInstance->GetAssetName() : "", FileIn);

}

void StaticMeshObject::OnDeserialize(FILE* FileIn)
{
	AMeshObject::OnDeserialize(FileIn);

	string MeshAssetName;
	AAssetFile::DeserializeString(MeshAssetName, FileIn);

	StaticMeshAssetInstance = App::GAssetManager->GetManagingStaticMesh(MeshAssetName);
}
