#ifndef SHADER_HPP
#define SHADER_HPP

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:

    Shader(const std::string& filePath);

    ~Shader();

    void bind() const;

    void unbind() const;

    // set uniforms
    void setUniform1i(const std::string& name, int value);

    void setUniform1f(const std::string& name, float value);

    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
    
    int getUniformLocation(const std::string& name);

    ShaderProgramSource parseShader(const std::string& filePath);

    unsigned int compileShader(unsigned int type, const std::string& source);

    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

    unsigned int m_rendererID;
    std::string m_filePath;
    std::unordered_map<std::string, int> m_uniformLocationCache;
};

#endif //SHADER_HPP
