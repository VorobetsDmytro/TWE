#ifndef REDERER_HPP
#define REDERER_HPP

#include "scene/components/components.hpp"

namespace TWE {
    class Renderer {
    public:
        static void execute(MeshComponent* meshComponent, MeshRendererComponent* meshRendererComponent, TransformComponent* transformComponent);
        static void cleanScreen(const glm::vec4& color);
        static void setViewport(int startX, int startY, int endX, int endY);
        [[nodiscard]] static GLuint generateCubemapTexture(const std::vector<std::string>& texPaths);
    };
}

#endif