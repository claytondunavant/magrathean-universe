//http://www.songho.ca/opengl/gl_sphere.html

#define STB_IMAGE_IMPLEMENTATION

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

    // interleave data and buffer it
    std::vector<float> interleavedData;
    for (size_t i = 0; i < vertices.size(); i += 3) {
        interleavedData.push_back(vertices[i]);
        interleavedData.push_back(vertices[i + 1]);
        interleavedData.push_back(vertices[i + 2]);
        interleavedData.push_back(texture_coordinates[i / 3]);
        interleavedData.push_back(texture_coordinates[i / 3 + 1]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(interleavedData[0]), interleavedData.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
    
    // set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
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


// this is insane and cool!
Sphere::Sphere(float radius, float orbit_distance, std::string path_to_texture) : 
UniverseObject(radius, orbit_distance),
RenderSphere(radius, "../../../shaders/v.glsl", "../../../shaders/f.glsl")
{
    Sphere::path_to_texture = path_to_texture;
    texture_id = load_texture(path_to_texture.c_str());
}

universe_object_type Sphere::get_type() {
    return sphere_type;
}

GLuint Sphere::load_texture(const char* fileName) {
    GLuint texture_id;
    glGenTextures(1, &texture_id);

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true); // Flip vertically because OpenGL textures are flipped
    unsigned char* data = stbi_load(fileName, &width, &height, &channels, 0);

    if (data)
    {
        GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture parameters (optional)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Failed to load texture at path: " << fileName << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &texture_id);
        texture_id = 0;
    }

    return texture_id;

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
    
    // Use the texture in your shader
    shader.setInt("textureSampler", 0); // Set the texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0 );
    glBindTexture(GL_TEXTURE_2D, 0);

    //glDeleteTextures(1, &texture_id);

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