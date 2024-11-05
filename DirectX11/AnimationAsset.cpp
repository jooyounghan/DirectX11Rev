#include "AnimationAsset.h"

using namespace std;
using namespace DirectX;

string AnimationAsset::AnimationKind = "Animation";

AnimationKey::AnimationKey() : Time(0.f), AnimationData(XMVectorZero())
{
}

AnimationKey::AnimationKey(const float& TimeIn, const XMVECTOR& AnimationDataIn)
	: Time(TimeIn), AnimationData(AnimationDataIn)
{
}

void AnimationKey::OnSerialize(FILE* FileIn)
{
	fwrite(&Time, sizeof(float), 1, FileIn);
	fwrite(&AnimationData, sizeof(XMVECTOR), 1, FileIn);
}

void AnimationKey::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	fread(&Time, sizeof(float), 1, FileIn);
	fread(&AnimationData, sizeof(XMVECTOR), 1, FileIn);
}


AnimationChannel::AnimationChannel()
{
}

AnimationChannel::~AnimationChannel()
{
}

XMVECTOR AnimationChannel::GetLerpedVectorKey(const float& TimeIn, const std::vector<AnimationKey>& Keys)
{
	if (Keys.empty())
	{
		return XMVECTOR();
	}

	for (size_t idx = 0; idx < Keys.size() - 1; ++idx)
	{
		const AnimationKey& KeyFrom = Keys[idx];
		const AnimationKey& KeyTo = Keys[idx + 1];
		const float TimeFrom = KeyFrom.GetTime();
		const float TimeTo = KeyTo.GetTime();

		if (TimeFrom - AnimationTimeErr < TimeIn && TimeIn < TimeTo + AnimationTimeErr)
		{
			return XMVectorLerp(KeyFrom.GetAnimationData(), KeyTo.GetAnimationData(), (TimeIn - TimeFrom) / (TimeTo - TimeFrom));
		}
	}

	return Keys[0].GetAnimationData();
}

void AnimationChannel::OnSerialize(FILE* FileIn)
{
	size_t PositionCounts = PositionKeys.size();
	size_t QuaternionCounts = QuaternionKeys.size();
	size_t ScaleCounts = ScaleKeys.size();

	fwrite(&PositionCounts, sizeof(size_t), 1, FileIn);
	for (AnimationKey& PositionKey : PositionKeys)
	{
		PositionKey.OnSerialize(FileIn);
	}
		
	fwrite(&QuaternionCounts, sizeof(size_t), 1, FileIn);
	for (AnimationKey& QuaternionKey : QuaternionKeys)
	{
		QuaternionKey.OnSerialize(FileIn);
	}

	fwrite(&ScaleCounts, sizeof(size_t), 1, FileIn);
	for (AnimationKey& ScaleKey : ScaleKeys)
	{
		ScaleKey.OnSerialize(FileIn);
	}
}

void AnimationChannel::OnDeserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	size_t PositionCounts;
	size_t QuaternionCounts;
	size_t ScaleCounts;

	fread(&PositionCounts, sizeof(size_t), 1, FileIn);
	for (size_t idx = 0; idx < PositionCounts; ++idx)
	{
		AnimationKey PositionKey;
		PositionKey.OnDeserialize(FileIn, AssetManagerIn);
		PositionKeys.push_back(std::move(PositionKey));
	}

	fread(&QuaternionCounts, sizeof(size_t), 1, FileIn);
	for (size_t idx = 0; idx < QuaternionCounts; ++idx)
	{
		AnimationKey QuaternionKey;
		QuaternionKey.OnDeserialize(FileIn, AssetManagerIn);
		QuaternionKeys.push_back(std::move(QuaternionKey));
	}


	fread(&ScaleCounts, sizeof(size_t), 1, FileIn);
	for (size_t idx = 0; idx < ScaleCounts; ++idx)
	{
		AnimationKey ScaleKey;
		ScaleKey.OnDeserialize(FileIn, AssetManagerIn);
		ScaleKeys.push_back(std::move(ScaleKey));
	}

}

void AnimationChannel::AddPositionKey(float TimeIn, DirectX::XMVECTOR PositionIn)
{
	PositionKeys.emplace_back(TimeIn, PositionIn);
}

void AnimationChannel::AddQuaternionKey(float TimeIn, DirectX::XMVECTOR QuaternionIn)
{
	QuaternionKeys.emplace_back(TimeIn, QuaternionIn);
}

void AnimationChannel::AddScaleKey(float TimeIn, DirectX::XMVECTOR ScaleIn)
{
	ScaleKeys.emplace_back(TimeIn, ScaleIn);
}

DirectX::XMVECTOR AnimationChannel::GetLerpedPositionKey(const float& TimeIn) const
{
	return GetLerpedVectorKey(TimeIn, PositionKeys);
}

XMVECTOR AnimationChannel::GetSLerpedQuaternionKey(const float& TimeIn) const
{
	if (QuaternionKeys.empty())
	{
		return XMVECTOR();
	}

	for (size_t idx = 0; idx < QuaternionKeys.size() - 1; ++idx)
	{
		const AnimationKey& KeyFrom = QuaternionKeys[idx];
		const AnimationKey& KeyTo = QuaternionKeys[idx + 1];
		const float TimeFrom = KeyFrom.GetTime();
		const float TimeTo = KeyTo.GetTime();

		if (TimeFrom - AnimationTimeErr < TimeIn && TimeIn < TimeTo + AnimationTimeErr)
		{
			return XMQuaternionSlerp(KeyFrom.GetAnimationData(), KeyTo.GetAnimationData(), (TimeIn - TimeFrom) / (TimeTo - TimeFrom));
		}
	}

	return QuaternionKeys[0].GetAnimationData();
}

DirectX::XMVECTOR AnimationChannel::GetLerpedScaleKey(const float& TimeIn) const
{
	return GetLerpedVectorKey(TimeIn, ScaleKeys);
}

AnimationAsset::AnimationAsset(
	const std::string& AssetNameIn, 
	const bool& LoadFromAsset,
	const float& DurationIn,
	const float& TicksPerSecondIn
)
	: AAssetFile(LoadFromAsset ? AssetNameIn : AssetNameIn + "_" + AnimationAsset::AnimationKind, AnimationAsset::AnimationKind),
	Duration(DurationIn), TicksPerSecond(TicksPerSecondIn)
{
}

AnimationAsset::~AnimationAsset()
{
}

void AnimationAsset::AddAnimationChannel(const std::string& ChannelName, const AnimationChannel& AnimChannel)
{
	ChannelNameToAnimationChannels.emplace(ChannelName, AnimChannel);
}

void AnimationAsset::AddAnimationChannel(const std::string& ChannelName, AnimationChannel&& AnimChannel)
{
	ChannelNameToAnimationChannels.emplace(ChannelName, AnimChannel);
}

XMMATRIX AnimationAsset::GetAnimationTransformation(const std::string& ChannelName, const float& TimeIn)
{
	if (ChannelNameToAnimationChannels.find(ChannelName) != ChannelNameToAnimationChannels.end())
	{
		const AnimationChannel& CurrentAnimationChannel = ChannelNameToAnimationChannels[ChannelName];

		return XMMatrixAffineTransformation(
			CurrentAnimationChannel.GetLerpedScaleKey(TimeIn),
			XMVectorZero(),
			CurrentAnimationChannel.GetSLerpedQuaternionKey(TimeIn),
			CurrentAnimationChannel.GetLerpedPositionKey(TimeIn)
		);				
	}
	return XMMatrixIdentity();
}

void AnimationAsset::Serialize()
{
	FILE* OutputAssetFile = nullptr;
	string OutputAssetFilePath = DefaultOpenFileHelper(AnimationAssetOutPath, OutputAssetFile);

	if (OutputAssetFile != nullptr)
	{
		SerializeHeader(OutputAssetFile);

		fwrite(&Duration, sizeof(float), 1, OutputAssetFile);
		fwrite(&TicksPerSecond, sizeof(float), 1, OutputAssetFile);

		size_t ChannelNameToAnimationChannelsCount = ChannelNameToAnimationChannels.size();
		fwrite(&ChannelNameToAnimationChannelsCount, sizeof(size_t), 1, OutputAssetFile);

		for (auto& ChannelNameToAnimationChannel : ChannelNameToAnimationChannels)
		{
			AAssetFile::SerializeString(ChannelNameToAnimationChannel.first, OutputAssetFile);
			ChannelNameToAnimationChannel.second.OnSerialize(OutputAssetFile);
		}

		fclose(OutputAssetFile);
	}
}

void AnimationAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
	fread(&Duration, sizeof(float), 1, FileIn);
	fread(&TicksPerSecond, sizeof(float), 1, FileIn);

	size_t ChannelNameToAnimationChannelsCount;
	fread(&ChannelNameToAnimationChannelsCount, sizeof(size_t), 1, FileIn);

	for (size_t AnimationChannelIdx = 0; AnimationChannelIdx < ChannelNameToAnimationChannelsCount; ++AnimationChannelIdx)
	{
		string ChannelName;
		AAssetFile::DeserializeString(ChannelName, FileIn);
		ChannelNameToAnimationChannels[ChannelName].OnDeserialize(FileIn, AssetManagerIn);
	}

}

