#ifndef TRANSFORMABLE_HPP
#define TRANSFORMABLE_HPP

#include "Transform.hpp"
#include <glm/gtx/quaternion.hpp>

class Transformable
{
public:

    Transformable();

    virtual ~Transformable();

    void setPosition(float x, float y, float z);

    void setPosition(const glm::vec3& position);

    void setRotation(float x, float y, float z);

    void setRotation(const glm::vec3& rotation);

    void setScale(float x, float y, float z);

    void setScale(const glm::vec3& scale);

    void setOrigin(float x, float y, float z);

    void setOrigin(const glm::vec3& origin);

    const glm::vec3& getPosition() const;

    const glm::vec3& getRotation() const;

    const glm::vec3& getScale() const;

    const glm::vec3& getOrigin() const;

    void move(float offsetX, float offsetY, float offsetZ);

    void move(const glm::vec3& offset);

    void rotate(float angleX, float angleY, float angleZ);

    void rotate(const glm::vec3& angles);

    void scale(float factorX, float factorY, float factorZ);

    void scale(const glm::vec3& factor);

    const Transform& getTransform() const;

    const Transform& getInverseTransform() const;

    const glm::vec3& getForwardVector() const;
    
    const glm::vec3& getUpVector() const;

    const glm::vec3& getRightVector() const;

private:

    void updateDirectionalVectors() const;

    glm::vec3 m_origin;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;

    mutable glm::vec3 m_forward;
    mutable glm::vec3 m_up;
    mutable glm::vec3 m_right;
    mutable bool      m_directionVectorsNeedUpdate;

    mutable Transform m_transform;
    mutable bool      m_transformNeedsUpdate;
    mutable Transform m_inverseTransform;
    mutable bool      m_inverseTransformNeedsUpdate;
};

#endif //TRANSFORMABLE_HPP
