#include "GlobalVariable.h"

#include "imgui.h"

using namespace DirectX;

UINT App::GWidth = 0;
UINT App::GHeight = 0;

XMVECTOR Direction::GDefaultForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
XMVECTOR Direction::GDefaultUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
XMVECTOR Direction::GDefaultRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);

ImVec4 Color::GBlack = ImVec4(0.f, 0.f, 0.f, 1.f);
ImVec4 Color::GWhite = ImVec4(1.f, 1.f, 1.f, 1.f);
ImVec4 Color::GDarkGray = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
ImVec4 Color::GLightGray = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
ImVec4 Color::GDarkBrown = ImVec4(0.4f, 0.26f, 0.13f, 1.f);
ImVec4 Color::GMediumBrown = ImVec4(0.7f, 0.5f, 0.34f, 1.f);
ImVec4 Color::GLightBrown = ImVec4(1.f, 0.843f, 0.404f, 1.f);

const char* DragDrop::GAsset = "DND_ASSET";
