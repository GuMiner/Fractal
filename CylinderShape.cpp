#define _USE_MATH_DEFINES
#include <cmath>
#include <glm\gtx\transform.hpp>
#include "PlanePrimitive.h"
#include "CylinderShape.h"

CylinderShape::CylinderShape(int angularSteps)
    : angularSteps(angularSteps)
{
}

CoreGeometry CylinderShape::Generate()
{
    CoreGeometry coreGeometry;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    // Generate the sides of the cylinder
    float sliceSize = 2.0f * (float)M_PI / angularSteps;

    // Our cylinder is really a capped polygon. With low angular resolution, we end up with other interesting shapes
    float innerPolygonHalfAngle = ((float)M_PI) / (angularSteps);
    float planeSideHalfDistance = std::sin(innerPolygonHalfAngle);

    for (int i = 0; i < angularSteps; i++)
    {
        // The angle the plane is to be rotated to.
        float angle = sliceSize * (float)i;

        // Each plane needs to be angled correctly to meet both the lower and upper radius
        PlanePrimitive::AddPlane(vertices, normals,
            glm::rotate(angle, glm::vec3(0, 1, 0)) * 
            glm::scale(glm::vec3(planeSideHalfDistance, 1.0f, 1.0f)), glm::vec3(0, 0, 1.0f - std::cos(innerPolygonHalfAngle))); // Rotate to the correct angle.
    }

    // Generate the tops and bottoms of the cylinder.
    glm::mat4 planeFanTranslation = glm::translate(glm::vec3(0, 1.0f, 0));
    PlanePrimitive::AddPlaneFan(vertices, normals, angularSteps, planeFanTranslation);
    
    planeFanTranslation = glm::translate(glm::vec3(0, -1.0f, 0)) * glm::rotate(sliceSize / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::rotate((float)M_PI, glm::vec3(1.0f, 0.0f, 0.0f));
    PlanePrimitive::AddPlaneFan(vertices, normals, angularSteps, planeFanTranslation);

    coreGeometry.SetGeometryData(vertices, normals);
    return coreGeometry;
}
