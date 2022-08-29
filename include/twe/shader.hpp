#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad.h>
#include <memory>
#include "file.hpp"
#include "material.hpp"

enum TransformMatrixOptions {
    MODEL,
    VIEW,
    PROJECTION
};

extern const char* TRANS_MAT_OPTIONS[3];

class Shader {
public:
    Shader() = default;
    Shader(const Shader& shader);
    Shader(const char* vertPath, const char* fragPath);
    ~Shader();
    void use();
    void clean();
    void setMaterial(const Material& material);
    void setUniform(const char* name, const glm::vec3& vec);
    void setUniform(const char* name, GLfloat value);
    void setUniform(const char* name, GLuint value);
    void setUniform(const char* name, GLint value);
    void setUniform(const char* name, GLboolean value);
    [[nodiscard]] GLuint getId() const noexcept;
    [[nodiscard]] Material& getMaterial() const noexcept;
private:
    [[nodiscard]] GLuint create(const char* fileContent, GLenum shaderType);
    void linkShaders(GLuint* shaders, GLuint size);
    void initMaterial();
    GLuint _id;
    std::shared_ptr<Material> _material;
};

#endif