#include "GlobalVariable.h"


using namespace DirectX;

UINT App::GWidth = 0;
UINT App::GHeight = 0;
PortfolioApp* App::GApp = nullptr;
GraphicsPipeline* App::GGraphicPipeline = nullptr;
PSOManager* App::GPSOManager = nullptr;
InputEventManager* App::GInputEventManager = nullptr;
UploadableBufferManager* App::GUploadableBufferManager = nullptr;

size_t Performance::GTotalIndexCount = 0;