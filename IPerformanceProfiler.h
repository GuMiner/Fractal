#pragma once
#include <glm\vec4.hpp>

// Defines how frame performance profilers are implemented to verify data transfer & rendering remains within limits
class IPerformanceProfiler
{
public:
    virtual void ResetProfileFrame() = 0;

    // Returns true if a limit was hit and (if present) adds the vertices and texels towards the limits.
    virtual bool HasFrameRenderHitLimit(int vertices, int texels) = 0;
    virtual bool HasFrameRenderHitLimit() = 0;
    virtual bool HasFrameTransferHitLimit(int vertices, int texels) = 0;
    virtual bool HasFrameTransferHitLimit() = 0;
    
    // Gets the % of limits used, in the order listed above.
    virtual glm::vec4 GetPerformancePercentages() = 0;
};

