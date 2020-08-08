#include "Transformable.hpp"

Transformable::Transformable()
: m_origin(0, 0, 0)
, m_position(0, 0, 0)
, m_rotation()
, m_scale(1, 1, 1)
, m_forward(0, 0, -1)
, m_up(0, 1, 0)
, m_right(1, 0, 0)
, m_directionVectorsNeedUpdate(true)
, m_transform()
, m_transformNeedsUpdate(true)
, m_inverseTransform()
, m_inverseTransformNeedsUpdate(true)
{
}

Transformable::~Transformable()
{
}

void Transformable::setPosition(float x, float y, float z)
{
    setPosition({x, y, z});
}

void Transformable::setPosition(const glm::vec3& position)
{
    m_position = position;
    m_transformNeedsUpdate = true;
    m_inverseTransformNeedsUpdate = true;
}

void Transformable::setRotation(float x, float y, float z)
{
    setRotation({x, y, z});
}

void Transformable::setRotation(const glm::vec3& rotation)
{
    m_rotation.x = static_cast<float>(fmod(rotation.x, 360));
    m_rotation.y = static_cast<float>(fmod(rotation.y, 360));
    m_rotation.z = static_cast<float>(fmod(rotation.z, 360));

    if (m_rotation.x < 0)
        m_rotation.x += 360.f;
    if (m_rotation.y < 0)
        m_rotation.y += 360.f;
    if (m_rotation.z < 0)
        m_rotation.z += 360.f;
    
    m_rotation = rotation;
    m_directionVectorsNeedUpdate = true;
    m_transformNeedsUpdate = true;
    m_inverseTransformNeedsUpdate = true;
}

void Transformable::setScale(float x, float y, float z)
{
    setScale({x, y, z});
}

void Transformable::setScale(const glm::vec3& scale)
{
    m_scale = scale;
    m_transformNeedsUpdate = true;
    m_inverseTransformNeedsUpdate = true;
}

void Transformable::setOrigin(float x, float y, float z)
{
    setOrigin({x, y, z});
}

void Transformable::setOrigin(const glm::vec3& origin)
{
    m_origin = origin;
    m_transformNeedsUpdate = true;
    m_inverseTransformNeedsUpdate = true;
}

const glm::vec3& Transformable::getPosition() const
{
    return m_position;
}

const glm::vec3& Transformable::getRotation() const
{
    return m_rotation;
}

const glm::vec3& Transformable::getScale() const
{
    return m_scale;
}

const glm::vec3& Transformable::getOrigin() const
{
    return m_origin;
}

void Transformable::move(float offsetX, float offsetY, float offsetZ)
{
    move({offsetX, offsetY, offsetZ});
}

void Transformable::move(const glm::vec3& offset)
{
    setPosition(m_position + offset);
}

void Transformable::rotate(float angleX, float angleY, float angleZ)
{
    rotate({angleX, angleY, angleZ});
}

void Transformable::rotate(const glm::vec3& angles)
{
    setRotation(m_rotation + angles);
}

void Transformable::scale(float factorX, float factorY, float factorZ)
{
    setScale({factorX, factorY, factorZ});
}

void Transformable::scale(const glm::vec3& factor)
{
    setScale(m_scale.x * factor.x, m_scale.y * factor.y, m_scale.z * factor.z);
}

const Transform& Transformable::getTransform() const
{
    if (m_transformNeedsUpdate)
    {
        // I'm pretty sure that's not how it works, but...
        m_transform.reset();
        m_transform.translate(m_position - m_origin);
        m_transform.rotate(m_rotation.x, m_rotation.y, m_rotation.z);
        m_transform.scale(m_scale);

        m_transformNeedsUpdate = false;
    }

    return m_transform;
}

const Transform& Transformable::getInverseTransform() const
{
    if (m_inverseTransformNeedsUpdate)
    {
        m_inverseTransform = getTransform().getInverseTransform();
        m_inverseTransformNeedsUpdate = false;
    }

    return m_inverseTransform;
}

const glm::vec3& Transformable::getForwardVector() const
{
    updateDirectionalVectors();

    return m_forward;
}

const glm::vec3& Transformable::getUpVector() const
{
    updateDirectionalVectors();

    return m_up;
}

const glm::vec3& Transformable::getRightVector() const
{
    updateDirectionalVectors();

    return m_right;
}

void Transformable::updateDirectionalVectors() const
{
    if (!m_directionVectorsNeedUpdate)
        return;


    m_forward.x = cos(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
    m_forward.y = sin(glm::radians(m_rotation.x));
    m_forward.z = sin(glm::radians(m_rotation.y)) * cos(glm::radians(m_rotation.x));
    m_forward = glm::normalize(m_forward);

    // also re-calculate the Right and Up vector
    m_up.x = cos(glm::radians(-m_rotation.z)) * cos(glm::radians(m_rotation.x));
    m_up.y = sin(glm::radians(m_rotation.x));
    m_up.z = sin(glm::radians(-m_rotation.z)) * cos(glm::radians(m_rotation.x));
    m_up = glm::normalize(m_up);

    m_right = glm::normalize(glm::cross(m_forward, m_up));  // make sure this is not left vector

    m_directionVectorsNeedUpdate = false;
}

