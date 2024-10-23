#pragma once
#include "ACamera.h"

class SDRCamera : public ACamera
{
public:
	SDRCamera(
		MapAsset* MapAssetInstance, const UINT& WidthIn, const UINT& HeightIn);
	virtual ~SDRCamera();

public:
	static std::string SDRCameraKind;

public:
	virtual void CleanupLens() override;
};

