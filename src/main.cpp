#include <GL/freeglut_std.h>
#include <GL/gl.h>

void display() {
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // Set color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw a pixel at the center of the window
    glBegin(GL_QUADS);
    glVertex2f(-.5f, -.5f); // Bottom-left vertex
    glVertex2f(.5f, -.5f); // Bottom-left vertex
    glVertex2f(.5f, .5f); // Bottom-left vertex
    glVertex2f(-.5f, .5f); // Bottom-left vertex
    glEnd();

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
    
    glutMainLoop();
}

