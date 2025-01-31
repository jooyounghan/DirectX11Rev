#include "PerformanceAnalyzerWindow.h"
#include <format>

using namespace std;
using namespace ImGui;

PerformanceAnalyzerWindow::PerformanceAnalyzerWindow(const string& windowID)
	: AWindow(windowID, true, nullptr)
{
}

void PerformanceAnalyzerWindow::PrepareWindow()
{
}

void PerformanceAnalyzerWindow::RenderWindowImpl()
{
    DrawFPSLinePlotting();
    DrawPerformanceIndicators();
}

void PerformanceAnalyzerWindow::DrawFPSLinePlotting()
{
    static size_t tickCountForDelay = 0;
    static int valuesOffset = 0;
    const static size_t tickPerFrame = 10;
    static float delayDeltaTime = 0.f;
    constexpr size_t fpsDataCount = 100;
    static vector<float> fpsData(fpsDataCount, 240.f);
    
    if (tickCountForDelay % tickPerFrame == 0)
    {
        delayDeltaTime = PerformanceAnalyzer::DeltaTime;
        const float currentFPS = 1.f / delayDeltaTime;
        fpsData[valuesOffset] = currentFPS;
        valuesOffset = (valuesOffset + 1) % fpsDataCount;
    }

    float average = 0.0f;
    for (size_t idx = 0; idx < fpsDataCount; idx++)
    {
        average += fpsData[idx];
    }
    average /= (float)fpsDataCount;

    PushID("FPSPlot");
    PlotLines("", fpsData.data(), fpsDataCount, valuesOffset, format("Average FPS : {}", average).c_str(), 0.0f, average * 2.f);
    PopID();

    tickCountForDelay++;
}

void PerformanceAnalyzerWindow::DrawPerformanceIndicators()
{
    Text("Total Drawn Indices Count %d", PerformanceAnalyzer::DrawCount);
    Text("Total Collision Check Count %d", PerformanceAnalyzer::CollisionCheckCount);
}