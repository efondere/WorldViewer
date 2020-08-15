#ifndef MESH_HPP
#define MESH_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

#include <iostream>

class Camera;

class Mesh
{

public:

    enum class RenderMode
    {
        Wireframe,
        Faces,
        ShadedWireframe,
        ShadedFaces,
        Normals
    };

    struct Vertex
    {
        Vertex(const float& x, const float& y, const float& z,
               const float& nx, const float& ny, const float& nz)
        : position(x, y, z)
        , normal(nx, ny, nz)
        {
        }

        glm::vec3 position;
        glm::vec3 normal;
    };

    struct TriangleIndices
    {
        TriangleIndices(const unsigned int& _p1, const unsigned int& _p2, const unsigned int& _p3)
        : p1(_p1)
        , p2(_p2)
        , p3(_p3)
        {
        }

        unsigned int p1;
        unsigned int p2;
        unsigned int p3;
    };

    struct FaceIndices
    {
        FaceIndices(const TriangleIndices& _t1, const TriangleIndices& _t2)
        : t1(_t1)
        , t2(_t2)
        {
        }

        TriangleIndices t1;
        TriangleIndices t2;
    };

    Mesh(const std::string& defaultShaderName, const std::string& colorShaderName, const std::string& textureFilePath);

    ~Mesh();

    void setViewProjMatrix(const glm::mat4& matrix)
    {
        m_viewProjMatrix = matrix;
    }

    void setProjMatrix(const glm::mat4& matrix)
    {
        m_projMatrix = matrix;
    }

    void setViewPosition(const glm::vec3& position)
    {
        m_viewPosition = position;
    }

    void setPosition(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f))
    {
        m_position = position;
    }

    void setRotation(glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f))
    {
        m_rotation = rotation;
    }

    void setScale(glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f))
    {
        m_scale = scale;
    }

    void move(glm::vec3 factor)
    {
        m_position += factor;
    }

    void rotate(glm::vec3 factor)
    {
        m_rotation += factor;
    }

    void scale(glm::vec3 factor)
    {
        m_scale += factor;
    }

    glm::vec3 getPosition() const
    {
        return m_position;
    }

    virtual void loadFromFile(const std::string& filePath);

    virtual void update(float dt)
    {
    }

    void draw(Camera* camera, RenderMode renderMode = RenderMode::Faces) const;

protected:

    std::vector<Vertex>      m_vertices;
    std::vector<FaceIndices> m_faces;

    unsigned int m_vaoID;
    unsigned int m_vboID;
    unsigned int m_iboID;
    unsigned int m_defaultShaderProgramID;
    unsigned int m_colorShaderProgramID;
    unsigned int m_wireframeShaderProgramID;
    unsigned int m_normalDisplayProgramID;
    unsigned int m_colorTextureID;
    unsigned int m_objTextureID;
    bool         m_useObjTexture;

private:

    unsigned int createProgram(const std::string& shaderName);
    unsigned int createProgramGeom(const std::string& shaderName);
    unsigned int compileShader(unsigned int type, const std::string& source);

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
    glm::mat4 m_viewProjMatrix;
    glm::mat4 m_projMatrix;
    glm::vec3 m_viewPosition;
    
};

#endif //MESH_HPP
