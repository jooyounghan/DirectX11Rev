#pragma once
#include "AAssetFile.h"

#include "HeaderHelper.h"
#include "StaticAssertHelper.h"
#include "IGuiModelAcceptor.h"

#include "APlaceableObject.h"
#include "AAttachableObject.h"

#include <list>
#include <unordered_map>
#include <memory>
#include <type_traits>

class AssetManager;

class EditorPawn;
class EnvironmentActor;

class ACamera;
class StaticMeshAsset;
class SkeletalMeshAsset;

constexpr const char* MapAssetOutPath = ".\\Assets\\Maps\\";

class MapAsset : public AAssetFile, public IGuiModelAcceptor
{
public:
	MapAsset(
		const std::string& MapNameIn, 
		bool LoadFromAsset
	);
	virtual ~MapAsset();

public:
	static std::string MapAssetKind;

protected:
	std::unique_ptr<EditorPawn> EditorActorInstance;
	std::unique_ptr<EnvironmentActor> EnvironmentActorInstance;
	MakeSmartPtrGetter(EditorActorInstance);
	MakeSmartPtrGetter(EnvironmentActorInstance);

protected:
	ACamera* CurrentCamera = nullptr;
	MakeGetter(CurrentCamera);

protected:
	std::list<std::unique_ptr<APlaceableObject>> RootPlaceables;
	std::unordered_map<unsigned int, APlaceableObject*> IdToPlaceables;
	MakeGetter(RootPlaceables);
	MakeGetter(IdToPlaceables);

public:
	void AddAsset(AAssetFile* AssetFileIn, const float& PosXIn, const float& PosYIn, const float& PosZIn);
	APlaceableObject* GetSelectedPlaceableByID(const UINT& IdIn);

private:
	void AddStaticMeshObjectActor(std::shared_ptr<StaticMeshAsset> StaticMeshAssetIn, const float& PosXIn, const float& PosYIn, const float& PosZIn);
	void AddSkeletalMeshObjectActor(std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetIn, const float& PosXIn, const float& PosYIn, const float& PosZIn);

private:
	void SetEnvironmentActorByCamera();

public:
	virtual void AcceptGui(IGuiModelVisitor* GuiVisitor) override;

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn) override;

private:
	void SerializeChildrenObjects(APlaceableObject* ChildPlaceableObjectIn, FILE* FileIn);
	void SerializeChildrenObjects(AAttachableObject* ChildAttachableObjectIn, FILE* FileIn);

private:
	template<typename T>
	void DeserializeParentObject(T* ParentObjectIn, FILE* FileIn);

public:
	template<typename T, typename ...Args>
	T* PlaceableAddHelper(Args... args)
	{
		static_assert(std::is_base_of<APlaceableObject, T>::value, DerivedCondition(APlaceableObject));

		RootPlaceables.emplace_back(std::make_unique<T>(args...));
		APlaceableObject* RootPlaceable = RootPlaceables.back().get();

		IdToPlaceables[RootPlaceable->GetPickingID().GetID()] = RootPlaceable;

		T* AddedObject = (T*)(RootPlaceable);
		return AddedObject;
	}

public:
	void PlaceableDeleteHelper(APlaceableObject* PlaceableObjectIn);
};

