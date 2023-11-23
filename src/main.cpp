// https://learnopengl.com/Getting-started/Coordinate-Systems
// https://learnopengl.com/Getting-started/Camera

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut_std.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <cstdlib>

#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

unsigned int VAO, VBO, EBO, shader_program;
Shader s;

Sphere sphere = Sphere(0.5f, 36, 18);
std::vector<float> vertices;
std::vector<unsigned int> indices;

Camera camera = Camera();

void display() {
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    s.use();
    
    // tranformation
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = camera.get_view_matrix();
    projection = glm::perspective(glm::radians(45.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
    s.setMat4("model", model);
    s.setMat4("view", view);
    s.setMat4("projection", projection);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }

    glutSwapBuffers();
}

void processKeyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            camera.move_forward();
            break;
        case 's':
            camera.move_backward();
            break;
        case 'a':
            camera.move_left();
            break;
        case 'd':
            camera.move_right();
            break;
    }
}

void processMotionFunc(int x, int y) {
    camera.process_mouse_motion(x,y);
}

void processMouseFunc(int button, int state, int x, int y) {
    camera.process_mouse_button(button, state, x, y);
}

void timer( int value ) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init(int argc, char** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE );
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutCreateWindow("Magrathean Universe");
    
    // callbacks
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(processKeyboardFunc);
    glutMotionFunc(processMotionFunc);
    glutMouseFunc(processMouseFunc);
    
    // init glew
    GLenum glew_status = glewInit();
    if ( glew_status != GLEW_OK ) {
        std::cout << "GLEW did not init correctly" << std::endl;
        exit(1);
    }
}

void prepare_render() {
    
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
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // enable the vertex attrib at location 0
                                
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(int argc, char** argv) {
    
    // initialize all the things
    init(argc, argv);
    
    // prepare to render everything
    prepare_render();
    
    // call the main loop
    glutMainLoop();
}

