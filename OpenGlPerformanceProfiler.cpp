#include "OpenGlPerformanceProfiler.h"

OpenGlPerformanceProfiler::OpenGlPerformanceProfiler()
{
}

void OpenGlPerformanceProfiler::Analyze()
{
    // TODO actually implement and judge the usefulness of these.
    maxVerticesPerFrame = 100000;
    maxTexelsPerFrame = 10000000;

    maxTexelsTransferredPerFrame = 100000;
    maxVerticesTransferredPerFrame = 10000000;
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
    return currentVerticesTransferredInFrame > maxVerticesPerFrame ||
        currentTexelsTransferredInFrame > maxTexelsPerFrame;
}