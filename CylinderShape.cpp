#define _USE_MATH_DEFINES
#include <cmath>
#include <glm\gtx\transform.hpp>
#include "PlanePrimitive.h"
#include "CylinderShape.h"

CylinderShape::CylinderShape(int angularSteps)
    : angularSteps(angularSteps), lowerRadius(1.0f), upperRadius(1.0f)
{
}

CylinderShape::CylinderShape(int angularSteps, float lowerRadius, float upperRadius)
    : angularSteps(angularSteps), lowerRadius(lowerRadius), upperRadius(upperRadius)
{
}

CoreGeometry CylinderShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // Generate the sides of the cylinder
    float midRadius = (upperRadius + lowerRadius) / 2.0f;
    float sliceSize = 2.0f * (float)M_PI / angularSteps;

    float cylinderTiltAngle = std::atan(std::abs(upperRadius - lowerRadius) / 2.0f);
    // Our cylinder is really a capped polygon. With low angular resolution, we end up with other interesting shapes
    float innerPolygonHalfAngle = ((float)(angularSteps - 2) * (float)M_PI) / (2.0f * angularSteps);
    float planeSideDistance = midRadius / std::tan(innerPolygonHalfAngle);

    for (int i = 0; i < angularSteps; i++)
    {
        // The angle the plane is to be rotated to.
        float angle = sliceSize * (float)i;

        // Each plane needs to be angled correctly to meet both the lower and upper radius, along with proper skewing of both the top and the bottom
        PlanePrimitive::AddPlane(vertices, normals,
            glm::rotate(cylinderTiltAngle, glm::vec3(1, 0, 0)) *  // TODO, we need to move the plane to 0,0,0 to perform this transformation ... and then back.
            glm::rotate(angle, glm::vec3(0, 1, 0)) * 
            glm::scale(glm::vec3(planeSideDistance, 1.0f, 1.0f)), glm::vec3(0, 0, 1.0f - midRadius)); // Rotate to the correct angle.
    }

    // Generate the tops and bottoms of the cylinder.
    glm::mat4 planeFanTranslation = glm::translate(glm::vec3(0, 1.0f, 0)) * glm::scale(glm::vec3(midRadius, 1.0f, midRadius));
    PlanePrimitive::AddPlaneFan(vertices, normals, angularSteps, planeFanTranslation); // TODO need to transform to top and bottom.
    
    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
