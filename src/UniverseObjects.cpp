//http://www.songho.ca/opengl/gl_sphere.html

#include <vector>

#include "UniverseObjects.h"

UniverseObject::UniverseObject(float radius, float orbit_distance, glm::vec3 orbit_center){
    m_radius = radius;
    m_orbit_distance = orbit_distance;
    m_orbit_center = orbit_center;
    m_rotation_offset = generate_rotation_offset();
}

// it is increasing based on the tick
// but shouldnt that not be the case becaues tick is only used in rotation which loops?
// is this being called faster as time goes on?

// apply to points already where they should be relative to origin? (makes sense in regards to sphere)
glm::mat4 UniverseObject::rotate_around_orbit_center_matrix(unsigned int tick) {

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float current_angle = (tick * TICK_ROTATION_FACTOR) + m_rotation_offset;

    // orbit center corrds matrix
    glm::mat4 center_matrix = glm::translate(glm::mat4(1.0f), m_orbit_center);
    // rotate current angle around the up axis
    // why the heck is angle a rolling sum here and not when rotating center_points
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), current_angle, up);
    // translate out the z by orbit distance    
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_orbit_distance, 0.0f, 0.0f));
    
    return center_matrix * rotation_matrix * translation_matrix;
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

float UniverseObject::get_radius() {
    return m_radius;
}

float UniverseObject::get_orbit_distance() {
    return m_orbit_distance;
}

glm::vec3 UniverseObject::get_orbit_center() {
    return m_orbit_center; 
}

void UniverseObject::set_orbit_center(glm::vec3 orbit_center) {
    m_orbit_center = orbit_center; 
}


RenderSphere::RenderSphere(float radius, std::string vertex_shader_path, std::string fragment_shader_path) {
    m_render_radius = radius; 

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

    float sector_step = 2 * PI / SECTOR_COUNT;
    float stack_step = PI / STACK_COUNT;
    float sector_angle, stack_angle;

    // iterate through stack (vertical panes on sphere)
    for (int i = 0; i <= STACK_COUNT; ++i) {
        
        stack_angle = PI / 2 - i * stack_step; // start from pi/2 to -pi/2
        xy = m_render_radius * cosf(stack_angle); // r * cos(u)
        z = m_render_radius * sinf(stack_angle); // r * sin(u)
                                
        // iterate through sectors (horizontal panes on sphere) in this level of the stack
        for (int j = 0; j <= SECTOR_COUNT; ++j) {
            sector_angle = j * sector_step; // start from 0 to 2pi
                                        
            // vertex position
            x = xy * cosf(sector_angle); // r * cos(u) * cos(v)
            y = xy * sinf(sector_angle); // r * cos(u) * sin(v)

            add_vertex(x, y, z);
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

void RenderSphere::add_vertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
}

void RenderSphere::add_indices(float i1, float i2, float i3) {
    indices.push_back(i1);
    indices.push_back(i2);    
    indices.push_back(i3);    
}

// this is insane and cool!
Sphere::Sphere(float radius, float orbit_distance, glm::vec3 orbit_center) : 
UniverseObject(radius, orbit_distance, orbit_center),
RenderSphere(radius, "../../../shaders/v.glsl", "../../../shaders/f.glsl")
{}

universe_object_type Sphere::get_type() {
    return sphere_type;
}


void Sphere::draw(glm::mat4 view, glm::mat4 projection, unsigned int tick) {

    shader.use(); 
    
    glm::mat4 model = rotate_around_orbit_center_matrix(tick);

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

void Sphere::print() {
    std::cout << "m_radius: " << m_radius << std::endl;
    std::cout << "m_orbit_distance: " << m_orbit_distance << std::endl;
    std::cout << "m_orbit_center: " << m_orbit_center.x << "," << m_orbit_center.y << "," << m_orbit_center.z << std::endl;
    std::cout << "m_rotation_offset: " << m_rotation_offset << std::endl;
}

Space::Space(float radius, float orbit_distance, glm::vec3 orbit_center) :
UniverseObject(radius, orbit_distance, orbit_center)
{}

universe_object_type Space::get_type() {
    return space_type;
}

void Sphere::set_orbit_center(glm::vec3 orbit_center) {
    m_orbit_center = orbit_center; 
}

// add a sphere to the space, update the radius and center point
void Space::add_sphere(Sphere * sphere) {
    
    // add sphere to orbits
    m_orbits.push_back(sphere);
    
    // if this is the center, the radius only goes up by the radius of the object
    float added_radius;
    if ( m_orbits.size() == 1 ) {
        added_radius = sphere->get_radius() + ORBIT_PADDING;
    // otherwise the radius of the space is increased by 2(radius + padding)
    } else {
        added_radius = 2 * (sphere->get_radius() + ORBIT_PADDING);
    }
    
    // increase the radius
    m_radius += added_radius;
    
    // shift the orbit center if this is an oribiting space
    if ( m_orbit_center != UNIVERSE_ORIGIN ) {
        shift_orbit_center_right(added_radius);
    }
}

void Space::add_space(Space * space) {
    m_orbits.push_back(space);
    
    // if this is the center, the radius only goes up by the radius of the object
    float added_radius;
    if ( m_orbits.size() == 1 ) {
        added_radius = space->get_radius() + ORBIT_PADDING;
    // otherwise the radius of the space is increased by 2(radius + padding)
    } else {
        added_radius = 2 * (space->get_radius() + ORBIT_PADDING);
    }
    
    m_radius += added_radius;
    
    if ( m_orbit_center != UNIVERSE_ORIGIN) {
        shift_orbit_center_right(added_radius);
    }
}

std::vector<UniverseObject *> Space::get_orbits(){
    return m_orbits;
}

// shift a space's orbit center to the right by a distance and update dependent objects
void Space::shift_orbit_center_right(float distance) {

    glm::vec3 new_orbit_center = m_orbit_center + glm::vec3(distance, 0.0f, 0.0f);

    m_orbit_center = new_orbit_center; 
    
    for (auto obj : m_orbits) {
        
        // set all the spheres in this space to the new orbit center
        if ( obj->get_type() == sphere_type ) {

            Sphere * sphere = static_cast<Sphere *>(obj);
            sphere->set_orbit_center(new_orbit_center);
            
        // because the orbit center of a space is what its objects rotate around
        // we have to shift it over by the distance in its parent space
        } else if ( obj->get_type() == space_type ) {
            
            Space * space = static_cast<Space *>(obj);
            space->shift_orbit_center_right(distance + space->get_orbit_distance());
        }
    }
}

void Space::set_orbit_center(glm::vec3 orbit_center) {
    m_orbit_center = orbit_center; 

    for ( auto obj : m_orbits ) {
        if ( obj->get_type() == sphere_type ) {

            Sphere * sphere = static_cast<Sphere *>(obj);
            sphere->set_orbit_center(orbit_center);
        }
    }
}

// populate cache of vectors from the orbit_center to the sub orb_center
void Space::populate_orbit_vectors_cache() {
    
    for ( auto obj : m_orbits ) {
        if ( obj->get_type() == space_type ) {
            Space * subspace = static_cast<Space *>(obj);
            
            // push back
            glm::vec3 orbit_center_vector = subspace->get_orbit_center() - get_orbit_center();
            m_orbit_vectors_cache.push_back(orbit_center_vector);
                
            // recursively fill subspace caches
            subspace->populate_orbit_vectors_cache();
        }
    }
    
}

// rotate the orbit centers using the cached vectors
void Space::rotate_orbit_centers(unsigned int tick) {
    
    // rotate the center points of subspaces around parent orbit_center
    for ( auto obj : get_orbits() ) {
        
        if ( obj->get_type() == space_type ) {
            Space * space = static_cast<Space *>(obj);
            
            // rotate current angle around the up axis
            // why the heck is angle a constant angle here and not when rotating spheres
            float angular_velocity = ORBIT_CENTER_ANGULAR_VELOCITY + (ORBIT_CENTER_ROTATION_OFFSET_FACTOR * m_rotation_offset);
            glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), angular_velocity, UP);
            
            // get the cached vector (maintains the length and angle)
            // delete it from the front
            glm::vec3 cached_orbit_center_vector = m_orbit_vectors_cache.front();
            m_orbit_vectors_cache.erase(m_orbit_vectors_cache.begin());
            
            // vector from the parent orbit center to the child orbit center
            glm::vec4 parent_oc_to_child_oc_vec = glm::vec4(cached_orbit_center_vector, 1.0f);
            glm::vec4 temp = rotation_matrix * parent_oc_to_child_oc_vec;

            // add rotated vector back to orbit center 
            glm::vec3 new_sub_orbit_center = get_orbit_center() + glm::vec3(temp.x, temp.y, temp.z);
            
            // update spheres
            space->set_orbit_center(new_sub_orbit_center);

            // rotate the orbit centers of the subspace
            space->rotate_orbit_centers(tick);
        }
    }
    
}

void Space::draw(glm::mat4 view, glm::mat4 projection, unsigned int tick) {
    
    // only rotate the orbits once per root draw call
    if ( m_orbit_center == UNIVERSE_ORIGIN) {
        
        // fill the cache
        populate_orbit_vectors_cache();
        
        // fill the orbit center vector cache of all the spaces
        rotate_orbit_centers(tick);
    }
    
    
    // draw 
    for ( auto obj : get_orbits() ) {
        
        // cheating polymorphism, one enum at a time
        if ( obj->get_type() == sphere_type ) {
            Sphere * sphere = static_cast<Sphere *>(obj);
            sphere->draw(view, projection, tick); 

        } else if ( obj->get_type() == space_type ) {
            Space * space = static_cast<Space *>(obj);
            space->draw(view, projection, tick); 
        }
    }
    
}

void Space::print() {
    
    std::cout << "Space: " << std::endl;

    std::cout << "m_radius: " << m_radius << std::endl;
    std::cout << "m_orbit_distance: " << m_orbit_distance << std::endl;
    std::cout << "m_orbit_center: " << m_orbit_center.x << "," << m_orbit_center.y << "," << m_orbit_center.z << std::endl;
    std::cout << "m_rotation_offset: " << m_rotation_offset << std::endl;

    int sphere_count = 0;
    int space_count = 0; 

    for ( auto obj : get_orbits() ) {
        
        // cheating polymorphism, one enum at a time
        if ( obj->get_type() == sphere_type ) {

            Sphere * sphere = static_cast<Sphere *>(obj);
            std::cout << "Sphere " << sphere_count << std::endl;
            sphere->print();
            sphere_count++;

        } else if ( obj->get_type() == space_type ) {

            Space * space = static_cast<Space *>(obj);
            std::cout << "Sub-space " << space_count << std::endl;
            space->print();
            space_count++;
        }
    }
    
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