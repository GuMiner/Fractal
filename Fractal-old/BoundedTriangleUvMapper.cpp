#define _USE_MATH_DEFINES
#include <cmath>
#include <glm\gtx\norm.hpp>
#include "BoundedTriangleUvMapper.h"

BoundedTriangleUvMapper::BoundedTriangleUvMapper()
{
}

std::vector<glm::vec2> BoundedTriangleUvMapper::MapToTexturePlane(CoreGeometry coreGeometry, float* maxScaleFactorPtr)
{
    const std::vector<glm::vec3> vertices = coreGeometry.GetVertices();
    const std::vector<glm::vec3> normals = coreGeometry.GetNormals();
    size_t triangleCount = vertices.size() / 3;

    std::vector<glm::vec2> uvs;
    uvs.resize(vertices.size());

    std::vector<glm::vec2> boundingBoxes;
    boundingBoxes.resize(triangleCount);
    
    float xSum = 0.0f;
    float ySum = 0.0f;
    for (size_t i = 0; i < triangleCount; i++)
    {
        size_t idxOne = i * 3;
        size_t idxTwo = i * 3 + 1;
        size_t idxThree = i * 3 + 2;

        // Distance x is the triangle max size when the one-two line is horizontal
        // Note that we don't care about the winding, as we're ok with inverted triangles for texture mapping
        GLfloat distOne = glm::distance(vertices[idxTwo], vertices[idxThree]);
        GLfloat distTwo = glm::distance(vertices[idxOne], vertices[idxThree]);
        GLfloat distThree = glm::distance(vertices[idxOne], vertices[idxTwo]);
        GLfloat oneAngle = acos((distTwo*distTwo + distThree*distThree - distOne*distOne) / (2 * distTwo*distThree));

        // Both one and two y offsets at zero by definition
        uvs[idxOne].y = 0;
        uvs[idxTwo].y = 0;

        // If the triangle is tilted to the left, apply an x-offset
        if (oneAngle > M_PI_2)
        {
            // Same as cos(side three)*distThree, projection on the x axis.
            GLfloat cosThreeAngle = (distThree*distThree + distOne*distOne - distTwo*distTwo) / (2 * distOne*distThree);

            // X offset, bottom straight line
            uvs[idxOne].x = cosThreeAngle*distOne - distThree;
            uvs[idxTwo].x = cosThreeAngle*distOne; // Second point

                                                              // By definition at zero from x-offset
            uvs[idxThree].x = 0;
            uvs[idxThree].y = sqrt(distOne*distOne - cosThreeAngle*cosThreeAngle*distOne*distOne); // y-projection

                                                                                                            // X and Y bounding boxes
            boundingBoxes[i].x = cosThreeAngle*distOne;
            boundingBoxes[i].y = uvs[(i * 3 + 2)].y; // Maximum height from y-projection.
        }
        else // Triangle tilted to the right
        {
            boundingBoxes[i].x = glm::max(distThree, cos(oneAngle)*distTwo);
            boundingBoxes[i].y = std::sqrt(distTwo*distTwo - cos(oneAngle)*distTwo*cos(oneAngle)*distTwo);

            // x positions, rather straightforward
            uvs[idxOne].x = 0;
            uvs[idxTwo].x = distThree;
            uvs[idxThree].x = cos(oneAngle)*distTwo;

            uvs[idxThree].y = boundingBoxes[i].y;
        }

        xSum += boundingBoxes[i].x;
        ySum += boundingBoxes[i].y;
    }

    // First pass done. Now try to place and squarify the triangles by skewing the distribution.
    // std::cout << "x sum: " << xSum << ", y sum: " << ySum << std::endl;
    int trianglesPerYHeight = (int)(std::sqrt(triangleCount) * xSum / ySum);
    // std::cout << "Num triangles: " << trianglesPerYHeight << std::endl;

    GLfloat xSideMax = 0;
    GLfloat yMax = 0;

    GLfloat xCurrent = 0, yCurrent = 0;
    int tCount = 0;
    for (int i = 0; i < triangleCount; i++)
    {
        size_t idxOne = i * 3;
        size_t idxTwo = i * 3 + 1;
        size_t idxThree = i * 3 + 2;

        // Add x position offsets
        uvs[idxOne].x += xCurrent;
        uvs[idxTwo].x += xCurrent;
        uvs[idxThree].x += xCurrent;

        xSideMax = glm::max(xSideMax, boundingBoxes[i].x);

        // Add y position offsets
        uvs[idxOne].y += yCurrent;
        uvs[idxTwo].y += yCurrent;
        uvs[idxThree].y += yCurrent;
        yCurrent += boundingBoxes[i].y;

        // Update maximum y position.
        yMax = glm::max(yMax, yCurrent);

        // Jump to the next series of triangles
        ++tCount;
        if (tCount > trianglesPerYHeight)
        {
            tCount = 0;

            xCurrent += xSideMax;
            xSideMax = 0;

            yCurrent = 0;
        }
    }

    // Update xCurrent to be xMax.
    xCurrent += xSideMax;

    //std::cout << "Actual maxes: (" << xCurrent << ", " << yMax << ")" << std::endl;

    // Finally, normalize placed triangles
    for (int i = 0; i < uvs.size(); i++)
    {
        // TODO TODO This sjpi;d be imofpr,/
        uvs[i].x /= xCurrent;
        uvs[i].y /= yMax;
    }

    // TODO this should be the scale down factor to reach 0-1.
    *maxScaleFactorPtr = 1.0f;

    return uvs;
}
