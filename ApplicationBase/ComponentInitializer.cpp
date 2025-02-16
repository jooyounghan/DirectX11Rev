#include "ComponentInitializer.h"

#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "CameraComponent.h"
#include "SphereCollisionComponent.h"
#include "OrientedBoxCollisionComponent.h"

#include "SpotLightComponent.h"
#include "PointLightComponent.h"

#include "SkeletalMeshAsset.h"
#include "AnimationPlayer.h"

#include "ConstantBuffer.h"
#include "DynamicBuffer.h"

#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"
#include "DSVOption.h"

ComponentInitializer::ComponentInitializer(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	: m_deviceCached(device), m_deviceContextCached(deviceContext)
{
}

void ComponentInitializer::Visit(StaticMeshComponent* staticModelComponent)
{
	InitBaseComponent(staticModelComponent);
}

void ComponentInitializer::Visit(SkeletalMeshComponent* skeletalModelComponent)
{
	InitBaseComponent(skeletalModelComponent);

	const SkeletalMeshAsset* skeletalMetalAsset = skeletalModelComponent->GetSkeletalMetalAsset();
	if (skeletalMetalAsset != nullptr)
	{
		AnimationPlayer* animationPlayer = new AnimationPlayer(skeletalModelComponent, skeletalMetalAsset->GetBoneAsset());
		skeletalModelComponent->SetAnimationPlayer(animationPlayer);
		animationPlayer->InitAnimationPlayer(m_deviceCached);
	}
}

void ComponentInitializer::Visit(CameraComponent* cameraComponent)
{
	InitBaseComponent(cameraComponent);
	InitViewComponent(cameraComponent);

	cameraComponent->SetFilm(new Texture2DInstance<SRVOption, RTVOption, UAVOption>(
		static_cast<uint32_t>(cameraComponent->Width), static_cast<uint32_t>(cameraComponent->Height),
		1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, m_deviceCached, m_deviceContextCached
	));

	cameraComponent->SetIDFilm(new Texture2DInstance<RTVOption>(
		static_cast<uint32_t>(cameraComponent->Width), static_cast<uint32_t>(cameraComponent->Height),
		1, 1, NULL ,NULL , D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_UINT, m_deviceCached, m_deviceContextCached
	));

	cameraComponent->SetIDStagingFilm(new Texture2DInstance<PureTextureOption>(
		1, 1, 1, 1, D3D11_CPU_ACCESS_READ, NULL, D3D11_USAGE_STAGING, DXGI_FORMAT_R32_UINT,
		m_deviceCached, m_deviceContextCached
	));

	cameraComponent->SetDepthStencilView(new Texture2DInstance<DSVOption>(
		static_cast<uint32_t>(cameraComponent->Width), static_cast<uint32_t>(cameraComponent->Height),
		1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_D24_UNORM_S8_UINT, m_deviceCached, m_deviceContextCached
	));

}

void ComponentInitializer::Visit(SphereCollisionComponent* sphereCollisionComponent)
{
	InitBaseComponent(sphereCollisionComponent);
}

void ComponentInitializer::Visit(OrientedBoxCollisionComponent* orientedBoxCollisionComponent)
{
	InitBaseComponent(orientedBoxCollisionComponent);
}

void ComponentInitializer::Visit(SpotLightComponent* spotLightComponent)
{
	InitBaseComponent(spotLightComponent);
	InitViewComponent(spotLightComponent);
	InitLightComponent(spotLightComponent);

	spotLightComponent->SetDepthTestView(new Texture2DInstance<SRVOption, DSVOption>(
		static_cast<uint32_t>(spotLightComponent->Width), static_cast<uint32_t>(spotLightComponent->Height),
		1, 1, NULL, NULL, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_TYPELESS, m_deviceCached, m_deviceContextCached
	));
}

void ComponentInitializer::Visit(PointLightComponent* pointLightComponent)
{
	InitBaseComponent(pointLightComponent);
	InitLightComponent(pointLightComponent);

	Texture2DInstance<SRVOption>* depthStencilViewCube = new Texture2DInstance<SRVOption>(
		static_cast<uint32_t>(pointLightComponent->GetWidth()), static_cast<uint32_t>(pointLightComponent->GetHeight()),
		6, 1, NULL, D3D11_RESOURCE_MISC_TEXTURECUBE, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R32_TYPELESS, 
		m_deviceCached, m_deviceContextCached, D3D11_BIND_DEPTH_STENCIL
	);

	pointLightComponent->SetDepthTestViewCube(depthStencilViewCube);

	for (size_t idx = 0; idx < 6; ++idx)
	{
		PointLightFrustumPart* pointLightFrustumPart = pointLightComponent->GetPointLightFrustumPart(idx);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(idx);
		dsvDesc.Texture2DArray.ArraySize = 1;
		m_deviceCached->CreateDepthStencilView(depthStencilViewCube->GetTexture2D(), &dsvDesc, pointLightFrustumPart->GetDepthTestDSVAddress());

		DynamicBuffer* viewProjBuffer = pointLightFrustumPart->GetViewProjMatrixBuffer();
		viewProjBuffer->InitializeBuffer(m_deviceCached);
	}
}

void ComponentInitializer::InitBaseComponent(AComponent* component)
{
	component->UpdateAbsoluteEntities();
	component->UpdateComponentTransformation();
	DynamicBuffer* transformationBuffer = component->GetTransformationBuffer();
	DynamicBuffer* comopnentBuffer = component->GetComponentBuffer();

	transformationBuffer->InitializeBuffer(m_deviceCached);
	comopnentBuffer->InitializeBuffer(m_deviceCached);
}

void ComponentInitializer::InitViewComponent(AViewComponent* viewComponent)
{
	viewComponent->UpdateViewEntity();
	DynamicBuffer* viewProjMatrixBuffer = viewComponent->GetViewProjMatrixBuffer();
	viewProjMatrixBuffer->InitializeBuffer(m_deviceCached);
}

void ComponentInitializer::InitLightComponent(LightEntity* lightEntity)
{
	DynamicBuffer* lightBuffer = lightEntity->GetLightEntityBuffer();
	lightBuffer->InitializeBuffer(m_deviceCached);
}