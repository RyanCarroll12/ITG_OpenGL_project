/**
 * File: ShapeGenerator.h
 * 
 * Author: Ryan Carroll
 * CSC-350
 * Header file for ShapeGenerator.cpp
 */

#ifndef SHAPEGENERATOR_H
#define SHAPEGENERATOR_H

#include <vector>

#include "Vector.h"
#include "Matrix.h"
#include "ShapeData.h"
#include "ShapeNode.h"

#define VERTICES_PER_TRIANGLE 3
#define VERTICES_PER_SQUARE VERTICES_PER_TRIANGLE * 2
#define VERTICES_PER_CUBE VERTICES_PER_SQUARE * 6

/**
 * Class: ShapeGenerator
 * 
 * This class will be used to generate shapes.
 */
class ShapeGenerator
{
    private:
        std::vector<struct Vertex> &vertices;

        void subdivideCylinderPoints(float circle_size, int &v_offset, mVec3 p1,
        mVec3 p2, const mVec3 center, const mVec3 up_vector, unsigned subdivision_level);

        void subdivideTriangles(float sphere_size, mVec3 sphere_center,
        int &v_offset, mVec3 p1, mVec3 p2, mVec3 p3, unsigned subdivision_level);

    public:

        ShapeGenerator(std::vector<struct Vertex> &verts);

        ShapeData add_cylinder(mVec3 color, float diffuseStrength, float specularStrength, const unsigned subdivision_level);

        ShapeData add_sphere(mVec3 color, float diffuseStrength, float specularStrength, const unsigned subdivision_level);

        ShapeData add_cube(mVec3 color, float diffuseStrength, float specularStrength);

        void load_3d_color_grid(mVec3 colors[], const int COLORS_HEIGHT, const int COLORS_WIDTH, GLfloat cube_size);

        void add_tree(ShapeNode *ground, float tree_size, mVec3 tree_location);
};

#endif /* SHAPEGENERATOR_H */