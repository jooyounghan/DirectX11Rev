#include "MapAsset.h"

#include "GraphicsPipeline.h"
#include "GlobalVariable.h"

#include "PSOManager.h"
#include "PSOObject.h"

#include "AssetManager.h"
#include "AMeshAsset.h"

#include "Actor.h"
#include "APawn.h"

#include "MeshObject.h"
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
	AAssetFile(LoadAsFile ? MapNameIn + AAssetFile::AssetTypeToSuffix[(EAssetType::Map)] : MapNameIn, EAssetType::Map)
{
}

MapAsset::~MapAsset()
{
	Serialize();
}

void MapAsset::AddRenderObject(std::shared_ptr<AMeshAsset> MeshAssetIn, float PosXIn, float PosYIn, float PosZIn)
{
	Actor* ta = PlaceableAddHelper<Actor>(MeshAssetIn);
	ta->RelativePosition.x = PosXIn;
	ta->RelativePosition.y = PosYIn;
	ta->RelativePosition.z = PosZIn;
}


void MapAsset::Update(const float& DeltaTimeIn)
{
	for (auto& ro : RootPlaceables)
	{
		ro->Update(DeltaTimeIn);
	}
	
	UpdateRenderState();
}

void MapAsset::UpdateRenderState()
{
	BoundingFrustumObject* CameraFrustum = CameraCached->GetCamearaFrustum();

	for (auto& ro : RootPlaceables)
	{
		const std::list<IIntersectable*>& Intersectables = ro->GetIntersectables();
		for (auto& intersectable : Intersectables)
		{
			CollisionVisitor CollisionVisitorInstance(intersectable);
			if (CollisionVisitorInstance.Visit(CameraFrustum))
			{
				ro->IgoreRendering = false;
			}
			else
			{
				ro->IgoreRendering = true;
			}
		}
	}
}

void MapAsset::RenderMap(PSOManager* PSOManagerInstance)
{
	CameraCached->CleanupLens();

	vector<ARenderer*> Renderers = {
		PSOManagerInstance->GetRenderers(EPSOType::R8G8B8A8_Skeletal_Solid),
		PSOManagerInstance->GetRenderers(EPSOType::R8G8B8A8_BoundingComponent_Wireframe)
	};

	ID3D11RenderTargetView* RTVs[] = { CameraCached->GetSceneRTV() };
	D3D11_VIEWPORT Viewports[] = { CameraCached->GetViewport() };
	ID3D11DepthStencilView* DSV = CameraCached->GetSceneDSV();

	for (auto& Renderer : Renderers)
	{
		Renderer->PresetRendering(1, RTVs, Viewports, DSV, CameraCached, this);

		for (const auto& ro : RootPlaceables)
		{
			if (!ro->IgoreRendering)
			{
				ro->AcceptRenderer(Renderer);
			}
		}

		Renderer->ResetRendering();
	}
}

void MapAsset::Serialize(const std::string& OutputAdditionalPath)
{
	FILE* OutputAssetFile = DefaultOpenFile(OutputAdditionalPath);
	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		size_t RootPlaceablesCount = RootPlaceables.size();
		fwrite(&RootPlaceablesCount, sizeof(size_t), 1, OutputAssetFile);

		for (auto& RootPlaceable : RootPlaceables)
		{
				// Object Kind
				const EPlaceableObjectKind& PlaceableObjectKind = RootPlaceable->GetPlaceableKind();
				fwrite(&PlaceableObjectKind, sizeof(EPlaceableObjectKind), 1, OutputAssetFile);

				RootPlaceable->OnSerialize(OutputAssetFile);

				SerializeChildrenObjects(RootPlaceable.get(), OutputAssetFile);
		}
		fclose(OutputAssetFile);
	}
}

void MapAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
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
		case ACTOR_KIND:
		{
			Actor* AddedActor = PlaceableAddHelper<Actor>();
			AddedActor->OnDeserialize(FileIn, AssetManagerIn);
			DeserializeParentObject(AddedActor, FileIn, AssetManagerIn);
			break;
		}
		case PAWN_KIND:
		{
			//APawn* AddedPawn = PlaceableAddHelper<APawn>();
			//AddedPawn->OnDeserialize(FileIn, AssetManagerIn);
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
		const EAttachableObjectKind& AttachedObjectKind = AttachedChild->GetAttachableKind();
		fwrite(&AttachedObjectKind, sizeof(EAttachableObjectKind), 1, FileIn);

		AttachedChild->OnSerialize(FileIn);

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
		case MESH_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<MeshObject>();
			break;
		case BOUNDING_SPHERE_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<BoundingSphereObject>();
			break;
		case OBB_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<OBBObject>();
			break;
		case NORMAL_CAMERA_KIND:
			AddedMeshObject = ParentObjectIn->AddAttachedObject<Camera>(App::GWidth, App::GHeight);
			break;
		default:
			break;
		}
		if (AddedMeshObject != nullptr)
		{
			AddedMeshObject->OnDeserialize(FileIn, AssetManagerIn);
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
