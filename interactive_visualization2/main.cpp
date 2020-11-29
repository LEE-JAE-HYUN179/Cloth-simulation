//
//  main.cpp
//  interactive_visualization2
//
//  Created by 이재현 on 2020/11/17.
//




#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "ground.hpp"
#include "error_check.h"
#include "cloth.hpp"
#include "cube.hpp"

GLFWwindow* window;

int WINDOW_WIDTH=0;
int WINDOW_HEIGHT=0;


int init(){
    if (!glfwInit()) {
        std::cout << "glfw init failed!!\n";
        return 1;
    }
    
   
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    WINDOW_WIDTH=1000;
    WINDOW_HEIGHT=1000;
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "opengl_playground", NULL, NULL);
    
    if (!window) {
        std::cout << "window creation failed!\n";
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    
    
    glewExperimental = GL_TRUE;
    
    GLenum errorCode = glewInit();
    if (GLEW_OK != errorCode) {
        
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    
    
    glClearColor( 0.25, 0.25, 0.7, 0.0f);
    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//     Enable depth test
    glEnable(GL_DEPTH_TEST);
//     Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    
  
    
    return 0;
}






int main(int argc, char ** argv) {
    
    
    init();

    
    Shader phong("../../../interactive_visualization2/shader/Phong_VertexShader.glsl","../../../interactive_visualization2/shader/Phong_FragmentShader.glsl");
    phong.use();
    
    Camera camera(
                glm::vec3(10, 10,10), // Camera is at (4,3,3), in World Space
                glm::vec3(0, 0, 0), // and looks at the origin
                glm::vec3(0, 1, 0),
                glm::radians(45.0f),
                (4.0f / 3.0f),
                0.0001f,
                100.0f
        );

        Light light{
            glm::vec3(5,5,5),
//            glm::vec3(-1,1,-1),
            glm::vec3(0,1,1),
            glm::vec3(1.0f,1.0f,1.0f), //spec
                glm::vec3(0.0f,0.0f,0.0f), //amb
                glm::vec3(1.0f,1.0f,1.0f) //diffuse
        };
    
    phong.setLight(light);
    phong.setCamera(camera);
    

    
    
    Ground ground;
    Cloth cloth;
    Cube cube;
    

    
  
    while (!glfwWindowShouldClose(window)) {
        
        
        
        glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
        
        cloth.update(1.f/600);
       
        ground.render(phong, light, camera);
        
        cloth.render(phong, light, camera);

        
        cube.render(phong,light,camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
    
    
    return 0;
}
