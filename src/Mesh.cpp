#include <glad/gl.h>
#include "Mesh.hpp"
#include "Camera.hpp"
#include "glUtil.hpp"
#include "util.hpp"
#include <stb_image/stb_image.h>
#include <fstream>
#include <iostream>
#include <sstream>

std::string getShaderSource(const std::string& filePath)
{
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        std::cout << "Could not open file: " << filePath << std::endl;
        return {};
    }

    std::string line;
    std::stringstream stream;
    while (getline(inputFile, line))
    {
        stream << line << "\n";
    }

    inputFile.close();

    return stream.str();
}

Mesh::Mesh(const std::string& defaultShaderName, const std::string& colorShaderName, const std::string& textureFilePath)
: m_vertices()
, m_faces()
, m_vaoID(0)
, m_vboID(0)
, m_iboID(0)
, m_defaultShaderProgramID(0)
, m_colorShaderProgramID(0)
, m_colorTextureID(0)
, m_position(0.f, 0.f, 0.f)
, m_rotation(0.f, 0.f, 0.f)
, m_scale(1.f, 1.f, 1.f)
, m_viewProjMatrix(1.f)
{
    m_defaultShaderProgramID = createProgram(defaultShaderName);
    m_colorShaderProgramID = createProgram(colorShaderName);
    m_wireframeShaderProgramID = createProgramGeom("wireframe");

    stbi_set_flip_vertically_on_load(1);

    int imageWidth, imageHeight, imageBytesPerPixel;
    uint8_t* imageData = stbi_load("./res/textures/height_texture_01.fw.png", &imageWidth, &imageHeight, &imageBytesPerPixel, 4);
    
    glGenTextures(1, &m_colorTextureID);
    glBindTexture(GL_TEXTURE_2D, m_colorTextureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (imageData)
        stbi_image_free(imageData);
}

Mesh::~Mesh()
{
    glDeleteProgram(m_colorShaderProgramID);
    glDeleteProgram(m_defaultShaderProgramID);
    glUseProgram(0);

    glDeleteTextures(1, &m_colorTextureID);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_vertices.size() != 0)
    {
        glDeleteBuffers(1, &m_iboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDeleteBuffers(1, &m_vboID);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDeleteVertexArrays(1, &m_vaoID);
        glBindVertexArray(0);
    }
}

void Mesh::loadFromFile(const std::string& filePath)
{
    // TODO: Load obj files
}

void Mesh::draw(Camera* camera, RenderMode renderMode) const
{
    if (m_vertices.size() == 0)
    {
        std::cout << "Nothing to draw: no mesh loaded." << std::endl;
        return;
    }

    glm::mat4 modelMatrix(1.f);
    modelMatrix = glm::translate(modelMatrix, m_position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.x),glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.y),glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_rotation.z),glm::vec3(0,0,1));
    modelMatrix = glm::scale(modelMatrix, m_scale);

    glm::mat4 mvpMatrix = m_viewProjMatrix * modelMatrix;

    // We should bind our vertex and buffer arrays

    glBindVertexArray(m_vaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboID);

    if (renderMode == RenderMode::Wireframe)
    {
        glUseProgram(m_wireframeShaderProgramID);
        glUniformMatrix4fv(glGetUniformLocation(m_wireframeShaderProgramID, "u_MVP"), 1, GL_FALSE, &mvpMatrix[0][0]);
        // Use a for loop and draw the elements with an offset to avoid drawing lines over each other and looping back
        // Also, use GL_LINES
        // We should also cache the uniform locations
        // http://docs.gl/gl3/glDrawRangeElements
        glDrawElements(GL_TRIANGLES, m_faces.size() * 2 * 3, GL_UNSIGNED_INT, nullptr);
    }
    else if (renderMode == RenderMode::Faces)
    {
        glUseProgram(m_defaultShaderProgramID);
        glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgramID, "u_MVP"), 1, GL_FALSE, &mvpMatrix[0][0]);
        glDrawElements(GL_TRIANGLES, m_faces.size() * 2 * 3, GL_UNSIGNED_INT, nullptr);
    }
    //else if (renderMode == RenderMode::ShadedWireframe)
    //{
    //    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, m_colorTextureID);
//
    //    glUseProgram(m_colorShaderProgramID);
    //    glUniformMatrix4fv(glGetUniformLocation(m_colorShaderProgramID, "u_MVP"), 1, GL_FALSE, &mvpMatrix[0][0]);
    //    glUniformMatrix4fv(glGetUniformLocation(m_colorShaderProgramID, "u_modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
    //    glUniformMatrix4fv(glGetUniformLocation(m_colorShaderProgramID, "u_viewMatrix"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
    //    glUniform1i(glGetUniformLocation(m_colorShaderProgramID, "u_texture"), 0);
    //    glDrawElements(GL_LINE_STRIP, m_faces.size() * 2 * 3, GL_UNSIGNED_INT, nullptr);
//
    //    glBindTexture(GL_TEXTURE_2D, 0);
    //}
    else if (renderMode == RenderMode::ShadedFaces)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_colorTextureID);

        float lightColor[3] = { 1.f, 1.f, 1.f };
        glm::vec3 lightPosition(10.f, 250.f, 0.f);

        glUseProgram(m_colorShaderProgramID);
        glUniformMatrix4fv(glGetUniformLocation(m_colorShaderProgramID, "u_MVP"), 1, GL_FALSE, &mvpMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_colorShaderProgramID, "u_modelMatrix"), 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_colorShaderProgramID, "u_viewMatrix"), 1, GL_FALSE, &camera->getViewMatrix()[0][0]);
        glUniform3f(glGetUniformLocation(m_colorShaderProgramID, "u_lightPos"), lightPosition.x, lightPosition.y, lightPosition.z);
        glUniform3f(glGetUniformLocation(m_colorShaderProgramID, "u_viewPos"), m_viewPosition[0], m_viewPosition[1], m_viewPosition[2]);
        glUniform3f(glGetUniformLocation(m_colorShaderProgramID, "u_lightColor"), lightColor[0], lightColor[1], lightColor[2]);
        glUniform1i(glGetUniformLocation(m_colorShaderProgramID, "u_texture"), 0);
        glUniform4f(glGetUniformLocation(m_colorShaderProgramID, "u_fogColor"), 0.2, 0.0, 0.3, 1.0);
        glUniform1f(glGetUniformLocation(m_colorShaderProgramID, "u_fogDensity"), 0.005f);
        glDrawElements(GL_TRIANGLES, m_faces.size() * 2 * 3, GL_UNSIGNED_INT, nullptr);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

unsigned int Mesh::createProgram(const std::string& shaderName)
{
    std::string vertexSource = getShaderSource("./res/shaders/" + shaderName + ".vert");
    std::string fragmentSource = getShaderSource("./res/shaders/" + shaderName + ".frag");

    glClearErrors();
    unsigned int id = glCreateProgram();
    ASSERT(glLogCall("glCreateProgram()", __FILE__, __LINE__));

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);
    glValidateProgram(id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return id;
}

unsigned int Mesh::createProgramGeom(const std::string& shaderName)
{
    std::string vertexSource = getShaderSource("./res/shaders/" + shaderName + ".vert");
    std::string fragmentSource = getShaderSource("./res/shaders/" + shaderName + ".frag");
    std::string geometrySource = getShaderSource("./res/shaders/" + shaderName + ".geom");

    glClearErrors();
    unsigned int id = glCreateProgram();
    ASSERT(glLogCall("glCreateProgram()", __FILE__, __LINE__));

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    unsigned int geometryShader = compileShader(GL_GEOMETRY_SHADER, geometrySource);
    
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glAttachShader(id, geometryShader);
    glLinkProgram(id);
    glValidateProgram(id);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    return id;
}

unsigned int Mesh::compileShader(unsigned int type, const std::string& shaderSource)
{
    unsigned int id = glCreateShader(type);
    const char* src = shaderSource.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
