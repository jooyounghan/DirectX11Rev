
#include "AnimationAsset.h"

using namespace std;
using namespace DirectX;

constexpr float AnimationTimeErr = 1E-3f;

function<XMVECTOR(const XMVECTOR&, const XMVECTOR&, const float&)> AnimChannel::lerpFunction 
	= [](const XMVECTOR& A, const XMVECTOR& B, const float& C) -> XMVECTOR {
		return XMVectorLerp(A, B, C);
	};

function<XMVECTOR(const XMVECTOR&, const XMVECTOR&, const float&)> AnimChannel::slerpFunction
	= [](const XMVECTOR& A, const XMVECTOR& B, const float& C) -> XMVECTOR {
			return XMQuaternionSlerp(A, B, C);
	};

void AnimChannel::AddPositionKey(const float& timeIn, const XMVECTOR& positionIn)
{
	m_timeTable.insert(timeIn);
	AddKey(m_positionKeys, timeIn, positionIn);
}

void AnimChannel::AddQuaternionKey(const float& timeIn, const XMVECTOR& quaternionIn)
{
	m_timeTable.insert(timeIn);
	AddKey(m_quaternionKeys, timeIn, quaternionIn);
}

void AnimChannel::AddScaleKey(const float& timeIn, const XMVECTOR& scaleIn)
{
	m_timeTable.insert(timeIn);
	AddKey(m_scaleKeys, timeIn, scaleIn);
}

void AnimChannel::AddKey(
	vector<SAnimationKey>& keys, 
	const float& timeIn, 
	const DirectX::XMVECTOR& keyData
)
{
	SAnimationKey animationKey{ timeIn, keyData };
	keys.emplace_back(move(animationKey));
}

DirectX::XMVECTOR AnimChannel::GetLerpedVectorKey(
	const vector<SAnimationKey>& keys,
	const float& timeIn,
	const function<XMVECTOR(const XMVECTOR&, const XMVECTOR&, const float&)>& interpolationMethod
)
{
	if (keys.empty())
	{
		return XMVECTOR();
	}

	for (size_t idx = 0; idx < keys.size() - 1; ++idx)
	{
		const SAnimationKey& keyFrom = keys[idx];
		const SAnimationKey& keyTo = keys[idx + 1];

		if (keyFrom.m_time - AnimationTimeErr < timeIn && timeIn < keyTo.m_time + AnimationTimeErr)
		{
			return interpolationMethod(keyFrom.m_animationData, keyTo.m_animationData, (timeIn - keyFrom.m_time) / (keyTo.m_time - keyFrom.m_time));
		}
	}

	return keys[0].m_animationData;
}

DirectX::XMMATRIX AnimChannel::GetLocalTransformation(const float& timeIn) const
{
	return XMMatrixAffineTransformation(
		GetLerpedVectorKey(m_scaleKeys, timeIn, lerpFunction),
		XMVectorZero(),
		GetLerpedVectorKey(m_quaternionKeys, timeIn, slerpFunction),
		GetLerpedVectorKey(m_positionKeys, timeIn, lerpFunction)
	);
}


void AnimChannel::Serialize(FILE* fileIn) const
{
	SerializeHelper::SerializeVectorContainer(m_positionKeys, fileIn);
	SerializeHelper::SerializeVectorContainer(m_quaternionKeys, fileIn);
	SerializeHelper::SerializeVectorContainer(m_scaleKeys, fileIn);
	SerializeHelper::SerializeIterableContainer(m_timeTable, fileIn);
}

void AnimChannel::Deserialize(FILE* fileIn)
{
	m_positionKeys = DeserializeHelper::DeserializeVectorContainer<vector<SAnimationKey>>(fileIn);
	m_quaternionKeys = DeserializeHelper::DeserializeVectorContainer<vector<SAnimationKey>>(fileIn);
	m_scaleKeys = DeserializeHelper::DeserializeVectorContainer<vector<SAnimationKey>>(fileIn);
	m_timeTable = DeserializeHelper::DeserializeIterableContainer<set<float>>(fileIn);
}

AnimationAsset::AnimationAsset(const string& assetName)
	: AAsset(assetName)
{
}

void AnimationAsset::SetAnimationDuration(const float& durationIn, const float& ticksPerSecond)
{
	m_duration = durationIn;
	m_ticksPerSecond = ticksPerSecond;
}

void AnimationAsset::AddAnimChaannel(const string& boneName, const AnimChannel& animChannel)
{
	m_boneNameToAnimChannels.emplace(boneName, animChannel);
}

void AnimationAsset::AddAnimChannel(const string& boneName, AnimChannel&& animChannel)
{
	m_boneNameToAnimChannels.emplace(boneName, move(animChannel));
}

const AnimChannel* AnimationAsset::GetAnimChannel(const std::string& boneName) const
{
	const AnimChannel* animChannel = nullptr;
	if (m_boneNameToAnimChannels.find(boneName) != m_boneNameToAnimChannels.end())
	{
		animChannel = &m_boneNameToAnimChannels.at(boneName);
	}
	return animChannel;
}

void AnimationAsset::Serialize(FILE* fileIn) const
{
	AAsset::Serialize(fileIn);

	SerializeHelper::SerializeElement(m_duration, fileIn);
	SerializeHelper::SerializeElement(m_ticksPerSecond, fileIn);

	SerializeHelper::SerializeContainerSize(m_boneNameToAnimChannels, fileIn);
	for (auto& boneNameToAnimChannel : m_boneNameToAnimChannels)
	{
		const string& boneName = boneNameToAnimChannel.first;
		const AnimChannel& animChannel = boneNameToAnimChannel.second;

		SerializeHelper::SerializeString(boneName, fileIn);
		animChannel.Serialize(fileIn);
	}
}

void AnimationAsset::Deserialize(FILE* fileIn)
{
	AAsset::Deserialize(fileIn);
	m_duration = DeserializeHelper::DeserializeElement<float>(fileIn);
	m_ticksPerSecond = DeserializeHelper::DeserializeElement<float>(fileIn);

	size_t containerSize = DeserializeHelper::DeserializeElement<size_t>(fileIn);
	for (size_t containerIdx = 0; containerIdx < containerSize; ++containerIdx)
	{
		string boneName = DeserializeHelper::DeserializeString(fileIn);
		AnimChannel animChannel;
		animChannel.Deserialize(fileIn);
		m_boneNameToAnimChannels.emplace(boneName, animChannel);
	}
}

void AnimationAsset::Accept(IAssetVisitor* visitor)
{
	visitor->Visit(this);
}
