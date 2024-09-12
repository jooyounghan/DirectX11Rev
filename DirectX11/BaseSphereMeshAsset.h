#pragma once
#include "BaseMeshAsset.h"
#include "HeaderHelper.h"

class BaseSphereMeshAsset : public BaseMeshAsset
{
private:
	BaseSphereMeshAsset();
	virtual ~BaseSphereMeshAsset();
	MakeSingleton(BaseSphereMeshAsset);

public:
	static BaseSphereMeshAsset* GetInstance();
};

