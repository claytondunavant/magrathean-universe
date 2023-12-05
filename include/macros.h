#include "glm/ext/vector_float3.hpp"

#define SECTOR_COUNT 36                             // number of sectors in a sphere
#define STACK_COUNT 18                              // number of stacks in a sphere
#define DEFAULT_RADIUS 0.05f                        // default radius of a sphere
#define TICK_ROTATION_FACTOR .0f                  // determines speed of roation of sphere around orbit axis
#define MAX_ROTATION_OFFSET 100.0f                  // max offset of rotation. set to 0 to have all orbitals in a line
#define DOT_RADIUS 0.01f                            // default radius of dot used for debugging
#define ORBIT_PADDING 0.01f                         // padding distance on both sides of a universe object
#define ORBIT_CENTER_ANGULAR_VELOCITY 0.01f         // radians per frame that an orbit centers rotat
#define ORBIT_CENTER_ROTATION_OFFSET_FACTOR 0.0001f // factor to multiply m_orbit_offset by when adding it to ORBIT_CENTER_ANGULAR_VELOCITY

#define UNIVERSE_ORIGIN glm::vec3(0.0f, 0.0f, 0.0f) // the origin of the universe (not zeros because floating point rouding errors (for some reason))
#define UP glm::vec3(0.0f, 1.0f, 0.0f)              // world up
