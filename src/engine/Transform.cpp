#include "Transform.hpp"
#include <glm/gtc/matrix_transform.hpp>

const Transform Transform::Identity;

Transform::Transform()
: m_matrix(1.f)
{
}

Transform::Transform(const glm::mat4& matrix)
: m_matrix(matrix)
{
}

glm::mat4 Transform::getMatrix() const
{
    return m_matrix;
}

Transform Transform::getInverseTransform() const
{
    return Transform(glm::inverse(m_matrix));
}

Transform& Transform::combine(const Transform& other)
{
    m_matrix = other.getMatrix() * m_matrix;
    return *this;
}

void Transform::translate(float x, float y, float z)
{
    return translate({x, y, z});
}

void Transform::translate(const glm::vec3& offset)
{
    m_matrix = glm::translate(m_matrix, offset);
}

void Transform::rotate(float x, float y, float z)
{
    rotate({x, y, z});
}

void Transform::rotate(const glm::vec3& angles)
{
    m_matrix = glm::rotate(m_matrix, glm::radians(angles.x), {1.f, 0.f, 0.f});
    m_matrix = glm::rotate(m_matrix, glm::radians(angles.y), {0.f, 1.f, 0.f});
    m_matrix = glm::rotate(m_matrix, glm::radians(angles.z), {0.f, 0.f, 1.f});
}

void Transform::rotate(float degrees, const glm::vec3& axis)
{
    m_matrix = glm::rotate(m_matrix, glm::radians(degrees), axis);
}

void Transform::scale(float x, float y, float z)
{
    scale({x, y, z});
}

void Transform::scale(const glm::vec3& factors)
{
    m_matrix = glm::scale(m_matrix, factors);
}

void Transform::reset()
{
    m_matrix = glm::mat4(1.f);
}

Transform operator *(const Transform& left, const Transform& right)
{
    return Transform(left).combine(right);
}

Transform& operator *=(Transform& left, const Transform& right)
{
    return left.combine(right);
}

