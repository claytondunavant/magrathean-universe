#include <GL/glew.h>
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cstddef>
#include <iostream>

const char *v_shader_src = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *f_shader_src = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

unsigned int VAO, VBO, shader_program;

void display() {
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram(shader_program);
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
    
    // TODO: load from files, look at assignment03::util.cpp::compileShader
    // build and compile shaders
    GLint success;
    char infoLog[512];

    unsigned int v_shader;
    v_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v_shader, 1, &v_shader_src, nullptr);
    glCompileShader(v_shader);
    glGetShaderiv(v_shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(v_shader, 512, NULL, infoLog);
        std::cout << "Vertex Shader Compilation Failed\n" << infoLog << std::endl;
        glDeleteShader(v_shader);
        return 1;
    }

    unsigned int f_shader;
    f_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f_shader, 1, &f_shader_src, nullptr);
    glCompileShader(f_shader);
    glGetShaderiv(f_shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(f_shader, 512, NULL, infoLog);
        std::cout << "Fragment Shader Compilation Failed\n" << infoLog << std::endl;
        glDeleteShader(f_shader);
        return 1;
    }

    // create shader program
    // TODO: put shaders in array like look at assignment03::util.cpp::linkProgram
    shader_program = glCreateProgram();
    glAttachShader(shader_program, v_shader);
    glAttachShader(shader_program, f_shader);
    glLinkProgram(shader_program);
    
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        // Handle OpenGL error
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
    
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "Shader Program Linking Failed\n" << infoLog << std::endl;
        glDeleteProgram(shader_program);
        return 1;
    }

    glDetachShader(shader_program, v_shader); 
    glDetachShader(shader_program, f_shader);
    glDeleteShader(v_shader);
    glDeleteShader(f_shader);
    
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

