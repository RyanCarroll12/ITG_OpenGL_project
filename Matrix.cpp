/**
 * File: Matrix.cpp
 * 
 * Author: Ryan Carroll
 * CSC-350
 * This code is used for matrix operations and transformations.
 */

#include "Matrix.h"

/**
 * Matrix Constructor
 * 
 * Creates identity matrix.
 */
Matrix::Matrix()
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            if (x == y)
                data[y][x] = 1;
            else
                data[y][x] = 0;
}

/**
 * Matrix Constructor
 * 
 * Loads in 2d float array of HEIGHT * WIDTH.
 * 
 * mat: float array to load in.
 */
Matrix::Matrix(float mat[HEIGHT][WIDTH])
{
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            data[y][x] = mat[y][x];
}

/**
 * Operator* overload
 * 
 * Used for matrix * matrix mulitiplication.
 * 
 * obj: Matrix to multiply.
 */
Matrix Matrix::operator*(const Matrix &obj) const
{
    // Matrix width needs to be the same as other matrix height
    if (HEIGHT != obj.getWidth() || WIDTH != obj.getHeight())
    {
        return NULL;
    }

    Matrix tmp;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            float sum = 0.0f;
            for (int i = 0; i < WIDTH; i++)
            {
                sum = sum + (data[y][i] * obj[i][x]);
            }
            tmp[y][x] = sum;
        }
    }

    return tmp;
}

/**
 * Operator* overload
 * 
 * Used for Vector * Matrix multiplication.
 * 
 * obj: vector to multiple against.
 */
mVec3 Matrix::operator*(const mVec3 &obj) const
{
    float sum[HEIGHT] = {};

    for (int y = 0; y < HEIGHT; y++)
    {
        sum[y] = sum[y] + obj.x * data[y][0];
        sum[y] = sum[y] + obj.y * data[y][1];
        sum[y] = sum[y] + obj.z * data[y][2];
        sum[y] = sum[y] + 1 * data[y][3];
    }

    return {sum[0], sum[1], sum[2]};
}

/**
 * Operator[] overload
 * 
 * Used to get array value at location i.
 * 
 * i: location of desired array value.
 */
float *Matrix::operator[](const int i)
{
    return data[i];
}

/**
 * Operator[] overload
 * 
 * Used to get array value at location i.
 * 
 * i: location of desired array value.
 */
const float *Matrix::operator[](const int i) const
{
    return data[i];
}

/**
 * Function: getHeight
 * 
 * Gets height of the array.
 * 
 * Return: array height.
 */
float Matrix::getHeight() const
{
    return HEIGHT;
}

/**
 * Function: getWidth
 * 
 * Gets width of the array.
 * 
 * Return: array width.
 */
float Matrix::getWidth() const
{
    return WIDTH;
}

/**
 * Function: print
 * 
 * Prints out matrix currently stored in object data.
 */
void Matrix::print()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            std::cout << data[y][x] << " ";
        }
        std::cout << "\n";
    }
}

/**
 * Function: translate
 * 
 * This function will translate the matrix in data
 * according to the vector translation.
 * 
 * translation: Vector which holds translation to be performed.
 */
void Matrix::translate(mVec3 translation)
{
    data[0][3] += translation.x;
    data[1][3] += translation.y;
    data[2][3] += translation.z;
}

/**
 * Function: scale
 * 
 * This function will load scale transformation into the matrix data.
 * 
 * factor: how much the matrix will scale by.
 */
void Matrix::scale(float factor)
{
    for (int i = 0; i < 3; i++)
    {
        data[i][i] *= factor;
    }
}

/**
 * Function: scale
 * 
 * This function scales can scale by each dimension.
 * 
 * factorDimensions: mVec3 holding how much to scale each dimension needs
 */
void Matrix::scale(mVec3 factorDimensions)
{
    data[0][0] *= factorDimensions.x;
    data[1][1] *= factorDimensions.y;
    data[2][2] *= factorDimensions.z;
}

/**
 * Function: rotate
 * 
 * This function will load a rotation transformation into
 * the matrix data.
 * 
 * rotation: Vector holding degrees to rotate each axis.
 */
void Matrix::rotate(mVec3 rotation)
{
    if (rotation.x)
    {
        float xRadians = rotation.x * PI / 180;
        float xMat[HEIGHT][WIDTH] = {
          {1.0,      0.0      ,      0.0       , 0.0}
        , {0.0, cos(xRadians), sin(xRadians), 0.0}
        , {0.0, -sin(xRadians), cos(xRadians) , 0.0}
        , {0.0,      0.0      ,      0.0       , 1.0}
        };
        Matrix xRotMat(xMat);
        *this = *this * xRotMat;
    }
    if (rotation.y)
    {
        float yRadians = rotation.y * PI / 180;
        float yMat[HEIGHT][WIDTH] = {
          {cos(yRadians) , 0.0, -sin(yRadians), 0.0}
        , {      0.0      , 1.0,      0.0      , 0.0}
        , {sin(yRadians), 0.0, cos(yRadians), 0.0}
        , {      0.0      , 0.0,      0.0      , 1.0}
        };
        Matrix yRotMat(yMat);
        *this = *this * yRotMat;
    }
    if (rotation.z)
    {
        float zRadians = rotation.z * PI / 180;
        float zMat[HEIGHT][WIDTH] = {
          {cos(zRadians), -sin(zRadians), 0.0, 0.0}
        , {sin(zRadians), cos(zRadians) , 0.0, 0.0}
        , {     0.0      ,       0.0      , 1.0, 0.0}
        , {     0.0      ,       0.0      , 0.0, 1.0}
        };
        Matrix zRotMat(zMat);
        *this = *this * zRotMat;
    }
}

/**
 * Function: projectPerspective
 * 
 * Creates a projection perspective matrix and multiplies it
 * by the matrix held it data, then stores it in data.
 * 
 * fov_degrees: Field-of-view of camera in degrees
 * aspect: Aspect ratio of the window
 * nearPlane: Closest z-value to be drawn
 * farPlane: Furthest z-value to be drawn.
 */
void Matrix::projectPerspective(float fov_degrees, float aspect, float nearPlane, float farPlane)
{
    nearPlane = nearPlane > 0 ? nearPlane : 0.1; // Assert nearPlane is greater than zero
    farPlane = farPlane <= nearPlane ? nearPlane + 1.0 : farPlane; // Assert farPlane is greater than nearPlane

    float tan_half_angle = tan((fov_degrees * PI) / 360.0f);
    float fov = 1 / tan_half_angle;

    float a = fov / aspect;
    float b = fov;
    float c = (farPlane + nearPlane) / (farPlane - nearPlane);
    float d = -(2.0 * farPlane * nearPlane) / (farPlane - nearPlane);

    float mat[HEIGHT][WIDTH] = {
          { a , 0 , 0 , 0 }
        , { 0 , b , 0 , 0 }
        , { 0 , 0 , c , d }
        , { 0 , 0 ,1.0, 0 }
    };

    Matrix pMat(mat);

    *this = pMat * *this;
}