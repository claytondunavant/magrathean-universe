//http://www.songho.ca/opengl/gl_sphere.html

#include <GL/glew.h>
#include <cmath>
#include <vector>

class Sphere
{
public:
    Sphere(float r, int sector_count, int stack_count);

    void build_vertices();
    void draw();
    
    std::vector<float> get_vertices();
    std::vector<unsigned int> get_indices();

private:
    
    void add_vertex(float x, float y, float z);
    void add_indices(float i1, float i2, float i3);
    
    float r;
    int sector_count;
    int stack_count;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};