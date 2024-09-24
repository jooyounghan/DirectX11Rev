#include "MapAsset.h"

#include "GraphicsPipeline.h"
#include "GlobalVariable.h"

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
#include "OBBObject.h"
#include "BoundingFrustumObject.h"

#include "Camera.h"

using namespace std;
using namespace DirectX;

MapAsset::MapAsset(
	const std::string& MapNameIn,
	AssetManager* AssetManagerIn,
	bool LoadAsFile
)
	: 
	AssetManagerCached(AssetManagerIn),
	AAssetFile(LoadAsFile ? MapNameIn : MapNameIn + AAssetFile::AssetTypeToSuffix[(EAssetType::Map)], EAssetType::Map)
{
	if (!LoadAsFile)
	{
		EditorActorInstance = make_unique<EditorPawn>(this);
		CurrentCamera = EditorActorInstance->GetCameraInstance();

		EnvironmentActorInstance = make_unique<EnvironmentActor>(this);
		BaseMeshAsset* BaseMeshInstance = AssetManagerCached->GetManagingBaseMesh("BaseSphere");
		EnvironmentActorInstance->SetEnvironmentMeshAsset(BaseMeshInstance);
	}
}

MapAsset::~MapAsset()
{
	Serialize();
}

void MapAsset::AddStaticMeshObjectActor(std::shared_ptr<StaticMeshAsset> StaticMeshAssetIn, float PosXIn, float PosYIn, float PosZIn)
{
	StaticMeshObjectActor* StaticMeshObjectIn = PlaceableAddHelper<StaticMeshObjectActor>(StaticMeshAssetIn);
	StaticMeshObjectIn->RelativePosition.x = PosXIn;
	StaticMeshObjectIn->RelativePosition.y = PosYIn;
	StaticMeshObjectIn->RelativePosition.z = PosZIn;
}

void MapAsset::AddSkeletalMeshObjectActor(std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetIn, float PosXIn, float PosYIn, float PosZIn)
{
	SkeletalMeshObjectActor* SkeletalMeshObjectIn = PlaceableAddHelper<SkeletalMeshObjectActor>(SkeletalMeshAssetIn);
	SkeletalMeshObjectIn->RelativePosition.x = PosXIn;
	SkeletalMeshObjectIn->RelativePosition.y = PosYIn;
	SkeletalMeshObjectIn->RelativePosition.z = PosZIn;
}

void MapAsset::Update(const float& DeltaTimeIn)
{
	EditorActorInstance->Update(DeltaTimeIn);
	EnvironmentActorInstance->RelativePosition = EditorActorInstance->RelativePosition;
	EnvironmentActorInstance->Update(DeltaTimeIn);


	BoundingFrustumObject* CameraFrustum = CurrentCamera->GetCamearaFrustum();
	for (auto& RootPlaceable : RootPlaceables)
	{
		RootPlaceable->Update(DeltaTimeIn);

		const std::list<IIntersectable*>& Intersectables = RootPlaceable->GetIntersectables();
		for (auto& intersectable : Intersectables)
		{
			CollisionVisitor CollisionVisitorInstance(intersectable);
			if (CollisionVisitorInstance.Visit(CameraFrustum))
			{
				RootPlaceable->SetIsRenderable(true);
				break;
			}
			else
			{
				RootPlaceable->SetIsRenderable(false);
			}
		}
	}
}

void MapAsset::Render()
{
	CurrentCamera->CleanupLens();

	EnvironmentActorInstance->Render();

	for (auto& RootPlaceable : RootPlaceables)
	{
		if (RootPlaceable->GetIsRenderable())
		{
			RootPlaceable->Render();
		}
	}
}

void MapAsset::AcceptGui(IGuiModelVisitor* GuiVisitor)
{
	GuiVisitor->Visit(this);
}

void MapAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);
	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		EditorActorInstance->OnSerializeFromMap(OutputAssetFile);
		EnvironmentActorInstance->OnSerializeFromMap(OutputAssetFile);

		size_t RootPlaceablesCount = RootPlaceables.size();
		fwrite(&RootPlaceablesCount, sizeof(size_t), 1, OutputAssetFile);

		for (auto& RootPlaceable : RootPlaceables)
		{
				// Object Kind
				const EPlaceableObjectKind& PlaceableObjectKind = RootPlaceable->GetPlaceableKind();
				fwrite(&PlaceableObjectKind, sizeof(EPlaceableObjectKind), 1, OutputAssetFile);

				RootPlaceable->OnSerializeFromMap(OutputAssetFile);

				SerializeChildrenObjects(RootPlaceable.get(), OutputAssetFile);
		}
		fclose(OutputAssetFile);
	}
}

void MapAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	EditorActorInstance = make_unique<EditorPawn>(this);
	EnvironmentActorInstance = make_unique<EnvironmentActor>(this);
	
	EditorActorInstance->OnDeserializeToMap(FileIn, AssetManagerIn);
	EnvironmentActorInstance->OnDeserializeToMap(FileIn, AssetManagerIn);

	CurrentCamera = EditorActorInstance->GetCameraInstance();

	size_t RootPlaceablesCount;
	fread(&RootPlaceablesCount, sizeof(size_t), 1, FileIn);

	for (size_t idx = 0; idx < RootPlaceablesCount; ++idx)
	{
		// Object Kind
		EPlaceableObjectKind PlaceableObjectKind;
		fread(&PlaceableObjectKind, sizeof(EPlaceableObjectKind), 1, FileIn);

		switch (PlaceableObjectKind)
		{
		case PLACABLE_NONE:
			break;
		case STATIC_MESH_ACTOR_KIND:
		{
			StaticMeshObjectActor* AddedActor = PlaceableAddHelper<StaticMeshObjectActor>(nullptr);
			AddedActor->OnDeserializeToMap(FileIn, AssetManagerIn);
			DeserializeParentObject(AddedActor, FileIn, AssetManagerIn);
			break;
		}
		case SKELETAL_MESH_ACTOR_KIND:
		{
			SkeletalMeshObjectActor* AddedActor = PlaceableAddHelper<SkeletalMeshObjectActor>(nullptr);
			AddedActor->OnDeserializeToMap(FileIn, AssetManagerIn);
			DeserializeParentObject(AddedActor, FileIn, AssetManagerIn);
			break;
		}
		case ENVIORNMENT_ACTOR_KIND:
		{
			//EnvironmentActor* AddedEnvironment = PlaceableAddHelper<EnvironmentActor>();
			//AddedEnvironment->OnDeserializeToMap(FileIn, AssetManagerIn);
			//DeserializeParentObject(AddedEnvironment, FileIn, AssetManagerIn);
			//break;
		}
		case EDITOR_PAWN_KIND:
		{
			//APawn* AddedPawn = PlaceableAddHelper<APawn>();
			//AddedPawn->OnDeserializeToMap(FileIn, AssetManagerIn);
			//DeserializeParentObject(AddedPawn, FileIn, AssetManagerIn);
			//break;
		}
			break;
		case CHARACTER_KIND:
			break;
		default:
			break;
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
		const EAttachableObjectKind& AttachedObjectKind = AttachedChild->GetAttachableKind();
		fwrite(&AttachedObjectKind, sizeof(EAttachableObjectKind), 1, FileIn);

		AttachedChild->OnSerializeFromMap(FileIn);

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
		const EAttachableObjectKind& AttachedObjectKind = AttachedChild->GetAttachableKind();
		fwrite(&AttachedObjectKind, sizeof(EAttachableObjectKind), 1, FileIn);

		AttachedChild->OnSerializeFromMap(FileIn);

		SerializeChildrenObjects(AttachedChild.get(), FileIn);
	}
}

template<typename T>
inline void MapAsset::DeserializeParentObject(T* ParentObjectIn, FILE* FileIn, AssetManager* AssetManagerIn)
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
		EAttachableObjectKind AttachedObjectKind;
		fread(&AttachedObjectKind, sizeof(EAttachableObjectKind), 1, FileIn);

		AAttachableObject* AddedMeshObject = nullptr;
		switch (AttachedObjectKind)
		{
		case ATTACHABLE_NONE:
			break;
		case STATIC_MESH_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<StaticMeshObject>(this, nullptr);
			break;
		case SKELETAL_MESH_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<SkeletalMeshObject>(this, nullptr);
			break;
		case BOUNDING_SPHERE_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<BoundingSphereObject>(this);
			break;
		case OBB_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<OBBObject>(this);
			break;
		case SDR_CAMERA_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<Camera>(this, App::GWidth, App::GHeight);
			break;
		default:
			break;
		}
		if (AddedMeshObject != nullptr)
		{
			AddedMeshObject->OnDeserializeToMap(FileIn, AssetManagerIn);
			DeserializeParentObject(AddedMeshObject, FileIn, AssetManagerIn);
		}
	}
}


FILE* MapAsset::DefaultOpenFile(const std::string& OutputAdditionalPath)
{
	const string OutputPath = OutputAdditionalPath.empty() ?
		MapAssetOutPath : MapAssetOutPath + OutputAdditionalPath;

	const string OutputFullPath = OutputPath + AssetName + AssetExtension;

	CreateDirectoryA(OutputPath.c_str(), NULL);

	FILE* OutputAssetFile = nullptr;
	errno_t result = fopen_s(&OutputAssetFile, OutputFullPath.c_str(), "wb");

	return OutputAssetFile;

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
