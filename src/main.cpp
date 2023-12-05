// https://learnopengl.com/Getting-started/Coordinate-Systems
// https://learnopengl.com/Getting-started/Camera

#include "util.h"
#include "UniverseObjects.h"
#include "Camera.h"

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 800;

Space * Universe = new Space(0, 0, UNIVERSE_ORIGIN);

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
    
    Universe->draw(view, projection, tick.get_tick());

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
    
    // depth
    glEnable(GL_DEPTH_TEST);
    
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
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

std::string getRandomTexture() {
    // array of all possible textures
    std::vector<std::string> texturePaths = {
        "../../../assets/textures/earthmap.jpg",
        "../../../assets/textures/jupitermap.jpg",
        "../../../assets/textures/mercurymap.jpg",
        "../../../assets/textures/marsmap.jpg",
        "../../../assets/textures/neptunemap.jpg",
        "../../../assets/textures/plutomap.jpg",
        "../../../assets/textures/saturnmap.jpg",
        "../../../assets/textures/uranusmap.jpg",
        "../../../assets/textures/venusmap.jpg"
    };
    // Use a random device to seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the distribution for integers from 0 to 9
    std::uniform_int_distribution<int> distribution(0, 8);

    // Generate a random number
    int random_number = distribution(gen);
    std::cout << "random number: " << random_number << std::endl;

    return texturePaths[random_number];
}

struct string_to_space_state {
    Space * space;
    int index;
};

string_to_space_state string_to_space(std::string string, int index = 0, int depth = 0, glm::vec3 orbit_center = UNIVERSE_ORIGIN, float orbit_distance = 0) {
    
    // when you create a space, its radius always starts as 0
    Space * space = new Space(0, orbit_distance, orbit_center);
    
    char c;
    
    for ( ; index < string.size(); index++ ){
        char c = string.at(index);
        
        if ( c == 'S' ) {
            
            float sphere_radius = DEFAULT_RADIUS;
            
            // the radius of a space is tight (assuming no padding)
            // if this is the first sphere, put it in the center
            float orbit_distance;
            if ( space->get_radius() == 0 ) {
                orbit_distance = 0;

            // otherwise put it just on the outside 
            } else {
                orbit_distance = space->get_radius() + sphere_radius; 
            }
            
            std::string path_to_texture = getRandomTexture();
            Sphere * s = new Sphere(sphere_radius, orbit_distance, orbit_center, path_to_texture);
            space->add_sphere(s);
            
        } else if ( c == '(' ) {
            
            // create new space setting both the center and the distance to the current radius of the space
            glm::vec3 sub_center = orbit_center + glm::vec3(space->get_radius(), 0.0f, 0.0f);
            float orbit_distance = space->get_radius();
            string_to_space_state state = string_to_space(string, index+1, ++depth, sub_center, orbit_distance);

            Space * subspace = state.space;
            
            space->add_space(subspace);
            
            index = state.index;

        } else if ( c == ')') {
            
            if ( depth == 0 )
                continue;
            
            string_to_space_state state = {
                space,
                index
            };

            return state;

        } else {
            std::cout << "Incorrect Syntax!" << std::endl;
        }
   }

    string_to_space_state state = {
        space,
        index
    };

    return state;
}

int main(int argc, char** argv) {
    
    // TODO: S(S(SS)(S)) the 1th subspace children collide
    // TODO: S(S(S))(S) does not have the 4th subspace
    std::string universe_string = "S(S(S))";
    
    // initialize all the things
    init(argc, argv);
    
    // prepare to render everything
    dots.push_back(new Dot(glm::vec3(0.0f, 0.0f, 0.0f))); // origin
    dots.push_back(new Dot(glm::vec3(1.0f, 0.0f, 0.0f))); // left & right
    dots.push_back(new Dot(glm::vec3(0.0f, 1.0f, 0.0f))); // up 
    dots.push_back(new Dot(glm::vec3(0.0f, 0.0f, 1.0f))); // towards & away from the screen
                                                    
    for ( float i = 0.0f; i < 1.0f; i += 0.1f ) {
        dots.push_back(new Dot(glm::vec3(i, 0.0f, 0.0f))); // left & right
    }

    Universe = string_to_space(universe_string).space;
    
    Universe->print();
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // call the main loop
    glutMainLoop();
}
