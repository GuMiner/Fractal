#pragma once
#include "IPerformanceProfiler.h"

// Logs performance of OpenGL and ensures we don't overflow our performance limitations.
class OpenGlPerformanceProfiler : public IPerformanceProfiler
{
    long long maxVerticesPerFrame;
    long long maxTexelsPerFrame;

    long long maxVerticesTransferredPerFrame;
    long long maxTexelsTransferredPerFrame;

    long long currentVerticesInFrame;
    long long currentTexelsInFrame;

    long long currentVerticesTransferredInFrame;
    long long currentTexelsTransferredInFrame;

public:
    OpenGlPerformanceProfiler();
    void Analyze();

    virtual void ResetProfileFrame() override;
    virtual bool HasFrameRenderHitLimit(int vertices, int texels) override;
    virtual bool HasFrameTransferHitLimit(int vertices, int texels) override;
    virtual bool HasFrameRenderHitLimit() override;
    virtual bool HasFrameTransferHitLimit() override;
    virtual glm::vec4 GetPerformancePercentages() override;
};

