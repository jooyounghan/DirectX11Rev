#include "ARenderer.h"
#include "GlobalVariable.h"
#include "GraphicsPipeline.h"
ARenderer::ARenderer(PSOObject* PSOObjectIn)
	: PSOObjectCached(PSOObjectIn), DeviceContextCached(App::GGraphicPipeline->GetDeviceContext())
{

}
