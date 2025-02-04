#pragma once
class PerformanceAnalyzer
{
public:
	static size_t RenderingDrawCount;
	static size_t DepthTestDrawCount;
	static size_t RenderingCollisionCheckCount;
	static size_t DepthTestCollisionCheckCount;
	static float DeltaTime;

public:
	static void ResetPerformacneVariable();

};

