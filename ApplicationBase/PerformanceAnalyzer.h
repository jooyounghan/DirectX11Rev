#pragma once
class PerformanceAnalyzer
{
public:
	static size_t DrawCount;
	static size_t DrawForDepthTestCount;
	static size_t CollisionCheckCount;
	static float DeltaTime;

public:
	static void ResetPerformacneVariable();

};

