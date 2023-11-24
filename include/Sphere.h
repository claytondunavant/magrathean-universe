//http://www.songho.ca/opengl/gl_sphere.html

#include <GL/glew.h>
#include "glm/ext/vector_float3.hpp"

#include <cmath>
#include <vector>

#include "Shader.h"

class Sphere
{
public:
    Sphere(float r, glm::vec3 position, int sector_count, int stack_count);

    void build_vertices();
    void draw(glm::mat4 view, glm::mat4 projection);
    
    std::vector<float> get_vertices();
    std::vector<unsigned int> get_indices();
    glm::vec3 get_position();
    void set_position(glm::vec3 new_position);

private:
    
    void add_vertex(float x, float y, float z);
    void add_indices(float i1, float i2, float i3);
    
    // rendering
    Shader shader;
    unsigned int VAO, VBO, EBO;
    int sector_count;
    int stack_count;

    // properties
    float r;
    glm::vec3 position;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};