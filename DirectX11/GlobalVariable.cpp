#include "GlobalVariable.h"

#include "imgui.h"

using namespace DirectX;

UINT App::GWidth = 0;
UINT App::GHeight = 0;
PortfolioApp* App::GApp = nullptr;
GraphicsPipeline* App::GGraphicPipeline = nullptr;
PSOManager* App::GPSOManager = nullptr;
InputEventManager* App::GInputEventManager = nullptr;

size_t Performance::GTotalIndexCount = 0;

const ImVec4 UIColor::GBlack = ImVec4(0.f, 0.f, 0.f, 1.f);
const ImVec4 UIColor::GWhite = ImVec4(1.f, 1.f, 1.f, 1.f);
const ImVec4 UIColor::GDarkGray = ImVec4(0.3f, 0.3f, 0.3f, 1.f);
const ImVec4 UIColor::GLightGray = ImVec4(0.55f, 0.55f, 0.55f, 1.f);
const ImVec4 UIColor::GDarkBrown = ImVec4(0.4f, 0.26f, 0.13f, 1.f);
const ImVec4 UIColor::GMediumBrown = ImVec4(0.7f, 0.5f, 0.34f, 1.f);
const ImVec4 UIColor::GLightBrown = ImVec4(1.f, 0.843f, 0.404f, 1.f);
const ImVec4 UIColor::GHilighted = ImVec4(0.2f, 0.3f, 0.4f, 1.0f);

const unsigned int NodeColor::VariableNodeBaseColor = IM_COL32(100, 180, 100, 180);
const unsigned int NodeColor::VariableNodeHilightedColor = IM_COL32(30, 200, 30, 180);
const unsigned int NodeColor::VariablePortBaseColor = IM_COL32(180, 180, 100, 180);
const unsigned int NodeColor::VariablePortHilightedColor = IM_COL32(200, 200, 30, 255);

const unsigned int NodeColor::FlowNodeBaseColor = IM_COL32(180, 100, 100, 180);
const unsigned int NodeColor::FlowNodeHilightedColor = IM_COL32(200, 30, 30, 180);
const unsigned int NodeColor::FlowPortBaseColor = IM_COL32(180, 100, 180, 180);
const unsigned int NodeColor::FlowPortHilightedColor = IM_COL32(200, 30, 200, 255);

const ImVec2 UISize::FileSize = ImVec2(100.f, 100.0f);
const ImVec2 UISize::WindowSize = ImVec2(800.f, 600.0f);

const char* DragDrop::GAsset = "DND_ASSET";
