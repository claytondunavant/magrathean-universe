#include "Shader.h"
#include <GL/gl.h>
#include <cstddef>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut_std.h>

unsigned int VAO, VBO, shader_program;
Shader s;

void display() {
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    s.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

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

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
    };
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); // bind Vertex Array Object

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer to type GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy data into buffer
    
    // tell opengl how to interpret vertex data for a given attribute (in this case aPos)
    // argv[0]: attribute at location 0
    // argv[1]: size of vertex attribute (vec3 the attribute is made of 3 values)
    // argv[2]: datatype
    // argv[3]: want data to be normalized?
    // argv[4]: stride: space between vertex elements
    // argv[5]: offset where the poistion data begins in buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // enable the vertex attrib at location 0
    
    glutMainLoop();
}

