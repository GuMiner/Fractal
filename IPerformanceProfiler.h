#pragma once

// Defines how frame performance profilers are implemented to verify data transfer & rendering remains within limits
class IPerformanceProfiler
{
public:
    virtual void ResetProfileFrame() = 0;
    virtual bool HasFrameRenderHitLimit(int vertices, int texels) = 0;
    virtual bool HasFrameRenderHitLimit() = 0;
    virtual bool HasFrameTransferHitLimit(int vertices, int texels) = 0;
    virtual bool HasFrameTransferHitLimit() = 0;
};

