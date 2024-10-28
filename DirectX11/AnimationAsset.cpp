#include "AnimationAsset.h"

using namespace std;
using namespace DirectX;

string AnimationAsset::AnimationKind = "Animation";

AnimationKey::AnimationKey(const float& TimeIn, const XMVECTOR& AnimationDataIn)
	: Time(TimeIn), AnimationData(AnimationDataIn)
{
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
}

void AnimationAsset::Deserialize(FILE* FileIn, AssetManager* AssetManagerIn)
{
}

