#pragma once
#include "Actor.h"

class BaseSphereMeshAsset;
class EXRTextureAsset;
class DDSTextureAsset;
class NormalTextureAsset;

class EnvironmentActor : public Actor
{
public:
	EnvironmentActor();
	virtual ~EnvironmentActor();

protected:
	BaseSphereMeshAsset* EnvironmentMeshAsset;
	MakeSetterGetter(EnvironmentMeshAsset);

protected:
	std::shared_ptr<EXRTextureAsset> EnvironmentBackgroundEXRTextureAsset;
	std::shared_ptr<DDSTextureAsset> EnvironmentSpecularDDSTextureAsset;
	std::shared_ptr<DDSTextureAsset> EnvironmentDiffuseDDSTextureAsset;
	std::shared_ptr<DDSTextureAsset> EnvironmentBRDFDDSTextureAsset;
};

