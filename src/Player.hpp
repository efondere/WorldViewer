#ifndef PLAYER_HPP
#define PLAYER_HPP

#define MOUSE_SENSITIVITY   0.2f
#define GRAVITY          5000.0f
#define AIR_FRICTION        0.1f
#define PLAYER_SPEED       35.0f
#define JUMP_SPEED       2000.0f

#include "Mesh.hpp"
#include "Camera.hpp"

class World;

class Player
{
public:

    Player(Camera* camera);

    void update(float dt);

    void draw();

    void jump(int key, int state);

    void onMouseMove(float xpos, float ypos);

    void setViewProjMatrix(const glm::mat4& matrix);

    void setPosition(float x, float y, float z)
    {
        m_mesh.setPosition({x, y, z});
    }

    glm::vec3 getVelocity() const
    {
        return m_velocity;
    }

    float getMaxVelocity() const
    {
        return m_maxVelocity;
    }

    glm::vec3 getSpeed() const
    {
        return glm::vec3(PLAYER_SPEED, PLAYER_SPEED, PLAYER_SPEED);
    }

    glm::vec3 getAcceleration() const
    {
        return m_acceleration;
    }

    glm::vec3 getNormalizedDirection() const
    {
        return glm::normalize(m_velocity);
    }

    void setVelocity(const glm::vec3& velocity)
    {
        m_velocity = velocity;
    }

    void setMaxVelocity(const float& maxVelocity)
    {
        m_maxVelocity = maxVelocity;
    }

    void setSpeed(const glm::vec3& speed)
    {
        // does nothing
    }

    void setAccleration(const glm::vec3& acceleration)
    {
        m_acceleration = acceleration;
    }

    void setDirection(const glm::vec3& direction)
    {
        // does nothing. Maybe for the camera?
        // https://glm.g-truc.net/0.9.9/api/a00578.html
    }

    void addVelocity(const glm::vec3& velocity)
    {
        m_velocity += velocity;
        if (glm::length(m_velocity) > m_maxVelocity)
        {
            m_velocity /= glm::length(m_velocity);
            m_velocity *= m_maxVelocity;
        }
    }

    void applyFriction(const float& friction)
    {
        m_velocity *= 1.f - friction;
    }

    void accelerate(const glm::vec3& acceleration)
    {
        m_acceleration += acceleration;
    }

    void accelerate(float x, float y, float z)
    {
        m_acceleration += glm::vec3(x, y, z);
    }

    void move(const glm::vec3& direction)
    {
        m_acceleration += direction * PLAYER_SPEED;
    }

    void setWorld(World* world)
    {
        m_world = world;
    }

private:

    Mesh      m_mesh;
    Camera*   m_camera;
    glm::vec2 m_lastMousePosition;
    World*    m_world;

    bool m_hasJumped;

    glm::vec3 m_velocity;
    float     m_maxVelocity;
    glm::vec3 m_acceleration;
    //glm::vec3 m_direction; // forward vector
};

#endif //PLAYER_HPP
