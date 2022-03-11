/**
 * File: Vector.h
 * 
 * Author: Ryan Carroll
 * CSC-350
 * Just holds some structs for now.
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

/**
 * Union: mVec3
 * 
 * Used for position and color float vectors of size 3
 */
union mVec3
{
    struct { float x, y, z; };
    struct { float r, g, b; };
};

float mVec3_calc_distance(mVec3 p1, mVec3 p2);
mVec3 mVec3_calc_triangle_normal(mVec3 p1, mVec3 p2, mVec3 p3);
mVec3 mVec3_normalize(mVec3 p1, mVec3 p2, float length);
mVec3 mVec3_unit_normalize(mVec3 p);
mVec3 mVec3_midpoint(mVec3 p1, mVec3 p2);
mVec3 mVec3_add(mVec3 p1, mVec3 p2);

/**
 * Struct: Vertex
 * 
 * Used for holding position, color, and normal vertex data
 */
struct Vertex
{
    mVec3 position;
    mVec3 color;
    mVec3 normal;
    float diffuseStrength;
    float specularStrength;
};

#endif // VECTOR_H