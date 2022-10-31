#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad.h>
#include <gtc/type_ptr.hpp>

#include "stream/file.hpp"
#include "material.hpp"

namespace TWE {
    enum TransformMatrixOptions {
        MODEL,
        VIEW,
        PROJECTION
    };

    enum ShaderIndices {
        DEFAULT_VERT,
        DEFAULT_FRAG,
        TEXTURE_FRAG,
        LIGHT_VERT,
        LIGHT_FRAG,
        CUBEMAP_VERT,
        CUBEMAP_FRAG,
    };

    extern const char* TRANS_MAT_OPTIONS[3];

    extern const char* SHADER_PATHS[7];

    class Shader {
    public:
        Shader() = default;
        Shader(const Shader& shader);
        Shader(const char* vertPath, const char* fragPath);
        ~Shader();
        void use();
        void clean();
        void setMaterial(const Material& material);
        void setUniform(const char* name, const glm::mat4& mat);
        void setUniform(const char* name, const glm::vec3& vec);
        void setUniform(const char* name, GLfloat value);
        void setUniform(const char* name, GLuint value);
        void setUniform(const char* name, GLint value);
        void setUniform(const char* name, GLboolean value);
        [[nodiscard]] GLuint getId() const noexcept;
    private:
        [[nodiscard]] GLuint create(const char* fileContent, GLenum shaderType);
        void linkShaders(GLuint* shaders, GLuint size);
        GLuint _id;
    };
}

#endif