#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "object.hpp"

class Light: public Object {
public:
    Light(GLfloat* vertices, GLsizei vertSize, GLuint* indices, GLsizei indSize, GLfloat innerRadius = 15.f, GLfloat outerRadius = 25.f, GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f, const std::string& type = "dir");
    Light(const Light& light);
    void setColor(const glm::vec3& color);
    [[nodiscard]] std::pair<uint32_t, uint32_t> getDepthMapSize();
    [[nodiscard]] FBO* getFBO();
    [[nodiscard]] GLuint getDepthTextureId() const noexcept;
    GLfloat innerRadius;
    GLfloat outerRadius;
    GLfloat constant;
    GLfloat linear; 
    GLfloat quadratic;
    std::string type;
    bool castShadows;
private:
    void createDepthMap();
    std::shared_ptr<FBO> _fbo;
    GLuint _depthTexId;
    uint32_t _depthMapWidth;
    uint32_t _depthMapHeight;
};

#endif