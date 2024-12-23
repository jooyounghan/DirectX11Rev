#pragma once
#include "AComponent.h"
#include "ViewEntity.h"
#include "Texture2DInstance.h"
#include "RTVOption.h"
#include "SRVOption.h"
#include "UAVOption.h"

class CameraComponent : public AComponent, public ViewEntity
{
public:
	CameraComponent(
		const uint64_t& componentID, DirectX::XMFLOAT3 position,
		const float& width, const float& height,
		const float& nearZ, const float& farZ, const float& fovAngle
	);

protected:
	Texture2DInstance<SRVOption, RTVOption, UAVOption>* m_film = nullptr;

public:
	virtual void InitEntity(ID3D11Device* device) override;
};

