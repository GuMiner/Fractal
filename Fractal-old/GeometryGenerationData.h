#pragma once

// Defines the geometry ID, LOD and any custom generation data necessary to generate the object geometry.
class GeometryGenerationData
{
public:
    // Custom data to use when generating geometry
    void* generationData;
    
    // The ID of the geometry being generated.
    long long geometryId;
    
    // The quality of the geometry to generate. 
    long long lod;
};

