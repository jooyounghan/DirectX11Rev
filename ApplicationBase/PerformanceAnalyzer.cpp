#include "PerformanceAnalyzer.h"

size_t PerformanceAnalyzer::RenderingDrawCount = 0;
size_t PerformanceAnalyzer::RenderingCollisionCheckCount = 0;
size_t PerformanceAnalyzer::DepthTestDrawCount = 0;
size_t PerformanceAnalyzer::DepthTestCollisionCheckCount = 0;
float PerformanceAnalyzer::DeltaTime = 0.f;

void PerformanceAnalyzer::ResetPerformacneVariable()
{
	RenderingDrawCount = 0;
	RenderingCollisionCheckCount = 0;
	DepthTestDrawCount = 0;
	DepthTestCollisionCheckCount = 0;
	DeltaTime = 0.f;
}
