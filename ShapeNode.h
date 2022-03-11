/**
 * File: ShapeNode.h
 * 
 * Author: Ryan Carroll
 * CSC-350
 * Header file for ShapeNode.cpp
 */

#ifndef SHAPENODE_H
#define SHAPENODE_H

#include <vector>

#include <GL/glew.h>
#include "Vector.h"
#include "Matrix.h"
#include "ShapeData.h"

class ShapeNode {
    private:
        Matrix vertexMatrix;
        Matrix normalMatrix;
        Matrix totalVertexMatrix;
        Matrix totalNormalMatrix;
        Matrix shapeVertexMatrix;
        Matrix shapeNormalMatrix;
        Matrix perspectiveMatrix;
        struct ShapeData shapeData;
        
        void drawShape(GLuint programID);
        void transverseShapeTree(GLuint programID);

    public:

        std::vector<ShapeNode *> children;

        ShapeNode();

        void add_child(ShapeNode *child);

        void setVertexMatrix(Matrix vMatrix);
        Matrix getVertexMatrix();
        void setNormalMatrix(Matrix nMatrix);
        Matrix getNormalMatrix();
        void setTotalVertexMatrix(Matrix vMatrix);
        Matrix getTotalVertexMatrix();
        void setTotalNormalMatrix(Matrix nMatrix);
        Matrix getTotalNormalMatrix();
        void setShapeVertexMatrix(Matrix vMatrix);
        Matrix getShapeVertexMatrix();
        void setShapeNormalMatrix(Matrix nMatrix);
        Matrix getShapeNormalMatrix();
        void setPerspectiveMatrix(Matrix pMatrix);
        Matrix getPerspectiveMatrix();

        void setShapeData(struct ShapeData);
        unsigned getVerticeCount();
        unsigned getStartLocation();

        void translateNode(mVec3 translation);
        void scaleNode(float factor);
        void scaleNode(mVec3 factorDimensions);
        void rotateNode(mVec3 rotation);

        void translateShape(mVec3 translation);
        void scaleShape(float factor);
        void scaleShape(mVec3 factorDimensions);
        void rotateShape(mVec3 rotation);

        void applyTranslation(mVec3 translation);
        void applyScaling(float factor);
        void applyScaling(mVec3 factorDimensions);
        void applyRotation(mVec3 rotation);

        void clearNodeTransform();
        void clearShapeTransform();
        void clearAllTranforms();

        void drawShapeTree(GLuint programID);
};

#endif