#include "Player.hpp"
#include "InputManager.hpp"
#include "World.hpp"

Player::Player(Camera* camera)
: m_mesh("greyShader", "colorShader", "./res/textures/playerTexture.png")
, m_camera(camera)
, m_lastMousePosition(0, 0)
, m_world(nullptr)
, m_velocity(0)
, m_maxVelocity(20000.f)
, m_acceleration(0)
{
    double mouseX, mouseY;
    glfwGetCursorPos(camera->getWindow(), &mouseX, &mouseY);
    m_lastMousePosition.x = static_cast<float>(mouseX);
    m_lastMousePosition.y = static_cast<float>(mouseY);

    InputManager::addKeyBinding(GLFW_KEY_ESCAPE, std::bind(&Camera::toggleMouseGrab, m_camera, std::placeholders::_1, std::placeholders::_2));
    InputManager::addKeyBinding(GLFW_KEY_SPACE, std::bind(&Player::jump, this, std::placeholders::_1, std::placeholders::_2));
    InputManager::addMouseMoveCallback(std::bind(&Player::onMouseMove, this, std::placeholders::_1, std::placeholders::_2));
}

void Player::update(float elapsedTime)
{
    m_acceleration.y -= GRAVITY * elapsedTime;

    if (glfwGetKey(m_camera->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_acceleration += m_camera->Front * PLAYER_SPEED;
    if (glfwGetKey(m_camera->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_acceleration += m_camera->Right * -PLAYER_SPEED;
    if (glfwGetKey(m_camera->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_acceleration += m_camera->Front * -PLAYER_SPEED;
    if (glfwGetKey(m_camera->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_acceleration += m_camera->Right * PLAYER_SPEED;

    float friction = PLAYER_SPEED * AIR_FRICTION;

    addVelocity(m_acceleration * glm::clamp(elapsedTime, 0.f, 0.5f));
    setAccleration(glm::vec3(0.f, 0.f, 0.f));
    applyFriction(friction * elapsedTime);

    m_mesh.move(m_velocity);

    float floorHeight = m_world->getHeightAt(m_mesh.getPosition().x, m_mesh.getPosition().z) + 10.f;
    if (m_mesh.getPosition().y < floorHeight && m_velocity.y < 0.f)
    {
        // when I move the window, I get tp really high up in the sky. I should find where this might come from
        m_velocity.y = 0.f;
        //m_acceleration.y += JUMP_SPEED;
        m_hasJumped = false;
        m_mesh.setPosition({m_mesh.getPosition().x, floorHeight, m_mesh.getPosition().z});
        //m_mesh.move({0.0, -m_velocity.y, 0.0});
    }

    m_camera->setPosition(m_mesh.getPosition() + glm::vec3(0.0, 10.0, 0.0));
    m_mesh.setViewPosition(m_camera->Position);
}

void Player::draw()
{
    //m_mesh.draw();
}

void Player::jump(int key, int state)
{
    if (key == GLFW_KEY_SPACE && state == GLFW_PRESS)
    {
        if (!m_hasJumped)
        {
            m_acceleration.y += JUMP_SPEED;
            m_hasJumped = true;
        }
    }
}

void Player::onMouseMove(float xpos, float ypos)
{
    float mouseDeltaX = xpos - m_lastMousePosition.x;
    float mouseDeltaY = ypos - m_lastMousePosition.y;
    m_lastMousePosition.x = xpos;
    m_lastMousePosition.y = ypos;

    if (!m_camera->isMouseGrabbed())
        return;
    
    m_camera->rotateHoriz(mouseDeltaX * MOUSE_SENSITIVITY);
    m_camera->rotateVert(-mouseDeltaY * MOUSE_SENSITIVITY);
}

void Player::setViewProjMatrix(const glm::mat4& matrix)
{
    m_mesh.setViewProjMatrix(matrix);
}
