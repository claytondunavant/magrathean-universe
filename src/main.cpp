#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut_std.h>

#include <cstddef>
#include <iostream>
#include <vector>

#include "Shader.h"
#include "Sphere.h"

unsigned int VAO, VBO, EBO, shader_program;
Shader s;

Sphere sphere = Sphere(0.5f, 36, 18);
std::vector<float> vertices;
std::vector<unsigned int> indices;

void display() {
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    s.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    
    // initGLUT
    int width = 800; int height = 800;
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    
    glutCreateWindow("FreeGLUT Window");
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    // GLUT callbacks
    glutDisplayFunc(display);
    
    // init glew
    GLenum glew_status = glewInit();
    if ( glew_status != GLEW_OK ) {
        std::cout << "GLEW did not init correctly" << std::endl;
        return 1;
    }

    // TODO: make not relative paths ??
    s = Shader("../shaders/v.glsl", "../shaders/f.glsl");

    // set up vertex data and buffers
    sphere.build_vertices();
    vertices = sphere.get_vertices();
    indices = sphere.get_indices();
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind Vertex Array Object
    glBindVertexArray(VAO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW); // copy data into buffer
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy data into buffer
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    
    // tell opengl how to interpret vertex data for a given attribute (in this case aPos)
    // argv[0]: attribute at location 0
    // argv[1]: size of vertex attribute (vec3 the attribute is made of 3 values)
    // argv[2]: datatype
    // argv[3]: want data to be normalized?
    // argv[4]: stride: space between vertex elements
    // argv[5]: offset where the poistion data begins in buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // enable the vertex attrib at location 0
                                
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glutMainLoop();
}

