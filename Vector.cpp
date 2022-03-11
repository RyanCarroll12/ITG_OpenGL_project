/**
 * File: Vector.cpp
 * 
 * Author: Ryan Carroll
 * CSC-350
 * This code is used for vector operations.
 */
#include "Vector.h"


/**
 * Function: mVec3_calc_distance
 * 
 * Calculates the distance between two points: p1, p2.
 * 
 * p1: First point
 * p2: Second point
 * return: Distance between first and second point.
 */
float mVec3_calc_distance(mVec3 p1, mVec3 p2)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;

    float distance = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));

    return distance;
}

/**
 * Function: mVec3_calc_triangle_normal
 * 
 * Calculates the normal of a triangle with the points: p1, p2, p3.
 * 
 * The normal's direction will depend the points order.
 * 
 * Points arranged in clockwise order from the viewer's perspective 
 * will have a normal pointing towards the viewer from the viewer's perspective.
 * 
 * Points arranged in counter-clockwise order from the viewer's perspective
 * will have a normal pointing away from the viewer from the viewer's perspective.
 * 
 * p1: First point.
 * p2: Second point.
 * p3: Third point.
 * return: The normal.
 */
mVec3 mVec3_calc_triangle_normal(mVec3 p1, mVec3 p2, mVec3 p3)
{
    mVec3 U = {p2.x - p1.x, p2.y - p1.y, p2.z - p1.z};
    mVec3 V = {p3.x - p1.x, p3.y - p1.y, p3.z - p1.z};

    mVec3 normal = {};

    normal.x = (U.y * V.z) - (U.z * V.y);
    normal.y = (U.z * V.x) - (U.x * V.z);
    normal.z = (U.x * V.y) - (U.y * V.x);

    return normal;
}

/**
 * Function: mVec3_normalize
 * 
 * Returns a point in the vector direction of p2 and distance of length away from p1.
 * 
 * p1: New point will be length away from this point.
 * p2: New point will have the vector direction from p1 to p2.
 * length: The distance between p1 and the new point.
 * 
 * return: Normalized point.
 */
mVec3 mVec3_normalize(mVec3 p1, mVec3 p2, float length)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float dz = p2.z - p1.z;

    float distance = mVec3_calc_distance(p1, p2);

    dx = dx * length / distance;
    dy = dy * length / distance;
    dz = dz * length / distance;

    mVec3 p3;
    p3.x = p1.x + dx;
    p3.y = p1.y + dy;
    p3.z = p1.z + dz;

    return p3;
}

/**
 * Function: mVec3_unit_normalize
 * 
 * Normalizes and mVec3 to a magnitude of 1
 * 
 * p: mVec3 to normalize
 * 
 * return: p normalized
 */
mVec3 mVec3_unit_normalize(mVec3 p)
{
    float distance = mVec3_calc_distance({0,0,0}, p);

    p.x /= distance;
    p.y /= distance;
    p.z /= distance;

    return p;
}

/**
 * Function: mVec3_midpoint
 * 
 * Calculates the midpoint between p1 and p2
 * 
 * p1: Point 1
 * p2: Point 2
 * 
 * return: Midpoint
 */
mVec3 mVec3_midpoint(mVec3 p1, mVec3 p2)
{
    mVec3 pHalf;

    pHalf.x = (p2.x + p1.x) / 2;
    pHalf.y = (p2.y + p1.y) / 2;
    pHalf.z = (p2.z + p1.z) / 2;

    return pHalf;
}

/**
 * Function: mVec3_add
 * 
 * Adds two vectors p1 and p2
 * 
 * p1: First part
 * p2: Second part
 * 
 * return: mVec3 sum of p1 and p2
 */
mVec3 mVec3_add(mVec3 p1, mVec3 p2)
{
    return {p1.x + p2.x, p1.y + p2.y, p1.z + p2.z};
}