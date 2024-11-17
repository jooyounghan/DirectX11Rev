#include "MapAsset.h"

#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
#include "AssetManager.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "IGuiModelVisitor.h"

#include "AssetManager.h"
#include "StaticMeshAsset.h"
#include "SkeletalMeshAsset.h"

#include "StaticMeshObjectActor.h"
#include "SkeletalMeshObjectActor.h"
#include "EnvironmentActor.h"
#include "EditorPawn.h"

#include "StaticMeshObject.h"
#include "SkeletalMeshObject.h"
#include "BoundingSphereObject.h"
#include "BoundingFrustumObject.h"
#include "OBBObject.h"
#include "SDRCamera.h"
#include "HDRCamera.h"


using namespace std;
using namespace DirectX;

string MapAsset::MapAssetKind = "Map";

MapAsset::MapAsset(
	const std::string& MapNameIn,
	bool LoadFromAsset
)
	: 
	AAssetFile(LoadFromAsset ? MapNameIn : MapNameIn + "_" + MapAsset::MapAssetKind, MapAsset::MapAssetKind)
{
	if (!LoadFromAsset)
	{
		EditorActorInstance = make_unique<EditorPawn>();
		EnvironmentActorInstance = make_unique<EnvironmentActor>();
		BaseMeshAsset* BaseMeshInstance = App::GAssetManager->GetManagingBaseMesh("BaseSphere");
		EnvironmentActorInstance->SetEnvironmentMeshAsset(BaseMeshInstance);

		CurrentCamera = EditorActorInstance->GetCameraInstance();
		SetEnvironmentActorByCamera();
	}
}

MapAsset::~MapAsset()
{
	Serialize();
}

APlaceableObject* MapAsset::GetSelectedPlaceableByID(const UINT& IdIn)
{
	if (IdToPlaceables.find(IdIn) != IdToPlaceables.end())
	{
		return IdToPlaceables.at(IdIn);
	}
	return nullptr;
}


void MapAsset::AddAsset(AAssetFile* AssetFileIn, const float& PosXIn, const float& PosYIn, const float& PosZIn)
{
	const string& AssetType = AssetFileIn->GetAssetType();

	if (AssetType == StaticMeshAsset::StaticMeshAssetKind)
	{
		AddStaticMeshObjectActor(
			App::GAssetManager->GetManagingStaticMesh(AssetFileIn->GetAssetName()),
			PosXIn,
			PosYIn,
			PosZIn
		);
	}
	else if (AssetType == SkeletalMeshAsset::SkeletalMeshAssetKind)
	{
		AddSkeletalMeshObjectActor(
			App::GAssetManager->GetManagingSkeletalMesh(AssetFileIn->GetAssetName()),
			PosXIn,
			PosYIn,
			PosZIn
		);
	}
}

void MapAsset::AddStaticMeshObjectActor(std::shared_ptr<StaticMeshAsset> StaticMeshAssetIn, const float& PosXIn, const float& PosYIn, const float& PosZIn)
{
	StaticMeshObjectActor* StaticMeshObjectIn = PlaceableAddHelper<StaticMeshObjectActor>(StaticMeshAssetIn);
	StaticMeshObjectIn->RelativePosition.x = PosXIn;
	StaticMeshObjectIn->RelativePosition.y = PosYIn;
	StaticMeshObjectIn->RelativePosition.z = PosZIn;
}

void MapAsset::AddSkeletalMeshObjectActor(std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetIn, const float& PosXIn, const float& PosYIn, const float& PosZIn)
{
	SkeletalMeshObjectActor* SkeletalMeshObjectIn = PlaceableAddHelper<SkeletalMeshObjectActor>(SkeletalMeshAssetIn);
	SkeletalMeshObjectIn->RelativePosition.x = PosXIn;
	SkeletalMeshObjectIn->RelativePosition.y = PosYIn;
	SkeletalMeshObjectIn->RelativePosition.z = PosZIn;
}

void MapAsset::SetEnvironmentActorByCamera()
{
	if (CurrentCamera != nullptr)
	{
		EnvironmentActorInstance->RelativeScale.x = CurrentCamera->FarZ * 0.95f;
		EnvironmentActorInstance->RelativeScale.y = CurrentCamera->FarZ * 0.95f;
		EnvironmentActorInstance->RelativeScale.z = CurrentCamera->FarZ * 0.95f;
	}
}

void MapAsset::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void MapAsset::Serialize()
{
	FILE* OutputAssetFile;
	string OutputAssetFilePath = DefaultOpenFileHelper(MapAssetOutPath, OutputAssetFile);
	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		EditorActorInstance->OnSerialize(OutputAssetFile);
		EnvironmentActorInstance->OnSerialize(OutputAssetFile);

		size_t RootPlaceablesCount = RootPlaceables.size();
		fwrite(&RootPlaceablesCount, sizeof(size_t), 1, OutputAssetFile);

		for (auto& RootPlaceable : RootPlaceables)
		{
			// Object Kind
			const string& PlaceableObjectKind = RootPlaceable->GetPlaceableKind();
			SerializeString(PlaceableObjectKind, OutputAssetFile);

			RootPlaceable->OnSerialize(OutputAssetFile);

			SerializeChildrenObjects(RootPlaceable.get(), OutputAssetFile);
		}
		fclose(OutputAssetFile);
	}
}

void MapAsset::Deserialize(FILE* FileIn)
{
	EditorActorInstance = make_unique<EditorPawn>();
	EnvironmentActorInstance = make_unique<EnvironmentActor>();

	EditorActorInstance->OnDeserialize(FileIn);
	EnvironmentActorInstance->OnDeserialize(FileIn);

	CurrentCamera = EditorActorInstance->GetCameraInstance();
	SetEnvironmentActorByCamera();

	size_t RootPlaceablesCount;
	fread(&RootPlaceablesCount, sizeof(size_t), 1, FileIn);

	for (size_t idx = 0; idx < RootPlaceablesCount; ++idx)
	{
		// Object Kind
		string PlaceableObjectKind;
		DeserializeString(PlaceableObjectKind, FileIn);

		if (PlaceableObjectKind == StaticMeshObjectActor::StaticMeshObjectActorKind)
		{
			StaticMeshObjectActor* AddedActor = PlaceableAddHelper<StaticMeshObjectActor>(nullptr);
			AddedActor->OnDeserialize(FileIn);
			DeserializeParentObject(AddedActor, FileIn);
		}
		else if (PlaceableObjectKind == SkeletalMeshObjectActor::SkeletalMeshObjectActorKind)
		{
			SkeletalMeshObjectActor* AddedActor = PlaceableAddHelper<SkeletalMeshObjectActor>(nullptr);
			AddedActor->OnDeserialize(FileIn);
			DeserializeParentObject(AddedActor, FileIn);
		}
		else if (PlaceableObjectKind == StaticMeshObjectActor::StaticMeshObjectActorKind)
		{
			//EnvironmentActor* AddedEnvironment = PlaceableAddHelper<EnvironmentActor>();
			//AddedEnvironment->OnDeserialize(FileIn, AssetManagerIn);
			//DeserializeParentObject(AddedEnvironment, FileIn, AssetManagerIn);
			//break;
		}
		else if (PlaceableObjectKind == StaticMeshObjectActor::StaticMeshObjectActorKind)
		{
			//APawn* AddedPawn = PlaceableAddHelper<APawn>();
			//AddedPawn->OnDeserialize(FileIn, AssetManagerIn);
			//DeserializeParentObject(AddedPawn, FileIn, AssetManagerIn);
			//break;
		}
		else
		{

		}
	}
}

void MapAsset::SerializeChildrenObjects(APlaceableObject* ChildPlaceableObjectIn, FILE* FileIn)
{
	// Attached Children Count
	size_t AttachedChildrenCount = ChildPlaceableObjectIn->GetAttachedChildrenObjects().size();
	fwrite(&AttachedChildrenCount, sizeof(size_t), 1, FileIn);

	for (auto& AttachedChild : ChildPlaceableObjectIn->GetAttachedChildrenObjects())
	{
		// Object Kind
		const string& AttachedObjectKind = AttachedChild->GetAttachableKind();
		SerializeString(AttachedObjectKind, FileIn);

		AttachedChild->OnSerialize(FileIn);

		SerializeChildrenObjects(AttachedChild.get(), FileIn);
	}
}

void MapAsset::SerializeChildrenObjects(AAttachableObject* ChildAttachableObjectIn, FILE* FileIn)
{
	// Attached Children Count
	size_t AttachedChildrenCount = ChildAttachableObjectIn->GetAttachedChildrenObjects().size();
	fwrite(&AttachedChildrenCount, sizeof(size_t), 1, FileIn);

	for (auto& AttachedChild : ChildAttachableObjectIn->GetAttachedChildrenObjects())
	{
		// Object Kind
		const string& AttachedObjectKind = AttachedChild->GetAttachableKind();
		SerializeString(AttachedObjectKind, FileIn);

		AttachedChild->OnSerialize(FileIn);

		SerializeChildrenObjects(AttachedChild.get(), FileIn);
	}
}

template<typename T>
inline void MapAsset::DeserializeParentObject(T* ParentObjectIn, FILE* FileIn)
{
	static_assert(
		std::is_base_of<APlaceableObject, T>::value | std::is_base_of<AAttachableObject, T>::value,
		DerivedCondition(APlaceableObject | AAttachableObject)
		);

	size_t ChildrenAttachedCount;
	fread(&ChildrenAttachedCount, sizeof(size_t), 1, FileIn);

	for (size_t idx = 0; idx < ChildrenAttachedCount; ++idx)
	{
		// Object Kind
		string AttachableObjectKind;
		DeserializeString(AttachableObjectKind, FileIn);

		AAttachableObject* AddedMeshObject = nullptr;
		if (AttachableObjectKind == StaticMeshObject::StaticMeshObjectKind)
		{
			AddedMeshObject = ParentObjectIn->AddAttachedObject<StaticMeshObject>(nullptr);
		}
		else if (AttachableObjectKind == SkeletalMeshObject::SkeletalMeshObjectKind)
		{
			AddedMeshObject = ParentObjectIn->AddAttachedObject<SkeletalMeshObject>(nullptr);
		}
		else if (AttachableObjectKind == BoundingSphereObject::BoundingSphereKind)
		{
			AddedMeshObject = ParentObjectIn->AddAttachedObject<BoundingSphereObject>();
		}
		//else if (AttachableObjectKind == BoundingFrustumObject::BoundingFrustumKind)
		//{
		//	AddedMeshObject = ParentObjectIn->AddAttachedObject<BoundingFrustumObject>();
		//}
		else if (AttachableObjectKind == OBBObject::BoundingOBBKind)
		{
			AddedMeshObject = ParentObjectIn->AddAttachedObject<OBBObject>();
		}
		else if (AttachableObjectKind == SDRCamera::SDRCameraKind)
		{
			AddedMeshObject = ParentObjectIn->AddAttachedObject<SDRCamera>(App::GWidth, App::GHeight);
		}
		else if (AttachableObjectKind == HDRCamera::HDRCameraKind)
		{
			AddedMeshObject = ParentObjectIn->AddAttachedObject<HDRCamera>(App::GWidth, App::GHeight);
		}

		if (AddedMeshObject != nullptr)
		{
			AddedMeshObject->OnDeserialize(FileIn);
			DeserializeParentObject(AddedMeshObject, FileIn);
		}
	}
}

void MapAsset::PlaceableDeleteHelper(APlaceableObject* PlaceableObjectIn)
{
	const unsigned int& PlaceableId = PlaceableObjectIn->GetPickingID().GetID();
	if (IdToPlaceables.find(PlaceableId) != IdToPlaceables.end())
	{
		IdToPlaceables.erase(PlaceableId);
	}

	auto it = std::find_if(RootPlaceables.begin(), RootPlaceables.end(),
		[PlaceableObjectIn](const std::unique_ptr<APlaceableObject>& ptr)
		{
			return ptr.get() == PlaceableObjectIn;
		}
	);

	if (it != RootPlaceables.end())
	{
		RootPlaceables.erase(it);
	}
}
