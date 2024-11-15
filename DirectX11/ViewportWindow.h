#pragma once
#include "AWindow.h"
#include "HeaderHelper.h"
#include "Delegation.h"

#include <memory>

struct Ray;
struct ImVec2;
struct XMFLOAT3;

class AAssetFile;
class ACamera;

typedef std::function<void(const unsigned int&)> IDSelectHandler;
typedef std::function<void(AAssetFile*, const float&, const float&, const float&)> AssetDropHandler;

class ViewportWindow : public AWindow
{
public: 
	ViewportWindow();
	virtual ~ViewportWindow();

public:
	virtual void RenderWindow() override;

protected:
	ACamera* CurrentCamera = nullptr;
	MakeSetter(CurrentCamera);

public:
	Delegation<const unsigned int&> IDSelectEvent;
	Delegation<AAssetFile*, const float&, const float&, const float&> AssetDropEvent;

private:
	ImVec2 ImagePosition = ImVec2();
	ImVec2 ImageSize = ImVec2();
};

