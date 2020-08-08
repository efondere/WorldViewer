#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glm/glm.hpp>

class Transform
{

public:

    enum class Space : uint8_t
    {
        Global,
        Local
    };
    
    // Creates an identity matrix
    Transform();

    // Create a transform from a pre-existing matrix
    Transform(const glm::mat4& matrix);

    glm::mat4 getMatrix() const;

    Transform getInverseTransform() const;

    Transform& combine(const Transform& other);

    void translate(float x, float y, float z);

    void translate(const glm::vec3& offset);

    void rotate(float x, float y, float z);

    void rotate(const glm::vec3& angles);

    void rotate(float degrees, const glm::vec3& axis);

    void scale(float x, float y, float z);

    void scale(const glm::vec3& factors);

    void reset();

    static const Transform Identity;

private:

    glm::mat4 m_matrix;
};

Transform operator *(const Transform& left, const Transform& right);

Transform& operator *=(Transform& left, const Transform& right);

#endif //TRANSFORM_HPP
