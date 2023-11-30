//http://www.songho.ca/opengl/gl_sphere.html

#include <GL/glew.h>
#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float3.hpp"

#include <cmath>
#include <vector>

#include "Shader.h"

#define SECTOR_COUNT 36
#define STACK_COUNT 18
#define DEFAULT_RADIUS 0.05f
#define TICK_ROTATION_FACTOR 0.025f // determines speed of roation of sphere around orbit axis

class UniverseObject 
{
    
    void set_radius(float radius);

protected:
    float m_radius;
    float m_orbit_distance;
    glm::vec3 m_orbit_axis;
};

// space where spheres and 
class Space : UniverseObject {

private:
    std::vector<UniverseObject *> orbits;
};

class Sphere : UniverseObject
{
public:
    Sphere(float radius, float orbit_distance, glm::vec3 orbit_axis);

    void build_vertices();
    void draw(glm::mat4 view, glm::mat4 projection, unsigned int tick);
    
    std::vector<float> get_vertices();
    std::vector<unsigned int> get_indices();
    glm::vec3 get_orbit_axis();

private:
    
    void add_vertex(float x, float y, float z);
    void add_indices(float i1, float i2, float i3);
    
    // rendering
    Shader shader;
    unsigned int VAO, VBO, EBO;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};