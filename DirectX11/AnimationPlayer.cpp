#include "AnimationPlayer.h"
#include "BoneAsset.h"
#include "AnimationAsset.h"

#include "GlobalVariable.h"
#include "UploadableBufferManager.h"
#include "StructuredBuffer.h"

using namespace std;
using namespace DirectX;

AnimationPlayer::AnimationPlayer()
{
}

AnimationPlayer::~AnimationPlayer()
{
}

void AnimationPlayer::SetBoneAsset(const std::shared_ptr<BoneAsset>& BoneAssetIn)
{
	StopAnimation(false);

	if (AnimationTransformationBuffer != nullptr)
	{
		AnimationTransformationBuffer->BeginRemoveEvent.Invoke(AnimationTransformationBuffer.get());
		AnimationTransformationBuffer = nullptr;
	}

	BoneAssetCached = BoneAssetIn;

	const size_t BoneCounts = BoneAssetCached->GetNameToBones().size();
	AnimationTransformationBuffer = App::GUploadableBufferManager->CreateUploadableBuffer<StructuredBuffer<XMMATRIX>>(BoneCounts);
}

void AnimationPlayer::PlayAnimation(const shared_ptr<AnimationAsset>& AnimationAssetIn, const size_t& PlayCountIn)
{
	if (BoneAssetCached != nullptr)
	{
		AnimationAssetCached = AnimationAssetIn;
		PlayCount = PlayCountIn;
		PlayTime = 0.f;
	}
}

void AnimationPlayer::StopAnimation(const bool& IsRaiseEvent)
{
	AnimationAssetCached = nullptr;
	PlayCount = 0;
	PlayTime = 0.f;

	if (IsRaiseEvent) AnimationEndedEvent.Invoke();
}

inline bool AnimationPlayer::IsPlaying() { return (AnimationAssetCached != nullptr) && (PlayCount > 0); }

void AnimationPlayer::Update(const float& DeltaTimeIn)
{
	if (IsPlaying())
	{
		PlayTime += DeltaTimeIn * AnimationAssetCached->GetTicksPerSecond();

		const float Duration = AnimationAssetCached->GetDuration();
		const float Quotient = floorf(PlayTime / Duration);
		const float Remainder = PlayTime / Duration - Quotient;

		if (Quotient > 0)
		{
			PlayTime = PlayTime - Quotient * Duration;
			PlayCount--;
		}

		if (PlayCount > 0) // 플레이 중일 경우
		{

		}
		else // 플레이가 끝난 경우
		{
			StopAnimation();
		}
	}

	Bone* RootBone = BoneAssetCached->GetRootBone();
	UpdateBoneFromParent(nullptr, RootBone);

	unordered_map<string, Bone> NameToBones = BoneAssetCached->GetNameToBones();
	const vector<XMMATRIX>& StagedAnimationTransformation = AnimationTransformationBuffer->GetStagedStructuredData();

	for (const auto& NameToBone : NameToBones)
	{
		const Bone& BoneData = NameToBone.second;
		const size_t BoneIndex = BoneData.GetBoneIndex();

		AnimationTransformationBuffer->SetStagedData(BoneIndex, XMMatrixTranspose(BoneData.GetOffsetMatrix() * StagedAnimationTransformation[BoneIndex]));
	}

}

void AnimationPlayer::UpdateBoneFromParent(Bone* Parent, Bone* Child)
{
	const vector<XMMATRIX>& StagedAnimationTransformation = AnimationTransformationBuffer->GetStagedStructuredData();

	if (Child != nullptr)
	{
		const XMMATRIX& ChildTranformation = StagedAnimationTransformation[Child->GetBoneIndex()];
		const XMMATRIX ChildAnimationTransformation =
			AnimationAssetCached != nullptr ?
			AnimationAssetCached->GetAnimationTransformation(Child->GetBoneName(), PlayTime) :
			XMMatrixIdentity();

		if (Parent != nullptr)
		{
			const XMMATRIX& ParentTranformation = StagedAnimationTransformation[Parent->GetBoneIndex()];
			AnimationTransformationBuffer->SetStagedData(Child->GetBoneIndex(), ChildAnimationTransformation * ParentTranformation);
		}
		else
		{
			AnimationTransformationBuffer->SetStagedData(Child->GetBoneIndex(), ChildAnimationTransformation);
		}

		vector<Bone*> GrandChildren = Child->GetChildrenBones();
		for (Bone* GrandChild : GrandChildren)
		{
			UpdateBoneFromParent(Child, GrandChild);
		}
	}
}
