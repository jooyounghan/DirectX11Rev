#pragma once
#include "Serializable.h"
#include "HeaderHelper.h"

#include <list>
#include <memory>
#include <unordered_map>

class GraphicsPipeline;
class PSOManager;
class PSOObject;
class AssetManager;

class Camera;
class IRenderable;
class PlaceableObject;
class RenderObject;
class IMeshAsset;

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
	std::unique_ptr<Camera> MapCamera;
	MakeSmartPtrGetter(MapCamera);

protected:
	std::unordered_map<PSOObject*, std::vector<IRenderable*>> PSOObjectToRenderables;
	std::list<std::unique_ptr<PlaceableObject>> Placeables;

public:
	void AddRenderObject(IMeshAsset* MeshAssetIn, float PosXIn, float PosYIn, float PosZIn);
	void AddRenderObject(IMeshAsset* MeshAssetIn, float ScreenXIn, float ScreenYIn, float ScreenWidthIn, float ScreenHeightIn);

public:
	void UpdateMap(const float& DeltaTimeIn);
	void RenderMap();

public:
	virtual void Serialize(const std::string& OutputAdditionalPath) override;
	virtual void Deserialize(FILE* FileIn, ID3D11Device* DeviceIn) override;

private:
	template<typename T, typename ...Args>
	T* RenderableAddHelper(Args... args)
	{
		Placeables.emplace_back(std::make_unique<T>(args...));
		static_assert(std::is_base_of<PlaceableObject, T>::value, "���ø� Ÿ���� PlaceableObject�� �Ļ� Ŭ�������� �մϴ�.");
		T* AddedObject = (T*)(Placeables.back().get());
		return AddedObject;
	}
};

