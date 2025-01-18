#include "ComponentEntityInitializer.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"

#include "SkeletalMeshAsset.h"

#include "AnimationPlayer.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"

#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

ComponentEntityInitializer::ComponentEntityInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_deviceCached(device), m_deviceContextCached(deviceContext)
{
}

void ComponentEntityInitializer::Visit(StaticMeshComponent* staticModelComponent)
{
	staticModelComponent->UpdateAbsoluteEntities();
	InitBaseEntityBuffer(staticModelComponent);
}

void ComponentEntityInitializer::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	skeletalModelComponent->UpdateAbsoluteEntities();
	InitBaseEntityBuffer(skeletalModelComponent);

	// AnimationPlayerComponent?
	const SkeletalMeshAsset* skeletalMetalAsset = skeletalModelComponent->GetSkeletalMetalAsset();
	if (skeletalMetalAsset != nullptr)
	{
		AnimationPlayer* animationPlayer = new AnimationPlayer(skeletalMetalAsset->GetBoneAsset());
		skeletalModelComponent->SetAnimationPlayer(animationPlayer);
		animationPlayer->InitAnimationPlayer(m_deviceCached);
	}
}

void ComponentEntityInitializer::Visit(CameraComponent* cameraComponent)
{
	cameraComponent->UpdateAbsoluteEntities();
	cameraComponent->UpdateViewElement();
	InitBaseEntityBuffer(cameraComponent);

	DynamicBuffer* viewProjMatrixBuffer = cameraComponent->GetViewProjMatrixBuffer();
	D3D11_SUBRESOURCE_DATA viewProjSubResourceData = viewProjMatrixBuffer->GetSubResourceData();
	viewProjMatrixBuffer->InitializeBuffer(m_deviceCached, &viewProjSubResourceData);

	cameraComponent->SetFilm(new Texture2DInstance<SRVOption, RTVOption, UAVOption>(
		static_cast<uint32_t>(cameraComponent->Width), static_cast<uint32_t>(cameraComponent->Height),
		1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, m_deviceCached, m_deviceContextCached
	));

	cameraComponent->SetIDFilm(new Texture2DInstance<RTVOption>(
		static_cast<uint32_t>(cameraComponent->Width), static_cast<uint32_t>(cameraComponent->Height),
		1, 1, NULL ,NULL , D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UINT, m_deviceCached, m_deviceContextCached
	));

	cameraComponent->SetIDStagingFilm(new Texture2DInstance<PureTextureOption>(
		1, 1, 1, 1, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, DXGI_FORMAT_R8G8B8A8_UINT, 
		m_deviceCached, m_deviceContextCached
	));

	cameraComponent->SetDepthStencilView(new Texture2DInstance<DSVOption>(
		static_cast<uint32_t>(cameraComponent->Width), static_cast<uint32_t>(cameraComponent->Height),
		1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_D24_UNORM_S8_UINT, m_deviceCached, m_deviceContextCached
	));

}

void ComponentEntityInitializer::InitBaseEntityBuffer(AComponent* component)
{
	component->UpdateComponentTransformation();
	DynamicBuffer* transformationBuffer = component->GetTransformationBuffer();
	ConstantBuffer* comopnentBuffer = component->GetComponentBuffer();

	const D3D11_SUBRESOURCE_DATA transformaionSubresource = transformationBuffer->GetSubResourceData();
	transformationBuffer->InitializeBuffer(m_deviceCached, &transformaionSubresource);

	const D3D11_SUBRESOURCE_DATA componentSubresource = comopnentBuffer->GetSubResourceData();
	comopnentBuffer->InitializeBuffer(m_deviceCached, &componentSubresource);

}
