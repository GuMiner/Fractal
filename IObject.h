#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <string>

// LOD => Geometry to generate
// GEO => Instances for each geometry level
// When tied together, we have a system that can find the LOD scale(s) necessary and generate geometry, the instances associated with each geometry level.

// [LOD]
// LOD is the object level detail, transformed by a LodSummarizer so that multiple player positions correspond to the same LOD.
// Right now, we skip the 'what you are looking at' part and just generate everything around the user, because we can at least ensure smooth camera rotation, even at the cost of extra generation / rendering.
// When finding objects to generate, we take a look at what we already have by a map of LOD -> Geometry
// In order that we don't have to regenerate geometry for each position -- and also don't generate everything all the time -- we have the following activation functions and map functions:

// --Comparators -- Coalleses LOD such that we only generate Geometry necessary
// ExponentialDistanceComparator -- Generates 0 at infinity, then 1, 2, 3, etc, based on closeness. Doesn't care what direction you're looking in. Useful for non-infinite objects.
// LocationComparator -- Only position is used in LOD comparisons. Useful for space-filling 3d functions. Can roughen up the location such that the LOD grid is larger than default.
// ExactComparator -- Every LOD is unique. Useful for space-filling 3d functions.

// Infinite objects are going to generate more than is necessary. Picture an infinite grid -- we probably want to generate a series of grid lines out to the horizon.
// Given a loose LocationComparator, the infinite grid will generate a series of lines around that location. When the user moves, we only want to regenerate *some* of the lines.
// This is represented with GeometryPart objects. A GeometryPart is the minimal subset of geometry that can be generated at the specified LOD that may be reused across LODs. 
// Each geometry part is represented with an ID triplet that the object gets to define. 
// For the infinite grid, we'd get our LOD, find all GeometryParts we are missing, find the associated Geometry that is missing, queue those for generation, and render those once complete.

// Non-infinite objects are easier. You simply generate the entire geometry (stord in a GeometryPart) once it's in view. 
// -> The ID triplet is the object instance. 


// -- Activators -- Enables and disables entire objects
// -- Of note, everything is enabled / disabled by our world / scene / level generation ... whenever that is implemented.
// DistanceActivator -- Activates when close enough. Useful for non-infinite objects.

// ... todo ...
// This supports the following scenarios:
//  P = player, x = object.
//
//  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
//  P1. . . . . . . . . . . . . . . . . . . . . . . . . . .P2. . . . . . . . . . . .P3.P4. . . x
//  At P3 and P4, we want the geometry generated to be the same. At P2 and P1, we want different geometry. 


// [Core Thread]
// Generate geometry at LOD
// Render geometry at LOD

// Generate
// - Find LOD. LOD == direction + position, broken into a grid structure (cardinal directions + parts, gridlines).
// - Find objects in world, find if LOD is applicable
// - For infinite objects, find if LOD is applicable
// - For each object Create / get Geometry object, generate / mark as alive instances that already exist. DETECT total amount of vertices / textures existing / to copy.
// - Spawn threads to generate LOD of all missing geometries

// Render
// - Copy over geometry not in GPU (includes associated textures)
// - For the LOD -> For each geometry -> If not in GPU, copy as much as RATE LIMITED allows.
// - For the LOD -> For each object -> For each geometry -> Render instances of geometry. RATE LIMIT by stopping rendering if limit hit.

// [Side Threads]
// - Generate Geometry at LOD (LOD == distance + angle).