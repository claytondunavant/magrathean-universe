#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/freeglut_std.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
#include <cstdlib>
#include <chrono>

class Tick {
    public:
        void inline tick_inc() {
          tick++;
        }

        unsigned int inline get_tick() {
          return tick;
        }
        
    private:
        unsigned int tick = 0;
};

void GLAPIENTRY
debug_message_callback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );