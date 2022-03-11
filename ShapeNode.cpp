/**
 * File: ShapeNode.cpp
 * 
 * Author: Ryan Carroll
 * CSC-350
 * This code hold functions for ShapeNode objects.
 */

#include "ShapeNode.h"

/**
 * Function: drawShape
 * 
 * Loads vertex matrix and normal matrix then draws from start the number of vertices the shape has.
 * 
 * programID: OpenGL programID
 */ 
void ShapeNode::drawShape(GLuint programID)
{
    if (!shapeData.verticeCount)
        return;
    
    Matrix pvMatrix = perspectiveMatrix * totalVertexMatrix * shapeVertexMatrix;
    Matrix nMatrix = totalNormalMatrix * shapeNormalMatrix;

    GLint vertexMatrixUniformLocation = glGetUniformLocation(programID, "vertexMatrix");
    glUniformMatrix4fv(vertexMatrixUniformLocation, 1, GL_TRUE, &pvMatrix[0][0]);

    GLint normalMatrixUniformLocation = glGetUniformLocation(programID, "normalMatrix");
    glUniformMatrix4fv(normalMatrixUniformLocation, 1, GL_TRUE, &nMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, shapeData.startLocation, shapeData.verticeCount);
}

/**
 * Function: transverseShapeTree
 * 
 * Transverses the shape tree drawing each shape in the tree.
 * 
 * programID: OpenGL programID
 */
void ShapeNode::transverseShapeTree(GLuint programID)
{
    totalVertexMatrix = totalVertexMatrix * vertexMatrix;
    totalNormalMatrix = totalNormalMatrix * normalMatrix;

    drawShape(programID);

    for (ShapeNode *child : children)
    {
        child->setTotalVertexMatrix(getTotalVertexMatrix());
        child->setTotalNormalMatrix(getTotalNormalMatrix());
        child->setPerspectiveMatrix(getPerspectiveMatrix());
        child->transverseShapeTree(programID);
    }

    totalVertexMatrix = Matrix();
    totalNormalMatrix = Matrix();
}

/**
 * ShapeNode Constructor
 * 
 * Creates default ShapeNode.
 */
ShapeNode::ShapeNode()
{
    vertexMatrix = Matrix();
    normalMatrix = Matrix();
    totalVertexMatrix = Matrix();
    totalNormalMatrix = Matrix();
    shapeVertexMatrix = Matrix();
    shapeNormalMatrix = Matrix();
    perspectiveMatrix = Matrix();
    shapeData = {};
}


/**
 * Function: add_child
 * 
 * Adds the ShapeNode pointer "child" as a child to this ShapeNode
 * 
 * child: Pointer to the desired child ShapeNode
 */
void ShapeNode::add_child(ShapeNode *child)
{
    children.push_back(child);
}


/**
 * Function: setVertexMatrix
 * 
 * Setter for class vertexMatrix
 * 
 * vMatrix: Matrix to set vertexMatrix to.
 */
void ShapeNode::setVertexMatrix(Matrix vMatrix)
{
    vertexMatrix = vMatrix;
}

/**
 * Function: getVertexMatrix
 * 
 * Getter for class vertexMatrix
 * 
 * returns: Class vertexMatrix
 */
Matrix ShapeNode::getVertexMatrix()
{
    return vertexMatrix;
}

/**
 * Function: setNormalMatrix
 * 
 * Setter for class normalMatrix
 * 
 * nMatrix: Matrix to set normalMatrix to.
 */
void ShapeNode::setNormalMatrix(Matrix nMatrix)
{
    normalMatrix = nMatrix;
}

/**
 * Function: getNormalMatrix
 * 
 * Getter for class normalMatrix
 * 
 * returns: Class normalMatrix
 */
Matrix ShapeNode::getNormalMatrix()
{
    return normalMatrix;
}

/**
 * Function: setTotalVertexMatrix
 * 
 * Setter for class totalVertexMatrix
 * 
 * vMatrix: Matrix to set totalVertexMatrix to.
 */ 
void ShapeNode::setTotalVertexMatrix(Matrix vMatrix)
{
    totalVertexMatrix = vMatrix;
}

/**
 * Function: getTotalVertexMatrix
 * 
 * Getter for class totalVertexMatrix
 * 
 * returns: Class totalVertexMatrix
 */
Matrix ShapeNode::getTotalVertexMatrix()
{
    return totalVertexMatrix;
}

/**
 * Function: setTotalNormalMatrix
 * 
 * Setter for class totalNormalMatrix
 * 
 * nMatrix: Matrix to set totalNormalMatrix to.
 */
void ShapeNode::setTotalNormalMatrix(Matrix nMatrix)
{
    totalNormalMatrix = nMatrix;
}

/**
 * Function: getTotalNormalMatrix
 * 
 * Getter for class totalNormalMatrix
 * 
 * return: Class totalNormalMatrix
 */
Matrix ShapeNode::getTotalNormalMatrix()
{
    return totalNormalMatrix;
}

/**
 * Function: setShapeVertexMatrix
 * 
 * Setter for class shapeVertexMatrix
 * 
 * vMatrix: Matrix to set shapeVertexMatrix to.
 */
void ShapeNode::setShapeVertexMatrix(Matrix vMatrix)
{
    shapeVertexMatrix = vMatrix;
}

/**
 * Function: getShapeVertexMatrix
 * 
 * Getter for class shapeVertexMatrix
 * 
 * return: Class shapeVertexMatrix
 */
Matrix ShapeNode::getShapeVertexMatrix()
{
    return shapeVertexMatrix;
}

/**
 * Function: setShapeNormalMatrix
 * 
 * Setter for class shapeNormalMatrix
 * 
 * nMatrix: Matrix to set shapeNormalMatrix to.
 */
void ShapeNode::setShapeNormalMatrix(Matrix nMatrix)
{
    shapeNormalMatrix = nMatrix;
}

/**
 * Function: getShapeNormalMatrix
 * 
 * Getter for class shapeNormalMatrix
 * 
 * return: Class shapeNormalMatrix
 */
Matrix ShapeNode::getShapeNormalMatrix()
{
    return shapeNormalMatrix;
}

/**
 * Function: setPerspectiveMatrix
 * 
 * Setter for class perspectiveMatrix
 * 
 * pMatrix: Matrix to set perspectiveMatrix to.
 */
void ShapeNode::setPerspectiveMatrix(Matrix pMatrix)
{
    perspectiveMatrix = pMatrix;
}

/**
 * Function: getPerspectiveMatrix
 * 
 * Getter for class perspectiveMatrix
 * 
 * return: Class perspectiveMatrix
 */
Matrix ShapeNode::getPerspectiveMatrix()
{
    return perspectiveMatrix;
}


/**
 * Function: setShapeData
 * 
 * Setter for class shapeData
 * 
 * sd: ShapeData to set class shapeData to.
 */
void ShapeNode::setShapeData(struct ShapeData sd)
{
    shapeData = sd;
}

/**
 * Function: getVerticeCount
 * 
 * Getter for class shapeData verticeCount
 * 
 * return: verticeCount
 */
unsigned ShapeNode::getVerticeCount()
{
    return shapeData.verticeCount;
}

/**
 * Function: getStartLocation
 * 
 * Getter for class shapeData startLocation
 * 
 * return: startLocation
 */
unsigned ShapeNode::getStartLocation()
{
    return shapeData.startLocation;
}


/**
 * Function: translateNode
 * 
 * Translates the node and its children by translation
 * 
 * translation: Dimensions to translate by
 */
void ShapeNode::translateNode(mVec3 translation)
{
    vertexMatrix.translate(translation);
}

/**
 * Function: scaleNode
 * 
 * Scales the node and its children by factor
 * 
 * factor: Float factor to scale it by
 */
void ShapeNode::scaleNode(float factor)
{
    vertexMatrix.scale(factor);
}

/**
 * Function: scaleNode
 * 
 * Scales the node and its children by factorDimensions
 * 
 * factorDimensions: mVec3 factor to scale each dimension by
 */
void ShapeNode::scaleNode(mVec3 factorDimensions)
{
    vertexMatrix.scale(factorDimensions);
}

/**
 * Function: rotateNode
 * 
 * Rotates the node and its children by rotation
 * 
 * rotation: mVec3 angle to rotate for each dimension
 */
void ShapeNode::rotateNode(mVec3 rotation)
{
    vertexMatrix.rotate(rotation);
    normalMatrix.rotate(rotation);
}


/**
 * Function: translateShape
 * 
 * Translates the shape independent of children by translation
 * 
 * translation: Dimensions to translate by
 */
void ShapeNode::translateShape(mVec3 translation)
{
    shapeVertexMatrix.translate(translation);
}

/**
 * Function: scaleShape
 * 
 * Scales the shape independent of its children by factor
 * 
 * factor: Float factor to scale it by
 */
void ShapeNode::scaleShape(float factor)
{
    shapeVertexMatrix.scale(factor);
}

/**
 * Function: scaleShape
 * 
 * Scales the shape independent its children by factorDimensions
 * 
 * factorDimensions: mVec3 factor to scale each dimension by
 */
void ShapeNode::scaleShape(mVec3 factorDimensions)
{
    shapeVertexMatrix.scale(factorDimensions);
}

/**
 * Function: rotateShape
 * 
 * Rotates the shape independent of its children by rotation
 * 
 * rotation: mVec3 angle to rotate for each dimension
 */
void ShapeNode::rotateShape(mVec3 rotation)
{
    shapeVertexMatrix.rotate(rotation);
    shapeNormalMatrix.rotate(rotation);
}


/**
 * Function: applyTranslation
 * 
 * Applies translation to node between draw calls
 * 
 * translation: Translation to apply
 */
void ShapeNode::applyTranslation(mVec3 translation)
{
    totalVertexMatrix.translate(translation);
}

/**
 * Function: applyScaling
 * 
 * Applies scaling to node between draw calls
 * 
 * factor: Factor to scale by
 */
void ShapeNode::applyScaling(float factor)
{
    totalVertexMatrix.scale(factor);
}

/**
 * Function: applyScaling
 * 
 * Applies scaling dimensionally to node between draw calls
 * 
 * factorDimensions: Factor to scale each dimension by
 */
void ShapeNode::applyScaling(mVec3 factorDimensions)
{
    totalVertexMatrix.scale(factorDimensions);
}

/**
 * Function: applyRotation
 * 
 * Applies rotation to node between draw calls
 * 
 * rotation: mVec3 angle to rotate for each dimension
 */
void ShapeNode::applyRotation(mVec3 rotation)
{
    totalVertexMatrix.rotate(rotation);
    totalNormalMatrix.rotate(rotation);
}


/**
 * Function: clearNodeTransform
 * 
 * Re-initializes inheiritable transforms made to the node
 */
void ShapeNode::clearNodeTransform()
{
    totalVertexMatrix = Matrix();
    totalNormalMatrix = Matrix();
    vertexMatrix = Matrix();
    normalMatrix = Matrix();
}

/**
 * Function: clearShapeTransform
 * 
 * Re-initializes transforms made to this node only
 */
void ShapeNode::clearShapeTransform()
{
    shapeVertexMatrix = Matrix();
    shapeNormalMatrix = Matrix();
}

/**
 * Function: clearAllTransforms
 * 
 * Re-initializes all transforms made to the node
 */
void ShapeNode::clearAllTranforms()
{
    totalVertexMatrix = Matrix();
    totalNormalMatrix = Matrix();
    vertexMatrix = Matrix();
    normalMatrix = Matrix();
    shapeVertexMatrix = Matrix();
    shapeNormalMatrix = Matrix();
}


/**
 * Function: drawShapeTree
 * 
 * Initializes node and draws tree from it.
 * 
 * programID: OpenGL programID
 */
void ShapeNode::drawShapeTree(GLuint programID)
{
    transverseShapeTree(programID);
}