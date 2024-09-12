#pragma once
#include "BaseMeshAsset.h"
#include "HeaderHelper.h"

class BaseCubeMeshAsset : public BaseMeshAsset
{
private:
	BaseCubeMeshAsset();
	virtual ~BaseCubeMeshAsset();
	MakeSingleton(BaseCubeMeshAsset);

public:
	static BaseCubeMeshAsset* GetInstance();
};