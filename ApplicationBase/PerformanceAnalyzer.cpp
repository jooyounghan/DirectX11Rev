#include "PerformanceAnalyzer.h"

size_t PerformanceAnalyzer::DrawCount = 0;
size_t PerformanceAnalyzer::DrawForDepthTestCount = 0;
size_t PerformanceAnalyzer::CollisionCheckCount = 0;
float PerformanceAnalyzer::DeltaTime = 0.f;

void PerformanceAnalyzer::ResetPerformacneVariable()
{
	DrawCount = 0;
	DrawForDepthTestCount = 0;
	CollisionCheckCount = 0;
	DeltaTime = 0.f;
}
