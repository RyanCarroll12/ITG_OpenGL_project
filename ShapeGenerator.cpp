/**
 * File: ShapeGenerator.cpp
 * 
 * Author: Ryan Carroll
 * CSC-350
 * This code is used for shape generation.
 * 
 * Shape generator holds reference to vector holding all
 * shape data.
 */

#include "ShapeGenerator.h"

ShapeGenerator::ShapeGenerator(std::vector<struct Vertex> &verts) : vertices(verts) {}
/**
 * Colon is used above to initialize vertices before constructor body executes, must be
 * used since references must be initialized before constructor executes.
 */

/**
 * Function: subdivideCylinderPoints
 * 
 * Breaks up two points into more.
 * 
 * circle_center: The coordinates for the center of the circle.
 * v_offset: The location to begin writing at in vertices.
 * p1: Point 1 on the edge.
 * p2: Point 2 on the edge.
 * center: center of the circle.
 * subdivision_level: The number of times the triangle is subdivided by.
 */
void ShapeGenerator::subdivideCylinderPoints(float circle_size, int &v_offset, mVec3 p1,
mVec3 p2, const mVec3 center, const mVec3 up_vector, unsigned subdivision_level)
{
    if(subdivision_level > 0)
    {
        // Lower subdivision level
        --subdivision_level;
        // Find midpoint
        mVec3 p1p2 = mVec3_midpoint(p1, p2);
        // Call the function on each of lines
        subdivideCylinderPoints(circle_size, v_offset, p1, p1p2, center, up_vector, subdivision_level);
        subdivideCylinderPoints(circle_size, v_offset, p1p2, p2, center, up_vector, subdivision_level);
    }
    else
    {
        // Subdivison is done, load normalized final points into vertices
        // Normalize the points
        p1 = mVec3_normalize(center, p1, circle_size / 2.0);
        p2 = mVec3_normalize(center, p2, circle_size / 2.0);

        mVec3 down_vector = {-up_vector.x, -up_vector.y, -up_vector.z};

        // Normalize vectors to create cylinder points.
        mVec3 p1_top = mVec3_normalize(p1, mVec3_add(p1, up_vector), circle_size);
        mVec3 p2_top = mVec3_normalize(p2, mVec3_add(p2, up_vector), circle_size);
        mVec3 center_top = mVec3_normalize(center, mVec3_add(center, up_vector), circle_size);
        mVec3 p1_bottom = mVec3_normalize(p1, mVec3_add(p1, down_vector), circle_size);
        mVec3 p2_bottom = mVec3_normalize(p2, mVec3_add(p2, down_vector), circle_size);
        mVec3 center_bottom = mVec3_normalize(center, mVec3_add(center, down_vector), circle_size);

        // Top triangle
        vertices[v_offset++].position = p1_top;
        vertices[v_offset++].position = center_top;
        vertices[v_offset++].position = p2_top;

        // Side triangles
        vertices[v_offset++].position = p1_top;
        vertices[v_offset++].position = p2_top;
        vertices[v_offset++].position = p1_bottom;
        vertices[v_offset++].position = p1_bottom;
        vertices[v_offset++].position = p2_top;
        vertices[v_offset++].position = p2_bottom;

        // Bottom triangle
        vertices[v_offset++].position = p2_bottom;
        vertices[v_offset++].position = center_bottom;
        vertices[v_offset++].position = p1_bottom;
    }
}

/**
 * Function: subdivideTriangles
 * 
 * Breaks up the coordinates of a larger triangle into the coordinates of smaller triangles
 * 
 * sphere_size: The scale of the sphere.
 * sphere_center: The coordinates for the center of the sphere.
 * v_offset: The location to begin writing at in vertices.
 * p1: Point 1 of the triangle.
 * p2: Point 2 of the triangle.
 * p3: Point 3 of the triangle.
 * subdivision_level: The number of times the triangle is subdivided by.
 */
void ShapeGenerator::subdivideTriangles(float sphere_size, mVec3 sphere_center,
int &v_offset, mVec3 p1, mVec3 p2, mVec3 p3, unsigned subdivision_level)
{
    if(subdivision_level > 0)
    {
        // Lower subdivision level
        --subdivision_level;
        // Find midpoints of the triangle
        mVec3 p1p2 = mVec3_midpoint(p1, p2);
        mVec3 p2p3 = mVec3_midpoint(p2, p3);
        mVec3 p3p1 = mVec3_midpoint(p3, p1);
        // Call the function on each of the inner triangles
        subdivideTriangles(sphere_size, sphere_center, v_offset, p1, p1p2, p3p1, subdivision_level);
        subdivideTriangles(sphere_size, sphere_center, v_offset, p1p2, p2p3, p3p1, subdivision_level);
        subdivideTriangles(sphere_size, sphere_center, v_offset, p3p1, p2p3, p3, subdivision_level);
        subdivideTriangles(sphere_size, sphere_center, v_offset, p1p2, p2, p2p3, subdivision_level);
    }
    else
    {
        // Subdivison is done, load normalized final points into vertices
        vertices[v_offset++].position = mVec3_normalize(sphere_center, p1, sphere_size / 2);
        vertices[v_offset++].position = mVec3_normalize(sphere_center, p2, sphere_size / 2);
        vertices[v_offset++].position = mVec3_normalize(sphere_center, p3, sphere_size / 2);
    }
}

/**
 * Function: add_cylinder
 * 
 * Adds a cylinder shape to vertices data.
 * 
 * color: The color of the cylinder.
 * diffuseStrength: The multiplier for how powerful diffuse light will affect this shape.
 * specularStrength: The multiplier for how powerful specular light will affect this shape.
 * subdivision_level: The level of detail applied to the cylinder
 * 
 * return: ShapeData struct holding the number of vertices and location the shape is at in vertices vector.
 */
ShapeData ShapeGenerator::add_cylinder(mVec3 color, float diffuseStrength, float specularStrength, const unsigned subdivision_level)
{
    struct ShapeData sd;
    sd.startLocation = vertices.size();

    float circle_size = 1.0f;
    // Circle coordinates
    float left_x = sqrt(3) * circle_size / -4.0f;
    float right_x = sqrt(3) * circle_size / 4.0f;
    float front_z = circle_size / -4.0f;
    float rear_z = circle_size / 2.0f;

    // Cylinder vertices
    mVec3 left = {left_x, 0, front_z};
    mVec3 right = {right_x, 0, front_z};
    mVec3 back = {0.0f, 0, rear_z};

    // Find the number of triangles we are rendering because why not
    int number_of_triangles = 4 * 3;
    number_of_triangles *= pow(2, subdivision_level);
    sd.verticeCount = number_of_triangles * VERTICES_PER_TRIANGLE;
    if (number_of_triangles > 50000)
        std::cout << "Cylinder triangle count: " << number_of_triangles << std::endl;

    // Make some room for new vertices.
    const int offset = vertices.size();
    for (int i = 0; i < number_of_triangles * VERTICES_PER_TRIANGLE; i++)
    {
        vertices.push_back(Vertex());
    }
    int v_offset = offset;

    // Calculate the new cylinder size, center, and the rotation.
    circle_size = (mVec3_calc_distance(left, right) * 2) / sqrt(3);
    mVec3 circle_center = {(left.x + right.x + back.x) / 3.0f, (left.y + right.y + back.y) / 3.0f, (left.z + right.z + back.z) / 3.0f};
    mVec3 up_vector = mVec3_calc_triangle_normal(left, circle_center, right);

    // Divide the triangle into three triangles then subdivide those.
    subdivideCylinderPoints(circle_size, v_offset, left, right, circle_center, up_vector, subdivision_level);
    subdivideCylinderPoints(circle_size, v_offset, right, back, circle_center, up_vector, subdivision_level);
    subdivideCylinderPoints(circle_size, v_offset, back, left, circle_center, up_vector, subdivision_level);

    // Load colors and normals
    for (int i = offset; i < offset + (number_of_triangles * VERTICES_PER_TRIANGLE); i = i + 3)
    {
        mVec3 triangle_normal = mVec3_calc_triangle_normal(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position);
        vertices[i].normal = triangle_normal;
        vertices[i + 1].normal = triangle_normal;
        vertices[i + 2].normal = triangle_normal;
    }

    for (int i = offset; i < offset + (number_of_triangles * VERTICES_PER_TRIANGLE); i++)
    {
        vertices[i].color = color;
        vertices[i].diffuseStrength = diffuseStrength;
        vertices[i].specularStrength = specularStrength;
    }

    return sd;
}

/**
 * Function: add_sphere
 * This function adds sphere data to vector vertices.
 * 
 * sphere_size: The desired size of the sphere
 * color: The color of the sphere.
 * diffuseStrength: The multiplier for how powerful diffuse light will affect this shape.
 * specularStrength: The multiplier for how powerful specular light will affect this shape.
 * subdivision_level: The level of detail of the sphere, increases exponentially.
 * 
 * return: ShapeData struct holding the number of vertices and location the shape is at in vertices vector.
 */
ShapeData ShapeGenerator::add_sphere(mVec3 color, float diffuseStrength, float specularStrength, const unsigned subdivision_level)
{
    struct ShapeData sd;
    sd.startLocation = vertices.size();

    float sphere_size = 1.0f;
    // Octahedron coordinates
    float left_x = sqrt(2) * sphere_size / -4.0;
    float right_x = sqrt(2) * sphere_size / 4.0;
    float bottom_y = sphere_size / -2.0;
    float top_y = bottom_y + sphere_size;
    float negative_z = sqrt(2) * sphere_size / -4.0;
    float positive_z = sqrt(2) * sphere_size / 4.0;

    // Octahedron vertices
    mVec3 left_front = {left_x, 0, negative_z};
    mVec3 left_rear = {left_x, 0, positive_z};
    mVec3 top = {0, top_y, 0};
    mVec3 bottom = {0, bottom_y, 0};
    mVec3 right_front = {right_x, 0, negative_z};
    mVec3 right_rear = {right_x, 0, positive_z};

    // Need to find new sphere size
    sphere_size = top.y - bottom.y;

    // Find the number of triangles we are rendering because why not
    int number_of_triangles = 8;
    number_of_triangles *= pow(4, subdivision_level);
    sd.verticeCount = number_of_triangles * VERTICES_PER_TRIANGLE;
    if (number_of_triangles > 50000)
        std::cout << "Sphere triangle count: " << number_of_triangles << std::endl;

    // Make space for vertex data

    const int offset = vertices.size();
    for (int i = 0; i < number_of_triangles * VERTICES_PER_TRIANGLE; i++)
    {
        vertices.push_back(Vertex());
    }
    int v_offset = offset;
    mVec3 sphere_center = {mVec3_midpoint(top, bottom)};

    //front top triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, left_front, top, right_front, subdivision_level);

    //front bottom triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, left_front, right_front, bottom, subdivision_level);

    //right top triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, right_front, top, right_rear, subdivision_level);

    //right bottom triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, right_front, right_rear, bottom, subdivision_level);

    //back top triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, right_rear, top, left_rear, subdivision_level);

    //back bottom triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, left_rear, bottom, right_rear, subdivision_level);

    //left top triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, left_rear, top, left_front, subdivision_level);

    //left bottom triangle
    subdivideTriangles(sphere_size, sphere_center, v_offset, left_rear, left_front, bottom, subdivision_level);

    // Load colors and normals
    for (int i = offset; i < offset + (number_of_triangles * VERTICES_PER_TRIANGLE); i = i + 3)
    {
        mVec3 triangle_normal = mVec3_calc_triangle_normal(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position);
        vertices[i].normal = triangle_normal;
        vertices[i + 1].normal = triangle_normal;
        vertices[i + 2].normal = triangle_normal;
    }

    for (int i = offset; i < offset + (number_of_triangles * VERTICES_PER_TRIANGLE); i++)
    {
        vertices[i].color = color;
        vertices[i].diffuseStrength = diffuseStrength;
        vertices[i].specularStrength = specularStrength;
    }

    return sd;
}

/**
 * Function: add_cube
 * This function adds cube data to the vector vertices.
 * 
 * cube_size: The size of the cube, same on every side.
 * color: The color of the cube.
 * diffuseStrength: The multiplier for how powerful diffuse light will affect this shape.
 * specularStrength: The multiplier for how powerful specular light will affect this shape.
 * 
 * return: ShapeData struct holding the number of vertices and location the shape is at in vertices vector.
 */
ShapeData ShapeGenerator::add_cube(mVec3 color, float diffuseStrength, float specularStrength)
{
    struct ShapeData sd;
    sd.startLocation = vertices.size();

    float cube_size = 1.0f;
    // Cube coordinates
    float left_x = cube_size / -2.0f;
    float right_x = left_x + cube_size;
    float bottom_y = cube_size / -2.0f;
    float top_y = bottom_y + cube_size;
    float negative_z = cube_size / -2.0f;
    float positive_z = negative_z + cube_size;

    // Cube vertices
    mVec3 left_top_front = {left_x, top_y, negative_z};
    mVec3 left_top_rear = {left_x, top_y, positive_z};
    mVec3 right_top_front = {right_x, top_y, negative_z};
    mVec3 right_top_rear = {right_x, top_y, positive_z};
    mVec3 left_bottom_front = {left_x, bottom_y, negative_z};
    mVec3 left_bottom_rear = {left_x, bottom_y, positive_z};
    mVec3 right_bottom_front = {right_x, bottom_y, negative_z};
    mVec3 right_bottom_rear = {right_x, bottom_y, positive_z};

    // Vector stuff

    const int offset = vertices.size();

    for (int i = 0; i < VERTICES_PER_CUBE; i++)
    {
        vertices.push_back(Vertex());
    }

    /* Loading cube vertices */

    int v_offset = offset;

    // Cube front
    vertices[v_offset++].position = left_top_front;
    vertices[v_offset++].position = right_top_front;
    vertices[v_offset++].position = left_bottom_front;
    vertices[v_offset++].position = right_bottom_front;
    vertices[v_offset++].position = left_bottom_front;
    vertices[v_offset++].position = right_top_front;
    // Cube rear
    vertices[v_offset++].position = left_top_rear;
    vertices[v_offset++].position = left_bottom_rear;
    vertices[v_offset++].position = right_top_rear;
    vertices[v_offset++].position = right_bottom_rear;
    vertices[v_offset++].position = right_top_rear;
    vertices[v_offset++].position = left_bottom_rear;
    // Cube left
    vertices[v_offset++].position = left_top_rear;
    vertices[v_offset++].position = left_top_front;
    vertices[v_offset++].position = left_bottom_rear;
    vertices[v_offset++].position = left_top_front;
    vertices[v_offset++].position = left_bottom_front;
    vertices[v_offset++].position = left_bottom_rear;
    // Cube right
    vertices[v_offset++].position = right_top_rear;
    vertices[v_offset++].position = right_bottom_rear;
    vertices[v_offset++].position = right_top_front;
    vertices[v_offset++].position = right_bottom_front;
    vertices[v_offset++].position = right_top_front;
    vertices[v_offset++].position = right_bottom_rear;
    // Cube top
    vertices[v_offset++].position = left_top_rear;
    vertices[v_offset++].position = right_top_rear;
    vertices[v_offset++].position = left_top_front;
    vertices[v_offset++].position = right_top_front;
    vertices[v_offset++].position = left_top_front;
    vertices[v_offset++].position = right_top_rear;
    // Cube bottom
    vertices[v_offset++].position = left_bottom_rear;
    vertices[v_offset++].position = left_bottom_front;
    vertices[v_offset++].position = right_bottom_rear;
    vertices[v_offset++].position = right_bottom_front;
    vertices[v_offset++].position = right_bottom_rear;
    vertices[v_offset++].position = left_bottom_front;

    /* Color and normals */

    for (int i = offset; i < offset + VERTICES_PER_CUBE; i = i + 3)
    {
        mVec3 triangle_normal = mVec3_calc_triangle_normal(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position);
        vertices[i].normal = triangle_normal;
        vertices[i + 1].normal = triangle_normal;
        vertices[i + 2].normal = triangle_normal;
    }

    for (int i = offset; i < offset + VERTICES_PER_CUBE; i++)
    {
        vertices[i].color = color;
        vertices[i].diffuseStrength = diffuseStrength;
        vertices[i].specularStrength = specularStrength;
    }

    sd.verticeCount = VERTICES_PER_CUBE;

    return sd;
}

/**
 * Function: load_3d_color_grid
 *
 * Loads color and vertex information into vertices array.
 * 
 * colors: Array of color infomation for pixel art.
 * COLORS_HEIGHT: Height of pixel art.
 * COLORS_WIDTH: Width of pixel art.
 * cube_size: Desired size of the cubes in the grid.
 */
void ShapeGenerator::load_3d_color_grid(mVec3 colors[], const int COLORS_HEIGHT, const int COLORS_WIDTH, GLfloat cube_size)
{
    GLfloat draw_space_size = sqrt(COLORS_HEIGHT * COLORS_WIDTH) - 1.0;
    GLfloat left_x = draw_space_size * cube_size / -2.0;
    GLfloat top_y = draw_space_size * cube_size / 2.0;

    for (int y = 0; y < COLORS_HEIGHT; y++)
    {
        for (int x = 0; x < COLORS_WIDTH; x++)
        {
            float fx = left_x + (x * cube_size);
            float fy = top_y - (y * cube_size);
            Matrix grid_transform;
            grid_transform.scale(cube_size);
            grid_transform.translate({fx, fy, 0});
            add_cube(colors[(y * COLORS_WIDTH) + x], 1.0f, 1.0f);
        }
    }
}

/**
 * Function: add_tree
 * 
 * Adds tree shape group to a ground node
 * 
 * ground: Node which trees inheirt transformations from
 */
void ShapeGenerator::add_tree(ShapeNode *ground, float tree_size, mVec3 tree_location)
{
    ShapeNode *tree = new ShapeNode;
    tree->scaleNode(tree_size);
    tree->translateNode({0.0f, (tree_size / 2.0f) + 0.5f, 0.0f});
    tree->translateNode(tree_location);
    ground->add_child(tree);

    ShapeNode *treeTrunk = new ShapeNode;
    treeTrunk->setShapeData(add_cube({0.2f, 0.13f, 0.07f}, 1.0f, 0.1f));
    treeTrunk->scaleShape({0.3f, 1.0f, 0.3f});
    tree->add_child(treeTrunk);

    ShapeNode *treeLeaves = new ShapeNode;
    treeLeaves->setShapeData(add_sphere({0.3, 1, 0}, 1.0f, 0.0f, 2));
    treeLeaves->translateNode({0.0f, 0.5f, 0.0f});
    tree->add_child(treeLeaves);
}