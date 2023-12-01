// https://learnopengl.com/Getting-started/Camera

#include "GL/freeglut_std.h"

#include "glm/detail/qualifier.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

#include <cmath>
#include <iostream>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.2f;
const float MOUSE_SENSITIVITY = 0.1f;

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
        
    float yaw;
    float pitch;
    float speed;
    float mouse_sensitivity;

    Camera() {
        position = glm::vec3(0.0f, 0.0f, 3.0f);
        world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        yaw = YAW;
        pitch = PITCH;
        speed = SPEED;
        mouse_sensitivity = MOUSE_SENSITIVITY;

        update_cam_vectors();
    } 
    
    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    }
    
    void move_forward() {
        position += front * speed;   
    }

    void move_backward() {
        position -= front * speed;   
    }
    
    void move_right() {
        position += right * speed;
    }
    
    void move_left() {
        position -= right * speed;
    }
    
    void process_mouse_motion(int x, int y) {
        
        float x_off = x - last_x;
        float y_off = last_y - y;
        
        last_x = x;
        last_y = y;
        
        x_off *= mouse_sensitivity;
        y_off *= mouse_sensitivity;
        
        yaw += x_off;
        pitch += y_off;

        if ( pitch > 89.0f ) {
            pitch = 89.0f;
        }
        
        if ( pitch < -90.0f ) {
            pitch = -89.0f;
        }

        update_cam_vectors();
    }
    
    void process_mouse_button(int button, int state, int x, int y) {
        if ( button == GLUT_LEFT_BUTTON ) {
            if ( state == GLUT_DOWN ) {
                last_x = x;
                last_y = y;
            }
        }
    }

private:
    
    float last_x;
    float last_y;
    bool first_mouse = true;
    
    void update_cam_vectors() {
        
        // create new front vector
        glm::vec3 new_front;
        new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        new_front.y = sin(glm::radians(pitch));
        new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(new_front);
       
        // re-caculate right and up vectors
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
    }
};