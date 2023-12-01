//http://www.songho.ca/opengl/gl_sphere.html

#include "UniverseObjects.h"
#include "glm/detail/qualifier.hpp"
#include <vector>
#include "stb_image.h"

UniverseObject::UniverseObject(float radius, float orbit_distance){
    m_radius = radius;
    m_orbit_distance = orbit_distance;
    m_rotation_offset = generate_rotation_offset();
}

int UniverseObject::generate_rotation_offset() {
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dis(0, MAX_ROTATION_OFFSET);
    return dis(gen);
}

universe_object_type UniverseObject::get_type() {
    return uninit_type;
}

RenderSphere::RenderSphere(float radius, std::string vertex_shader_path, std::string fragment_shader_path) {
    m_radius = radius; 

    // build vertices and indices
    build_vertices();
    
    // TODO: make not relative paths ??
    // init shader
    shader = Shader(vertex_shader_path.c_str(), fragment_shader_path.c_str());

    // init rendering
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // bind Vertex Array Object
    glBindVertexArray(VAO); 

    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // enable the vertex attrib at location 0
}

// TODO: fix up vector
void RenderSphere::build_vertices()
{
    const float PI = acos(-1.f);

    float x, y, z, xy;      // vertex position
    float u, v;             // texture coordinates

    float sector_step = 2 * PI / SECTOR_COUNT;
    float stack_step = PI / STACK_COUNT;
    float sector_angle, stack_angle;

    // iterate through stack (vertical panes on sphere)
    for (int i = 0; i <= STACK_COUNT; ++i) {
        
        stack_angle = PI / 2 - i * stack_step; // start from pi/2 to -pi/2
        xy = m_radius * cosf(stack_angle); // r * cos(u)
        z = m_radius * sinf(stack_angle); // r * sin(u)
                                
        // iterate through sectors (horizontal panes on sphere) in this level of the stack
        for (int j = 0; j <= SECTOR_COUNT; ++j) {
            sector_angle = j * sector_step; // start from 0 to 2pi
                                        
            // vertex position
            x = xy * cosf(sector_angle); // r * cos(u) * cos(v)
            y = xy * sinf(sector_angle); // r * cos(u) * sin(v)

            // texture coordinates
            u = static_cast<float>(j) / SECTOR_COUNT;
            v = static_cast<float>(i) / STACK_COUNT;

            add_vertex(x, y, z, u, v);
        }
    }
    
    // indices
    // k1 -- k1 + 1
    // |    / |
    // |   /  |
    // | /    |
    // k2 -- k2 + 1
    unsigned int k1, k2;
    for (int i = 0; i < STACK_COUNT; ++i) {
        k1 = i * (SECTOR_COUNT + 1); // begining of current stack
        k2 = k1 + (SECTOR_COUNT + 1); // beginning of next stack

        for (int j = 0; j < SECTOR_COUNT; ++j, ++k1, ++k2) {

            // 2 triangles per sector excluding 1st and last stacks
            if ( i != 0 ) // add top triangle
            {
                add_indices(k1, k2, k1+1); 
            }

            if ( i != (STACK_COUNT - 1 )) // add bottom triangle
            {
                add_indices(k1+1, k2, k2+1);
            }
        }
    }
}

std::vector<float> RenderSphere::get_vertices() {
   return vertices; 
}

std::vector<unsigned int> RenderSphere::get_indices() {
   return indices; 
}

void RenderSphere::add_vertex(float x, float y, float z, float u, float v) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);

    texture_coordinates.push_back(u);
    texture_coordinates.push_back(v);
}

void RenderSphere::add_indices(float i1, float i2, float i3) {
    indices.push_back(i1);
    indices.push_back(i2);    
    indices.push_back(i3);    
}

void RenderSphere::compute_texture_coordinates() {
    // use spherical mapping
}

// this is insane and cool!
Sphere::Sphere(float radius, float orbit_distance) : 
UniverseObject(radius, orbit_distance),
RenderSphere(radius, "../../../shaders/v.glsl", "../../../shaders/f.glsl")
{}

universe_object_type Sphere::get_type() {
    return sphere_type;
}

void Sphere::draw(glm::mat4 view, glm::mat4 projection, unsigned int tick) {

    shader.use(); 

    glm::mat4 model = glm::mat4(1.0f);
    // rotate around up
    model *= glm::rotate(model, (tick * TICK_ROTATION_FACTOR) + m_rotation_offset, glm::vec3(0.0f, 1.0f, 0.0f));
    // TODO: translate relative to orbit axis
    model *= glm::translate(glm::mat4(1.0f), glm::vec3(m_orbit_distance, 0.0f, 0.0f));

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
   
    // texturing
    //GLuint textureID = loadTexture("path/to/your/texture.jpg");

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}

Space::Space(float radius, float orbit_distance) :
UniverseObject(radius, orbit_distance)
{}

universe_object_type Space::get_type() {
    return space_type;
}

void Space::add_sphere(Sphere * sphere) {
    m_orbits.push_back(sphere);
}

std::vector<UniverseObject *> Space::get_orbits(){
    return m_orbits;
}

Dot::Dot(glm::vec3 postion) :
    // fix issue with relative paths
RenderSphere(DOT_RADIUS, "../../../shaders/v.glsl", "../../../shaders/f_dot.glsl")
{
    m_position = postion;
}

void Dot::draw(glm::mat4 view, glm::mat4 projection) {
    
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    // TODO: translate relative to orbit axis
    model *= glm::translate(glm::mat4(1.0f), m_position);

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
   
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
}