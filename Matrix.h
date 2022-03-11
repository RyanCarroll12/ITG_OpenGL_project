/**
 * File: Matrix.h
 * 
 * Author: Ryan Carroll
 * CSC-350
 * Header file for Matrix.cpp
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <math.h>
#include "Vector.h"

#define PI 3.14159265359

/**
 * Class: Matrix
 * 
 * This class will hold a 4 * 4 matrix and is used
 * to perform operations on the matrix.
 */
class Matrix
{
    private:
        static const int HEIGHT = 4;
        static const int WIDTH = 4;
        float data[HEIGHT][WIDTH];
    public:
        Matrix();
        Matrix(float mat[HEIGHT][WIDTH]);
        Matrix operator *(const Matrix&) const;
        mVec3 operator *(const mVec3&) const;
        float *operator [](int i);
        const float *operator [](int i) const;
        float getHeight() const;
        float getWidth() const;
        void print();
        void translate(mVec3 translation);
        void scale(float factor);
        void scale(mVec3 factorDimensions);
        void rotate(mVec3 rotation);
        void projectPerspective(float fov, float aspect, float nearPlane, float farPlane);
};

#endif