#include "GlobalVariable.h"

#include "imgui.h"

using namespace DirectX;

UINT App::GWidth = 0;
UINT App::GHeight = 0;

const ImVec4 UIColor::GBlack = ImVec4(0.f, 0.f, 0.f, 1.f);
const ImVec4 UIColor::GWhite = ImVec4(1.f, 1.f, 1.f, 1.f);
const ImVec4 UIColor::GDarkGray = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
const ImVec4 UIColor::GLightGray = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
const ImVec4 UIColor::GDarkBrown = ImVec4(0.4f, 0.26f, 0.13f, 1.f);
const ImVec4 UIColor::GMediumBrown = ImVec4(0.7f, 0.5f, 0.34f, 1.f);
const ImVec4 UIColor::GLightBrown = ImVec4(1.f, 0.843f, 0.404f, 1.f);

const ImVec2 UISize::FileSize = ImVec2(100.f, 100.0f);

const char* DragDrop::GAsset = "DND_ASSET";
