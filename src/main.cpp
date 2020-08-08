#include <glad/gl.h>
#include "World.hpp"
#include "InputManager.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "Skybox.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <bits/c++config.h>

const GLuint WIDTH = 1600, HEIGHT = 900;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    InputManager::onKeyPress(key, action);
}

void mouse_move_callback(GLFWwindow* window, double xpos, double ypos)
{
    InputManager::onMouseMove(static_cast<float>(xpos), static_cast<float>(ypos));
}

void openGLDebugCallback(GLenum source​, GLenum type, GLuint id​, GLenum severity​, GLsizei length​, const GLchar* message, const void* userParam​)
{
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "[GL ERROR] " : " ") << "type = " << type << " severity = " << severity​ << " message: " << message << std::endl;
}

int main(int argc, char* argv[])
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "World Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetKeyCallback(window, key_callback);

    {

    int version = gladLoadGL(glfwGetProcAddress);
    if (!version)
        std::cout << "Error: Could not load glad" << std::endl;

    std::cout << "GL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(openGLDebugCallback, 0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_FRAMEBUFFER_SRGB); // gamma correction

    Camera cam(window);

    Player player(&cam);
    player.setPosition(0, 300, 0);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.f), (float)WIDTH / (float)HEIGHT, 0.1f, 10000.f);
    //glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 400.0f, -300.0f),
  	//	                               glm::vec3(0.0f, 0.0f,  0.0f),
  	//	                               glm::vec3(0.0f, 1.0f,  0.0f));

    World world;
    world.loadFromFile("./res/worlds/newWorld.world");

    Skybox sky("aurora", &cam, projectionMatrix, true);

    player.setWorld(&world);

    double prevTime = glfwGetTime();
    double newTime = 0.f;
    float elapsedTime = 0.f;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        newTime = glfwGetTime();
        elapsedTime =  static_cast<float>(newTime - prevTime);
        cam.deltaTime = elapsedTime;
        prevTime = newTime;

        glm::mat4 viewProjMatrix = projectionMatrix * cam.getViewMatrix();
        world.setViewProjMatrix(viewProjMatrix);
        player.setViewProjMatrix(viewProjMatrix);
        world.setViewPosition(cam.Position);

        glClearColor(0.f, 0.f, 0.f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world.update(elapsedTime);
        player.update(elapsedTime);
        //sky.draw(); // I should draw it last, but it doesn't work with transparency otherwise
        //world.draw(&cam, Mesh::RenderMode::ShadedFaces);
        //player.draw();
        world.draw(&cam, World::RenderMode::Wireframe);

        glfwSwapBuffers(window);
    }

    }

    glfwTerminate();

    return 0;
}