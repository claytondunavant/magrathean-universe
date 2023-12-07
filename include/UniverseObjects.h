//http://www.songho.ca/opengl/gl_sphere.html

#ifndef UNIVERSE_OBJECTS_H
#define UNIVERSE_OBJECTS_H

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

#include "macros.h"
#include "Shader.h"

enum universe_object_type {
    sphere_type,
    space_type,
    uninit_type,
};

extern float sphere_movement_toggle;
extern float sub_space_movement_toggle;
extern float shadow_toggle;
                         
// An object in the universe
class UniverseObject 
{
public: 
    UniverseObject(float radius, float orbit_distance, glm::vec3 orbit_center);

    glm::mat4 rotate_around_orbit_center_matrix(unsigned int tick);
    int generate_rotation_offset();
    
    virtual universe_object_type get_type();
    float get_radius();
    float get_orbit_distance(); 
    glm::vec3 get_orbit_center();

    virtual void set_orbit_center(glm::vec3 orbit_center);

protected:
    float m_radius;
    float m_orbit_distance;
    glm::vec3 m_orbit_center;
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
    void add_vertex(float x, float y, float z, float u, float v, glm::vec3 normal, glm::vec3 tangent);
    void add_indices(float i1, float i2, float i3);
    
    float m_render_radius;
    
    // rendering
    Shader shader;
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // texturing
    std::vector<float> texture_coordinates;
    std::vector<float> normals;
    std::vector<float> tangents;
    std::vector<float> interleavedData;
    
};

// Sphere in an orbit
class Sphere : public UniverseObject, public RenderSphere
{
public:
    Sphere(float radius, float orbit_distance, glm::vec3 orbit_center, std::string path_to_texture, std::string path_to_normal_map, bool is_illuminated);
    
    GLuint load_texture(const char* fileName, bool isBump);
    void print();
    
    universe_object_type get_type() override;
    void set_orbit_center(glm::vec3 orbit_center) override;
    void set_sun_location(glm::vec3 sun);

    void draw(glm::mat4 view, glm::mat4 projection, unsigned int tick, glm::vec3 sun_location);

private:
    GLuint texture_id;
    GLuint normal_texture_id;
    bool m_is_illuminated;
    glm::vec3 sun_location;
};

// Space consisting of orbiting spheres and sub-spaces
class Space : public UniverseObject {
public:
    Space(  float radius,               // radius of Space
            float orbit_distance,       // distance of Space from Orbit Center
            glm::vec3 orbit_center      // point Space orbits
    );

   void draw(glm::mat4 view, glm::mat4 projection, unsigned int tick);
   void populate_orbit_vectors_cache();
   void print();

   void shift_orbit_center_right(float distance);
   void rotate_orbit_centers();
   void add_sphere(Sphere * sphere);
   void add_space(Space * space);
   void set_sun_location(glm::vec3 sun_location);
   glm::vec3 get_sun_location();

   universe_object_type get_type() override;
   std::vector<UniverseObject *> get_orbits();

   void set_orbit_center(glm::vec3 orbit_center) override;

   std::vector<glm::vec3> m_orbit_vectors_cache; // cache of vector from m_orbit_center to sub space orbit_centers
    
private:
    std::vector<UniverseObject *> m_orbits;
    glm::vec3 sun_location;
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

#endif