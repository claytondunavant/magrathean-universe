//http://www.songho.ca/opengl/gl_sphere.html

#include <GL/glew.h>
#include "glm/detail/qualifier.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <cmath>
#include <vector>
#include <cstdlib>
#include <math.h>
#include <random>

#include "Shader.h"

#define SECTOR_COUNT 36
#define STACK_COUNT 18
#define DEFAULT_RADIUS 0.05f
#define TICK_ROTATION_FACTOR 0.025f // determines speed of roation of sphere around orbit axis
#define MAX_ROTATION_OFFSET 100.0f // max offset of rotation. set to 0 to have all orbitals in a line
#define DOT_RADIUS 0.01f // default radius of dot used for debugging
                
extern Shader sphere_shader;
extern Shader dot_shader;

// An object in the universe
class UniverseObject 
{
public: 
    UniverseObject(float radius, float orbit_distance, glm::vec3 orbit_axis);

    int generate_rotation_offset();

    glm::vec3 get_orbit_axis();

protected:
    float m_radius;
    float m_orbit_distance;
    glm::vec3 m_orbit_axis;
    int m_rotation_offset;
};

// Everything needed to render a sphere except the draw function
class RenderSphere 
{
public:
    RenderSphere(float radius, std::string vertex_shader_path, std::string fragment_shader_path);
    
    void build_vertices();
    std::vector<float> get_vertices();
    std::vector<unsigned int> get_indices();

protected:
    void add_vertex(float x, float y, float z);
    void add_indices(float i1, float i2, float i3);
    
    float m_radius;
    
    // rendering
    Shader shader;
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

// Space consisting of orbiting spheres and sub-spaces
class Space : UniverseObject {

private:
    std::vector<UniverseObject *> orbits;
};

// Sphere in an orbit
class Sphere : UniverseObject, RenderSphere
{
public:
    Sphere(float radius, float orbit_distance, glm::vec3 orbit_axis);

    void draw(glm::mat4 view, glm::mat4 projection, unsigned int tick);
};

// Be able to draw dots (small spheres) for debugging
class Dot : RenderSphere
{
public:
    Dot(glm::vec3 position);

    void draw(glm::mat4 view, glm::mat4 projection);

private:
    glm::vec3 m_position; 
};