#include "TaskAnalyzerWindow.h"
#include "GlobalVariable.h"

#include <string>

using namespace std;
using namespace ImGui;

unordered_map<RenderingAlgorithm, string> TaskAnalyzerWindow::RenderingAlgorithmToDescriptions
=
{
    {RenderingAlgorithm::ForwardRendering, "Forward Rendering"},
    {RenderingAlgorithm::DefferedRendergin, "Deffered Rendering"}
};

TaskAnalyzerWindow::TaskAnalyzerWindow()
    : AWindow()
{
}

TaskAnalyzerWindow::~TaskAnalyzerWindow()
{
}

void TaskAnalyzerWindow::RenderWindow()
{
    Begin("Task Analyzer");

    DrawFPSLinePlotting();
    DrawTotalIndexedCount();
    DrawRenderingAlgorithmSelection();

    End();
}

void TaskAnalyzerWindow::DrawFPSLinePlotting()
{
    ImGuiIO& io = GetIO();
    Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    static float FrameRates[1000] = {};
    static int values_offset = 0;

    FrameRates[values_offset] = io.Framerate;
    values_offset = (values_offset + 1) % IM_ARRAYSIZE(FrameRates);

    float average = 0.0f;
    for (int n = 0; n < IM_ARRAYSIZE(FrameRates); n++)
        average += FrameRates[n];
    average /= (float)IM_ARRAYSIZE(FrameRates);

    char OverlayText[32];
    sprintf_s(OverlayText, "Average FPS : %f", average);
    PushID("FPSPlot");
    PlotLines("", FrameRates, IM_ARRAYSIZE(FrameRates), values_offset, OverlayText, 0.0f, average * 2.f);
    PopID();
}

void TaskAnalyzerWindow::DrawTotalIndexedCount()
{
    Text("Total Drawn Indices Count %d", Performance::GTotalIndexCount);
    Performance::GTotalIndexCount = 0;

}

void TaskAnalyzerWindow::DrawRenderingAlgorithmSelection()
{
    PushID("SelectingRenderingAlgorithm");
    if (ImGui::BeginCombo("", RenderingAlgorithmToDescriptions[SelectedRenderingAlgorithm].c_str()))
    {
        for (auto& RenderingAlgorithmToDescription : RenderingAlgorithmToDescriptions)
        {
            if (ImGui::Selectable(RenderingAlgorithmToDescription.second.c_str()))
            {
                SelectedRenderingAlgorithm = RenderingAlgorithmToDescription.first;
            }
        }
        ImGui::EndCombo();
    }
    PopID();
}
