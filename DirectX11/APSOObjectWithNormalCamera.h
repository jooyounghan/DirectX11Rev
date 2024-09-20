#pragma once
#include "APSOObject.h"

class Camera;

class APSOObjectWithNormalCamera : public APSOObject
{
public:
	APSOObjectWithNormalCamera(PSOArgsIn) : APSOObject(PSOArgs) {};
	virtual ~APSOObjectWithNormalCamera() {};

public:
	virtual void PresetPSO(Camera* CameraIn) = 0;

};

