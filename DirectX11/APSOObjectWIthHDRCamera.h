#pragma once
#include "APSOObject.h"

class HDRCamera;

class APSOObjectWIthHDRCamera : public APSOObject
{
public:
	APSOObjectWIthHDRCamera(PSOArgsIn) : APSOObject(PSOArgs) {};
	virtual ~APSOObjectWIthHDRCamera() {};

public:
	virtual void PresetPSO(HDRCamera* CameraIn) = 0;
};

