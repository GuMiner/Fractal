#include "OpenGlPerformanceProfiler.h"

OpenGlPerformanceProfiler::OpenGlPerformanceProfiler()
{
}

void OpenGlPerformanceProfiler::Analyze()
{
    // TODO actually implement and judge the usefulness of these.
    maxVerticesPerFrame = 10000000;
    maxTexelsPerFrame = 10000000;

    maxVerticesTransferredPerFrame = 10000000;
    maxTexelsTransferredPerFrame = 10000000;
}

void OpenGlPerformanceProfiler::ResetProfileFrame()
{
    currentVerticesInFrame = 0;
    currentTexelsInFrame = 0;

    currentVerticesTransferredInFrame = 0;
    currentTexelsTransferredInFrame = 0;
}

bool OpenGlPerformanceProfiler::HasFrameRenderHitLimit(int vertices, int texels)
{
    currentVerticesInFrame += vertices;
    currentTexelsInFrame += texels;
    return HasFrameRenderHitLimit();
}

bool OpenGlPerformanceProfiler::HasFrameRenderHitLimit()
{
    return currentVerticesInFrame > maxVerticesPerFrame ||
        currentTexelsInFrame > maxTexelsPerFrame;
}

bool OpenGlPerformanceProfiler::HasFrameTransferHitLimit(int vertices, int texels)
{
    currentVerticesTransferredInFrame += vertices;
    currentTexelsTransferredInFrame += texels;
    return HasFrameTransferHitLimit();
}

bool OpenGlPerformanceProfiler::HasFrameTransferHitLimit()
{
    return currentVerticesTransferredInFrame > maxVerticesTransferredPerFrame ||
        currentTexelsTransferredInFrame > maxTexelsTransferredPerFrame;
}

glm::vec4 OpenGlPerformanceProfiler::GetPerformancePercentages()
{
    return glm::vec4(
        (float)currentVerticesInFrame / (float)maxVerticesPerFrame,
        (float)currentTexelsInFrame / (float)maxTexelsPerFrame,
        (float)currentVerticesTransferredInFrame / (float)maxVerticesTransferredPerFrame,
        (float)currentTexelsTransferredInFrame / (float)maxTexelsTransferredPerFrame);
}