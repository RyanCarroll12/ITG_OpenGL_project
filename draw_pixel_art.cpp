/**
 * File: draw_pixel_art.cpp
 * 
 * Author: Ryan Carroll
 * Homework Final for CSC350
 * This code does not draw pixel art, it renders a 3D scene.
 */

#include <iostream>
#include <vector>

#include <GL/glew.h> // Core OpenGL
#include <GL/glut.h> // Display Window
#include "Vector.h"
#include "Matrix.h"
#include "ShapeGenerator.h"
#include "ShapeNode.h"
#include "shader_code.cpp"

// Not mine.
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

#define NUMBER_OF_DIMENSIONS 3
#define NUMBER_OF_COLORS 3
#define VERTICES_PER_TRIANGLE 3
#define VERTICES_PER_SQUARE VERTICES_PER_TRIANGLE * 2
#define VERTICES_PER_CUBE VERTICES_PER_SQUARE * 6

static const unsigned windowWidth = 512;
static const unsigned windowHeight = 512;

GLuint vertex_buffer_ID;
GLuint programID;

static ShapeNode *globalBaseShapeNode;

mVec3 W = {1, 1, 1};// Titanium White
mVec3 B = {0, 1, 1}; // Sky Blue
mVec3 G = {0.3, 1, 0}; // Grassy Green
mVec3 R = {0.812, 0.114, 0.122}; // Mario Red
mVec3 DB = {0.494, 0.318, 0.129}; // Dark Brown
mVec3 LB = {0.878, 0.69, 0.337}; // Light Brown

#define MARIO_HEIGHT 16
#define MARIO_WIDTH 16

mVec3 mario[] = 
{
      W , W , W , B , B , R , R , R , R , R , B , B , B , B , B , B
    , W , W , B , B , R , R , R , R , R , R , R , R , R , B , B , W
    , B , B , B , B , DB, DB, DB, LB, LB, DB, LB, B , B , B , W , W
    , B , B , B , DB, LB, DB, LB, LB, LB, DB, LB, LB, LB, B , B , W
    , W , W , B , DB, LB, DB, DB, LB, LB, LB, DB, LB, LB, LB, B , B
    , W , B , B , DB, DB, LB, LB, LB, LB, DB, DB, DB, DB, B , B , B
    , B , B , B , B , B , LB, LB, LB, LB, LB, LB, LB, B , B , B , B
    , B , B , B , B , DB, DB, R , DB, DB, DB, B , B , B , B , B , B
    , B , B , B , DB, DB, DB, R , DB, DB, R , DB, DB, DB, B , B , B
    , B , B , DB, DB, DB, DB, R , R , R , R , DB, DB, DB, DB, B , B
    , B , B , LB, LB, DB, R , LB, R , R , LB, R , DB, LB, LB, B , B
    , B , B , LB, LB, LB, R , R , R , R , R , R , LB, LB, LB, B , B
    , B , B , LB, LB, R , R , R , R , R , R , R , R , LB, LB, B , B
    , B , B , B , B , R , R , R , B , B , R , R , R , B , B , B , B
    , B , B , B , DB, DB, DB, B , B , B , B , DB, DB, DB, B , B , B
    , G , G , DB, DB, DB, DB, G , G , G , G , DB, DB, DB, DB, G , G
};


/**
 * Function: add_ambient_light
 * 
 * This function loads uniform values into the shaders for ambient light.
 * The color is normalized to match the strength.
 * 
 * ambientColor: The color of the light.
 * ambientStrength: The strength of the light.
 */
void add_ambient_light(mVec3 ambientColor, float ambientStrength)
{
    mVec3 ambientValues = mVec3_normalize({}, ambientColor, ambientStrength);
    GLfloat ambient[] = {ambientValues.x, ambientValues.y, ambientValues.z, 1.0};
    GLint ambientLight = glGetUniformLocation(programID, "ambientLight");
    glUniform4fv(ambientLight, 1, &ambient[0]);
}

/**
 * Function: add_infinite_light
 * 
 * This function loads uniform values into the shaders for directional light.
 * 
 * infiniteLightDirection: The direction of the light, will be normalized.
 * infiniteLightColor: The color of the light.
 */
void add_infinite_light(mVec3 infiniteLightDirection, mVec3 infiniteLightColor)
{
    infiniteLightDirection = mVec3_unit_normalize(infiniteLightDirection);
    GLint infiniteDir = glGetUniformLocation(programID, "infiniteLightDirection");
    glUniform3fv(infiniteDir, 1, &infiniteLightDirection.x);

    GLint infiniteColor = glGetUniformLocation(programID, "infiniteLightColor");
    glUniform3fv(infiniteColor, 1, &infiniteLightColor.x);
}

/**
 * Function: add_point_light
 * 
 * This function loads uniform values into the shaders for point light.
 * 
 * pointLightPosition: The position of the light.
 * pointLightColor: The color of the light.
 */
void add_point_light(mVec3 pointLightPosition, mVec3 pointLightColor)
{
    GLint pLightPos = glGetUniformLocation(programID, "pointLightPosition");
    glUniform3fv(pLightPos, 1, &pointLightPosition.x);

    GLint pLightColor = glGetUniformLocation(programID, "pointLightColor");
    glUniform3fv(pLightColor, 1, &pointLightColor.r);
}

/**
 * Function: add_spot_light
 * 
 * This function loads uniform values into the shaders for spot light.
 * 
 * spotLightPosition: The position of the light.
 * spotLightColor: The color of the light.
 * coneDirection: The direction of the light, will be normalized.
 * spotCutoffDegrees: The cone of light's width in degrees.
 */
void add_spot_light(mVec3 spotLightPosition, mVec3 spotLightColor, mVec3 coneDirection, float spotCutoffDegrees)
{
    GLint sLightPos = glGetUniformLocation(programID, "spotLightPosition");
    glUniform3fv(sLightPos, 1, &spotLightPosition.x);

    GLint spotLC = glGetUniformLocation(programID, "spotLightColor");
    glUniform3fv(spotLC, 1, &spotLightColor.x);

    coneDirection = mVec3_unit_normalize(coneDirection);
    GLint coneDir = glGetUniformLocation(programID, "coneDirection");
    glUniform3fv(coneDir, 1, &coneDirection.x);

    float spotCutoff = cos(spotCutoffDegrees * PI / 180.0);
    GLint spotCC = glGetUniformLocation(programID, "spotCosCutoff");
    glUniform1f(spotCC, spotCutoff);
}

/**
 * Function: checkShaderStatus
 * 
 * Code from openGL video. Checks compilation status of shaderID.
 * shaderID: The shaderID.
 * 
 * returns: GL_TRUE if success, GL_FALSE if failure
 */
GLboolean checkShaderStatus(GLuint shaderID)
{
    GLint compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* buffer = new GLchar[infoLogLength];

        GLsizei bufferSize;
        glGetShaderInfoLog(shaderID, infoLogLength, &bufferSize, buffer);
        std::cout << buffer << std::endl;
        delete [] buffer;
        return GL_FALSE;
    }
    return GL_TRUE;
}

/**
 * Function: read_data_from_file
 * 
 * Reads characters from file into malloced memory.
 * Only tested for reading shader code so caution using it anywhere else.
 * 
 * filename: Name of the file to be read.
 * data: Data read from the file.
 * 
 * Return: Number of bytes read.
 */
GLuint read_data_from_file(const char * filename, char *data)
{
    FILE *shader_code_fp = fopen(filename, "r");

    if (shader_code_fp == NULL)
    {
        perror("Unable to open file");
    }

    fseek(shader_code_fp, 0, SEEK_END);
    int file_size = ftell(shader_code_fp);
    fseek(shader_code_fp, 0, SEEK_SET);
    data = (char *) malloc(file_size + 1);
    fread(data, sizeof(char), file_size, shader_code_fp);
    fclose(shader_code_fp);

    return file_size;
}

/**
 * Function: installShaders
 * 
 * Creates, loads, compiles, and begins running shaders.
 */
void installShaders(void)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    const GLchar* adapter[1];

    adapter[0] = VertexShaderCode; // buffer
    // (The shader ID, how many are in the char * array, the array, how long is it with 0 meaning until null terminator)
    glShaderSource(vertexShaderID, 1, adapter, 0);

    adapter[0] = FragmentShaderCode; // buffer

    glShaderSource(fragmentShaderID, 1, adapter, 0);

    glCompileShader(vertexShaderID);
    glCompileShader(fragmentShaderID);

    if(!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
        return;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    glUseProgram(programID);
}

/**
 * Function: load_shape_node_tree
 * 
 * Creates a scene using a tree of shape nodes using the base_node as a base.
 * 
 * base_node: The base of the tree.
 * vertices: A reference to the vector holding the shape data.
 */
void load_shape_node_tree(ShapeNode *base_node, std::vector<struct Vertex> &vertices)
{
    ShapeGenerator *sg = new ShapeGenerator(vertices);

    Matrix perspectiveMatrix;
    perspectiveMatrix.projectPerspective(60.0f, ((float)windowWidth)/((float)windowHeight), 0.1f, 10000.0f);

    base_node->setPerspectiveMatrix(perspectiveMatrix);

    mVec3 headColor = LB;
    mVec3 eyeColor = DB;
    mVec3 shirtColor = {0.0f, 1.0f, 1.0f};
    mVec3 armColor = headColor;
    mVec3 pantsColor = {0.5f, 0, 1.0f};
    mVec3 pickHandleColor = {0.2f, 0.13f, 0.07f};
    mVec3 pickHeadColor = {0.2f, 1.0f, 1.0f};

    float body_specular = 0.01f;


    ShapeNode *body = new ShapeNode;
    body->setShapeData(sg->add_cube(shirtColor, 0.5, body_specular));
    body->scaleShape({1.0f, 1.0f, 0.5f});
    base_node->add_child(body);
    
    ShapeNode *leftArmSocket = new ShapeNode;
    leftArmSocket->translateNode({-0.75f, 0.5, 0.0f});
    body->add_child(leftArmSocket);

    ShapeNode *leftArm = new ShapeNode;
    leftArm->setShapeData(sg->add_cube(armColor, 0.5f, body_specular));
    leftArm->scaleShape({0.5f, 1.0f, 0.5f});
    leftArm->translateNode({0.0f, -0.5f, 0.0f});
    leftArmSocket->add_child(leftArm);

    ShapeNode *rightArmSocket = new ShapeNode;
    rightArmSocket->translateNode({0.75f, 0.5f, 0.0f});
    body->add_child(rightArmSocket);

    ShapeNode *rightArm = new ShapeNode;
    rightArm->setShapeData(sg->add_cube(armColor, 0.5f, body_specular));
    rightArm->scaleShape({0.5f, 1.0f, 0.5f});
    rightArm->translateNode({0.0f, -0.5f, 0.0f});
    rightArmSocket->add_child(rightArm);

    ShapeNode *rightLegSocket = new ShapeNode;
    rightLegSocket->translateNode({0.25f, -0.5f, 0.0f});
    body->add_child(rightLegSocket);

    ShapeNode *rightLeg = new ShapeNode;
    rightLeg->setShapeData(sg->add_cube(pantsColor, 0.3f, body_specular));
    rightLeg->scaleShape({0.5f, 1.0f, 0.5f});
    rightLeg->translateNode({0.0f, -0.5f, 0.0f});
    rightLegSocket->add_child(rightLeg);

    ShapeNode *leftLegSocket = new ShapeNode;
    leftLegSocket->translateNode({-0.25f, -0.5f, 0.0f});
    body->add_child(leftLegSocket);

    ShapeNode *leftLeg = new ShapeNode;
    leftLeg->setShapeData(sg->add_cube(pantsColor, 0.3f, body_specular));
    leftLeg->scaleShape({0.5f, 1.0f, 0.5f});
    leftLeg->translateNode({0.0f, -0.5f, 0.0f});
    leftLegSocket->add_child(leftLeg);
    

    ShapeNode *head = new ShapeNode;
    head->setShapeData(sg->add_cube(headColor, 0.5f, body_specular));
    head->translateNode({0.0f, 1.0f, 0.0f});
    body->add_child(head);

    ShapeNode *rightEye = new ShapeNode;
    rightEye->setShapeData(sg->add_cube(eyeColor, 0.5f, 1.0f));
    rightEye->scaleNode(0.1f);
    rightEye->translateNode({0.2f, 0.0f, -0.5f});
    head->add_child(rightEye);

    ShapeNode *rightEyeWhite = new ShapeNode;
    rightEyeWhite->setShapeData(sg->add_cube(W, 0.5f, 1.0f));
    rightEyeWhite->translateNode({1.0f, 0.0f, 0.0f});
    rightEye->add_child(rightEyeWhite);

    ShapeNode *leftEye = new ShapeNode;
    leftEye->setShapeData(sg->add_cube(eyeColor, 0.5f, 1.0f));
    leftEye->scaleNode(0.1f);
    leftEye->translateNode({-0.2f, 0.0f, -0.5f});
    head->add_child(leftEye);

    ShapeNode *leftEyeWhite = new ShapeNode;
    leftEyeWhite->setShapeData(sg->add_cube(W, 0.5f, 1.0f));
    leftEyeWhite->translateNode({-1.0f, 0.0f, 0.0f});
    leftEye->add_child(leftEyeWhite);


    ShapeNode *pick = new ShapeNode;
    pick->rotateNode({90.0f, 0.0f, 0.0f});
    pick->translateNode({0.0f, -0.3f, -0.4f});
    leftArm->add_child(pick);

    ShapeNode *pickHandle = new ShapeNode;
    pickHandle->setShapeData(sg->add_cylinder(pickHandleColor, 1.0f, 0.0f, 1));
    pickHandle->scaleShape({0.3f, 1.0f, 0.3f});
    pick->add_child(pickHandle);

    ShapeNode *pickHeadFront = new ShapeNode;
    pickHeadFront->setShapeData(sg->add_cylinder(pickHeadColor, 0.5f, 1.0f, 1));
    pickHeadFront->scaleShape({0.25f, 0.5f, 0.25f});
    pickHeadFront->translateNode({0.0f, 0.6f, -0.3f});
    pickHeadFront->rotateNode({-70.0f, 0.0f, 0.0f});
    pick->add_child(pickHeadFront);

    ShapeNode *pickHeadRear = new ShapeNode;
    pickHeadRear->setShapeData(sg->add_cylinder(pickHeadColor, 0.5f, 1.0f, 1));
    pickHeadRear->scaleShape({0.25f, 0.5f, 0.25f});
    pickHeadRear->translateNode({0.0f, 0.6f, 0.3f});
    pickHeadRear->rotateNode({70.0f, 0.0f, 0.0f});
    pick->add_child(pickHeadRear);


    ShapeNode *skyBox = new ShapeNode;
    skyBox->setShapeData(sg->add_sphere({0.1f, 0.2f, 0.5f}, 1.0f, 0.0f, 5));
    skyBox->scaleNode(4000.0f);
    base_node->add_child(skyBox);

    ShapeNode *ground = new ShapeNode;
    ground->setShapeData(sg->add_cube(G, 0.5, 0.0));
    ground->scaleShape({100.0f, 1.0f, 100.0f});
    ground->translateNode({0.0f, -2.0f, 0.0f});
    base_node->add_child(ground);

    float tree_amount = 12;
    for (float z = -tree_amount; z < tree_amount; z++)
    {
        sg->add_tree(ground, 4.0f, {4.0f, 0.0f, z * 4.0f});
        sg->add_tree(ground, 4.0f, {-4.0f, 0.0f, z * 4.0f});
    }
}

/**
 * Function: init
 * Initializes Opengl buffers and vertex attributes. Also adds shapes
 * 
 * vertices: A reference to the vector holding the shape data.
 */
void init(std::vector<struct Vertex> &vertices)
{
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;

    // This tells openGL to do depth comparisions and update depth buffer.
    glEnable(GL_DEPTH_TEST);

    globalBaseShapeNode = new ShapeNode;

    load_shape_node_tree(globalBaseShapeNode, vertices);

    glGenBuffers(1, &vertex_buffer_ID);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(struct Vertex), &vertices[0], GL_STATIC_DRAW);

    GLuint vPosition = 0; // vertex position
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, NUMBER_OF_DIMENSIONS, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), BUFFER_OFFSET(0));

    GLuint vColor = 1; // vertex color
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, NUMBER_OF_COLORS, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), BUFFER_OFFSET(sizeof(GLfloat) * NUMBER_OF_DIMENSIONS));

    GLuint vNormal = 2; // vertex normal
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, NUMBER_OF_COLORS, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), BUFFER_OFFSET(sizeof(GLfloat) * (NUMBER_OF_DIMENSIONS + NUMBER_OF_COLORS)));

    GLuint vDiffuseStrength = 3;
    glEnableVertexAttribArray(vDiffuseStrength);
    glVertexAttribPointer(vDiffuseStrength, NUMBER_OF_COLORS, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), BUFFER_OFFSET(sizeof(GLfloat) * (NUMBER_OF_DIMENSIONS + NUMBER_OF_COLORS + NUMBER_OF_DIMENSIONS)));

    GLuint vSpecularStrength = 4;
    glEnableVertexAttribArray(vSpecularStrength);
    glVertexAttribPointer(vSpecularStrength, NUMBER_OF_COLORS, GL_FLOAT, GL_FALSE,
    sizeof(Vertex), BUFFER_OFFSET(sizeof(GLfloat) * (NUMBER_OF_DIMENSIONS + NUMBER_OF_COLORS + NUMBER_OF_DIMENSIONS + 1)));

    installShaders();
}

/**
 * Function: add_light
 * 
 * Loading lighting for scene;
 */
void add_light()
{
    mVec3 ambientLightColor = {1.0f, 1.0f, 1.0f};
    float ambientLightStrength = 0.3;
    add_ambient_light(ambientLightColor, ambientLightStrength);

    mVec3 infiniteLightDirection = {1.0f, 1.0f, 1.0f};
    mVec3 infiniteLightColor = {0.5f, 0.5f, 0.0f};
    add_infinite_light(infiniteLightDirection, infiniteLightColor);

    mVec3 pointLightPosition = {0.0f, 5.0f, 3.0f};
    mVec3 pointLightColor = {0.4f, 0.4f, 1.0f};
    add_point_light(pointLightPosition, pointLightColor);

    /*mVec3 spotLightPosition = {0.0f, 0.0f, 0.01f};
    mVec3 spotLightColor = {1.0f, 1.0f, 1.0f};
    mVec3 coneDirection = {0.0f, 0.0f, 1.0f};
    float spotCosCutoff = 10.0f;
    add_spot_light(spotLightPosition, spotLightColor, coneDirection, spotCosCutoff);*/
}

/**
 * Function: display
 * This is the display function passed into glutDisplayFunc().
 */
float rotation_global = 0.0f;
float ground_total_movement_global = 0.0f;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    add_light();

    globalBaseShapeNode->applyTranslation({0.0f, 0.0f, 3.0f});
    globalBaseShapeNode->applyRotation({(float) (20.0f * sin((rotation_global * PI * 1.5f) / 180.0f)), rotation_global, 0.0f});

    rotation_global += 0.01;

    // Move arms and legs
    for (int i = 0; i < 4; i+=2)
    {
        globalBaseShapeNode->children[0]->children[i]->rotateNode({cos(rotation_global), 0.0f, 0.0f});
    }
    for (int i = 1; i < 4; i+=2)
    {
        globalBaseShapeNode->children[0]->children[i]->rotateNode({(float)cos(rotation_global + PI), 0.0f, 0.0f});
    }

    // Move ground
    if (ground_total_movement_global >= 4.0f)
    {
        ground_total_movement_global = 0.0f;
        globalBaseShapeNode->children[2]->clearNodeTransform();
        globalBaseShapeNode->children[2]->translateNode({0.0f, -2.0f, 0.0f});
    }
    else
    {
        float ground_translate_amount = 0.01f;
        ground_total_movement_global += ground_translate_amount;
        globalBaseShapeNode->children[2]->translateNode({0.0f, 0.0f, ground_translate_amount});
    }

    globalBaseShapeNode->drawShapeTree(programID);

    glFlush();
    glutPostRedisplay();
}

/**
 * Function: main
 * This function initializes glut and displays the image
 * 
 * argc: The number of arguments passed to the program.
 * argv: The arguments passed to the program.
 */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(argv[0]);

    if (glewInit()) 
    {
	    std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	    exit(EXIT_FAILURE);
    }

    std::vector<struct Vertex> vertices;

    init(vertices);

    glutDisplayFunc(display);

    glutMainLoop();
}