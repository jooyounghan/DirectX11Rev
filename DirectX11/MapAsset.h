#pragma once
#include "AAssetFile.h"

#include "HeaderHelper.h"
#include "StaticAssertHelper.h"

#include "MeshObjectRenderer.h"
#include "BoundingObjectRenderer.h"

#include "PlaceableObject.h"
#include "AttachableObject.h"

#include <list>
#include <memory>
#include <unordered_map>
#include <type_traits>

class GraphicsPipeline;

class PSOManager;
class PSOObject;
class ARenderer;

class AssetManager;

class PlaceableObject;
class AttachableObject;

class Camera;
class AObject;
class MeshObject;
class AMeshAsset;

class IIntersectable;

class MapAsset : public AAssetFile
{
public:
	MapAsset(const std::string& MapNameIn, AssetManager* AssetManagerInstance, bool LoadAsFile);
	virtual ~MapAsset();

protected:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;
	AssetManager* AssetManagerCached = nullptr;

protected:
	Camera* CameraCached = nullptr;
	MakeSetter(CameraCached);

protected:
	std::list<std::unique_ptr<PlaceableObject>> RootPlaceables;
	MakeGetter(RootPlaceables);

protected:
	std::unordered_map<unsigned int, PlaceableObject*> IdToPlaceables;
	MakeGetter(IdToPlaceables);

public:
	void AddRenderObject(std::shared_ptr<AMeshAsset> MeshAssetIn, float PosXIn, float PosYIn, float PosZIn);
	void RenderMap(PSOManager* PSOManagerInstance);

public:
	void UpdateMap(const float& DeltaTimeIn);

private:
	void UpdateRenderState();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath = "") override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn, AssetManager* AssetManagerIn) override;

private:
	void SerializeChildrenObjects(PlaceableObject* ChildPlaceableObjectIn, FILE* FileIn);
	void SerializeChildrenObjects(AttachableObject* ChildAttachableObjectIn, FILE* FileIn);

private:
	template<typename T>
	void DeserializeParentObject(T* ParentObjectIn, FILE* FileIn, AssetManager* AssetManagerIn);

public:
	virtual FILE* DefaultOpenFile(const std::string& OutputAdditionalPath);

public:
	template<typename T, typename ...Args>
	T* PlaceableAddHelper(Args... args)
	{
		static_assert(std::is_base_of<PlaceableObject, T>::value, DerivedCondition(PlaceableObject));

		RootPlaceables.emplace_back(std::make_unique<T>(args...));
		PlaceableObject* RootPlaceable = RootPlaceables.back().get();

		IdToPlaceables[RootPlaceable->GetPickingID().GetID()] = RootPlaceable;

		T* AddedObject = (T*)(RootPlaceable);
		return AddedObject;
	}

public:
	void PlaceableDeleteHelper(PlaceableObject* PlaceableObjectIn);
};

