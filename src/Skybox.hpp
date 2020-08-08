// http://www.mbsoftworks.sk/tutorials/opengl3/
#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <glad/gl.h>
#include "Shader.hpp"
#include <stb_image/stb_image.h>
#include <string>
#include <sstream>

class Skybox
{
public:

    Skybox(const std::string& skyboxName, Camera* camera, const glm::mat4& projectionMatrix, bool skyboxTextureIsPng = true)
    : m_shader("./res/shaders/skybox.shader")
    , m_projectionMatrix(projectionMatrix)
    , m_camera(camera)
    {
        glGenTextures(1, &m_textureID);

        float skyboxVertices[] = {
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        m_shader.setUniform1i("u_texture", 0);

        loadFromFiles(skyboxName, skyboxTextureIsPng);
    }

    ~Skybox()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteTextures(1, &m_textureID);
    }

    bool loadFromFiles(const std::string& skyboxName, bool isPng)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

        int width, height, channelCount;
        for (unsigned int i = 0; i < 6; i++)
        {
            std::stringstream fileName;
            fileName << "./res/textures/skyboxes/" << skyboxName << "/"
                << skyboxName << "-" << std::to_string(i) << (isPng ? ".png" : ".jpg");

            stbi_set_flip_vertically_on_load(0);

            uint8_t* data = stbi_load(fileName.str().c_str(), &width, &height, &channelCount, 3);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                    GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Failed to open image: " << fileName.str() << std::endl;
                stbi_image_free(data);
                return false;
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return true;
    }

    void draw()
    {
        glDepthFunc(GL_LEQUAL);

        m_shader.bind();

        glm::mat4 view = glm::mat4(glm::mat3(m_camera->getViewMatrix())); // remove translation
        m_shader.setUniformMat4f("u_view", view);
        m_shader.setUniformMat4f("u_projection", m_projectionMatrix);

        glBindVertexArray(m_VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthFunc(GL_LESS); // set it back to default
    }

private:

    unsigned int m_VAO;
    unsigned int m_VBO;
    Shader m_shader;
    unsigned int m_textureID;

    const glm::mat4& m_projectionMatrix;
    Camera* m_camera;
};

#endif //SKYBOX_HPP
