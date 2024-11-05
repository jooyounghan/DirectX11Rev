#pragma once
#include "AAssetFile.h"
#include "directxmath/DirectXMath.h"

constexpr float AnimationTimeErr = 1E-3f;
constexpr const char* AnimationAssetOutPath = ".\\Assets\\Animation\\";

class AnimationKey : public IOnSerializableElement
{
public:
	AnimationKey();
	AnimationKey(const float& TimeIn, const DirectX::XMVECTOR& AnimationDataIn);

protected:
	float Time;
	DirectX::XMVECTOR AnimationData;
	MakeGetter(Time);
	MakeGetter(AnimationData);	

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

class AnimationChannel : public IOnSerializableElement
{
public:
	AnimationChannel();
	~AnimationChannel();

protected:
	std::vector<AnimationKey> PositionKeys;
	std::vector<AnimationKey> QuaternionKeys;
	std::vector<AnimationKey> ScaleKeys;

public:
	void AddPositionKey(float TimeIn, DirectX::XMVECTOR PositionIn);
	void AddQuaternionKey(float TimeIn, DirectX::XMVECTOR QuaternionIn);
	void AddScaleKey(float TimeIn, DirectX::XMVECTOR ScaleIn);

public:
	DirectX::XMVECTOR GetLerpedPositionKey (const float& TimeIn) const;
	DirectX::XMVECTOR GetSLerpedQuaternionKey (const float& TimeIn) const;
	DirectX::XMVECTOR GetLerpedScaleKey(const float& TimeIn) const;

private:
	static DirectX::XMVECTOR GetLerpedVectorKey(const float& TimeIn, const std::vector<AnimationKey>& Keys);

public:
	virtual void OnSerialize(FILE* FileIn) override;
	virtual void OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};

class AnimationAsset : public AAssetFile
{
public:
	AnimationAsset(
		const std::string& AssetNameIn, 
		const bool& LoadFromAsset,
		const float& DurationIn = 0.f,
		const float& TicksPerSecondIn = 0.f
	);
	~AnimationAsset();

public:
	static std::string AnimationKind;

protected:
	float Duration;
	float TicksPerSecond;
	MakeGetter(Duration);
	MakeGetter(TicksPerSecond);

protected:
	std::unordered_map<std::string, AnimationChannel> ChannelNameToAnimationChannels;

public:
	void AddAnimationChannel(const std::string& ChannelName, const AnimationChannel& AnimChannel);
	void AddAnimationChannel(const std::string& ChannelName, AnimationChannel&& AnimChannel);

public:
	DirectX::XMMATRIX GetAnimationTransformation(const std::string& ChannelName, const float& TimeIn);

public:
	virtual void Serialize() override;
	virtual void Deserialize(FILE* FileIn, AssetManager* AssetManagerIn) override;
};
