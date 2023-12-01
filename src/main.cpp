// https://learnopengl.com/Getting-started/Coordinate-Systems
// https://learnopengl.com/Getting-started/Camera

#include "glm/fwd.hpp"
#include "util.h"

#include "Shader.h"
#include "Sphere.h"
#include "Camera.h"
#include <vector>

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

Space * Universe = new Space(0, 0);

std::vector<Sphere *> spheres;
std::vector<Dot *> dots;

Camera camera = Camera();
Tick tick;

void display() {
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // tranformation
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = camera.get_view_matrix();
    projection = glm::perspective(glm::radians(45.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 0.1f, 100.0f);
    
    for ( auto dot : dots ) {
        dot->draw(view, projection);
    }
    
    for ( auto sphere : spheres ) {

        sphere->draw(view, projection, tick.get_tick());
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
    tick.tick_inc();
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
    
    // debug
    glEnable( GL_DEBUG_OUTPUT );
    glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
    glDebugMessageCallback(debug_message_callback, nullptr);
}



int main(int argc, char** argv) {
    
    // initialize all the things
    init(argc, argv);
    
    // prepare to render everything
    dots.push_back(new Dot(glm::vec3(0.0f, 0.0f, 0.0f))); // origin
    dots.push_back(new Dot(glm::vec3(1.0f, 0.0f, 0.0f))); // left & right
    dots.push_back(new Dot(glm::vec3(0.0f, 1.0f, 0.0f))); // up 
    dots.push_back(new Dot(glm::vec3(0.0f, 0.0f, 1.0f))); // towards & away from the screen

    // sphere render loop
    float distance = 0;
    float distance_inc = 0.5f;
    for (int i = 0; i < 5; i++) {
        Sphere * sphere = new Sphere(0.1f, distance);
        spheres.push_back(sphere);
        distance += distance_inc;
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // call the main loop
    glutMainLoop();
}
