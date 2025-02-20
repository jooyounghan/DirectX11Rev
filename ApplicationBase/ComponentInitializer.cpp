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

	DynamicBuffer& viewProjMatrixBuffer = cameraComponent->GetViewEntityBuffer();
	viewProjMatrixBuffer.InitializeBuffer(m_deviceCached);

	auto& film = cameraComponent->GetFilm();
	auto& idFilm = cameraComponent->GetIDFilm();
	auto& idStagingFilm = cameraComponent->GetIDStatgingFilm();
	auto& depthStencilView = cameraComponent->GetDepthStencilView();

	film.InitializeByOption(film.GetTexture2D(), m_deviceCached, m_deviceContextCached);
	idFilm.InitializeByOption(idFilm.GetTexture2D(), m_deviceCached, m_deviceContextCached);
	idStagingFilm.InitializeByOption(idStagingFilm.GetTexture2D(), m_deviceCached, m_deviceContextCached);
	depthStencilView.InitializeByOption(depthStencilView.GetTexture2D(), m_deviceCached, m_deviceContextCached);
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

	auto& depthTestView = spotLightComponent->GetDepthTestView();
	depthTestView.InitializeByOption(depthTestView.GetTexture2D(), m_deviceCached, m_deviceContextCached);
}

void ComponentInitializer::Visit(PointLightComponent* pointLightComponent)
{
	InitBaseComponent(pointLightComponent);

	auto& deptTestViewCube = pointLightComponent->GetDepthTestViewCube();
	deptTestViewCube.InitializeByOption(deptTestViewCube.GetTexture2D(), m_deviceCached, m_deviceContextCached);

	for (size_t idx = 0; idx < 6; ++idx)
	{
		PointLightFrustum& pointLightFrustum = pointLightComponent->GetPointLightFrustum(idx);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
		dsvDesc.Texture2DArray.MipSlice = 0;
		dsvDesc.Texture2DArray.FirstArraySlice = static_cast<UINT>(idx);
		dsvDesc.Texture2DArray.ArraySize = 1;
		m_deviceCached->CreateDepthStencilView(deptTestViewCube.GetTexture2D(), &dsvDesc, pointLightFrustum.GetDepthTestDSVAddress());
	}
}

void ComponentInitializer::InitBaseComponent(AComponent* component)
{
	DynamicBuffer& transformationEntityBuffer = component->GetTransformationEntityBuffer();
	DynamicBuffer& comopnentEntityBuffer = component->GetComponentEntityBuffer();

	transformationEntityBuffer.InitializeBuffer(m_deviceCached);
	comopnentEntityBuffer.InitializeBuffer(m_deviceCached);
}