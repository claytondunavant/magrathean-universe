//http://www.songho.ca/opengl/gl_sphere.html

#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <math.h>

#include "Sphere.h"

Sphere::Sphere(float r, glm::vec3 position, int sector_count, int stack_count) {
    this->r = r;
    this->position = position;
    this->sector_count = sector_count;
    this->stack_count = stack_count; 
    
    // build vertices and indices
    build_vertices();
    
    // TODO: make not relative paths ??
    // init shader
    shader = Shader("../shaders/v.glsl", "../shaders/f.glsl");

    // init rendering
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind Vertex Array Object
    glBindVertexArray(VAO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // enable the vertex attrib at location 0
}

// TODO: be able to draw two spheres
void Sphere::draw(glm::mat4 view, glm::mat4 projection) {

    shader.use(); 

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model *= glm::translate(glm::mat4(1.0f), position);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
   
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
    
}

void Sphere::build_vertices()
{
    const float PI = acos(-1.f);

    float x, y, z, xy;      // vertex position

    float sector_step = 2 * PI / sector_count;
    float stack_step = PI / stack_count;
    float sector_angle, stack_angle;

    // iterate through stack (vertical panes on sphere)
    for (int i = 0; i <= stack_count; ++i) {
        
        stack_angle = PI / 2 - i * stack_step; // start from pi/2 to -pi/2
        xy = r * cosf(stack_angle); // r * cos(u)
        z = r * sinf(stack_angle); // r * sin(u)
                                
        // iterate through sectors (horizontal panes on sphere) in this level of the stack
        for (int j = 0; j <= sector_count; ++j) {
            sector_angle = j * sector_step; // start from 0 to 2pi
                                        
            // vertex position
            x = xy * cosf(sector_angle); // r * cos(u) * cos(v)
            y = xy * sinf(sector_angle); // r * cos(u) * sin(v)

            add_vertex(x, y, z);
        }
    }
    
    // indices
    // k1 -- k1 + 1
    // |    / |
    // |   /  |
    // | /    |
    // k2 -- k2 + 1
    unsigned int k1, k2;
    for (int i = 0; i < stack_count; ++i) {
        k1 = i * (sector_count + 1); // begining of current stack
        k2 = k1 + (sector_count + 1); // beginning of next stack

        for (int j = 0; j < sector_count; ++j, ++k1, ++k2) {

            // 2 triangles per sector excluding 1st and last stacks
            if ( i != 0 ) // add top triangle
            {
                add_indices(k1, k2, k1+1); 
            }

            if ( i != (stack_count - 1 )) // add bottom triangle
            {
                add_indices(k1+1, k2, k2+1);
            }
        }
    }
}

std::vector<float> Sphere::get_vertices() {
   return vertices; 
}

std::vector<unsigned int> Sphere::get_indices() {
   return indices; 
}

glm::vec3 Sphere::get_position() {
    return position;
}

void Sphere::set_position(glm::vec3 new_position) {
    position = new_position;
}

void Sphere::add_vertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void Sphere::add_indices(float i1, float i2, float i3) {
    indices.push_back(i1);
    indices.push_back(i2);    
    indices.push_back(i3);    
}