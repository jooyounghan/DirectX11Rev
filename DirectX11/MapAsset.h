#pragma once
#include "AAssetFile.h"

#include "HeaderHelper.h"
#include "StaticAssertHelper.h"

#include "APlaceableObject.h"
#include "AAttachableObject.h"

#include "IUpdatable.h"

#include <list>
#include <memory>
#include <unordered_map>
#include <type_traits>

class PSOManager;
class APSOObject;

class AssetManager;

class APlaceableObject;
class AAttachableObject;

class Camera;
class AObject;
class StaticMeshAsset;
class SkeletalMeshAsset;

class IIntersectable;

class MapAsset : public AAssetFile, public IUpdatable
{
public:
	MapAsset(
		const std::string& MapNameIn, 
		AssetManager* AssetManagerIn, 
		bool LoadAsFile
	);
	virtual ~MapAsset();

protected:
	AssetManager* AssetManagerCached = nullptr;

protected:
	Camera* CameraCached = nullptr;
	MakeSetter(CameraCached);

protected:
	std::list<std::unique_ptr<APlaceableObject>> RootPlaceables;
	MakeGetter(RootPlaceables);

protected:
	std::unordered_map<unsigned int, APlaceableObject*> IdToPlaceables;
	MakeGetter(IdToPlaceables);

public:
	void AddStaticMeshObjectActor(std::shared_ptr<StaticMeshAsset> StaticMeshAssetIn, float PosXIn, float PosYIn, float PosZIn);
	void AddSkeletalMeshObjectActor(std::shared_ptr<SkeletalMeshAsset> SkeletalMeshAssetIn, float PosXIn, float PosYIn, float PosZIn);

public:
	virtual void Update(const float& DeltaTimeIn) override;

private:
	void UpdateRenderState();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;

private:
	void SerializeChildrenObjects(APlaceableObject* ChildPlaceableObjectIn, FILE* FileIn);
	void SerializeChildrenObjects(AAttachableObject* ChildAttachableObjectIn, FILE* FileIn);

private:
	template<typename T>
	void DeserializeParentObject(T* ParentObjectIn, FILE* FileIn, AssetManager* AssetManagerIn);

public:
	virtual FILE* DefaultOpenFile(const std::string& OutputAdditionalPath);

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

