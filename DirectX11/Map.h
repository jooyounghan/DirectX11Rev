#pragma once
#include "Serializable.h"
#include "HeaderHelper.h"
#include "StaticAssertHelper.h"

#include "MeshObjectRenderer.h"
#include "BoundingComponentRenderer.h"

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

class Map : public ISerializable
{
public:
	Map(GraphicsPipeline* GraphicsPipelineInstance, PSOManager* PSOManagerInstance, AssetManager* AssetManagerInstance);
	~Map();

protected:
	GraphicsPipeline* GraphicsPipelineCached = nullptr;
	PSOManager* PSOManagerCached = nullptr;
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

protected:
	std::list<IIntersectable*> Tests;

public:
	void AddRenderObject(AMeshAsset* MeshAssetIn, float PosXIn, float PosYIn, float PosZIn);

public:
	void UpdateMap(const float& DeltaTimeIn);
	void RenderMap();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath) override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;

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
};

