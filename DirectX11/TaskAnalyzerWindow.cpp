#include "TaskAnalyzerWindow.h"

TaskAnalyzerWindow::TaskAnalyzerWindow()
    : IWindow()
{
}

TaskAnalyzerWindow::~TaskAnalyzerWindow()
{
}

void TaskAnalyzerWindow::RenderWindow()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Begin("Task Analyzer");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
}
