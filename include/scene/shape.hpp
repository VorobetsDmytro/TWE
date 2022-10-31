#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <string>
#include <entt/entt.hpp>

#include "scene/scene.hpp"
#include "model-loader/modelloaderdata.hpp"
#include "scene/components/components.hpp"
#include "renderer/renderer.hpp"

namespace TWE {
    class Shape{
    public:
        Shape() = default;
        static Entity createCubeEntity(Scene* scene, const std::vector<std::string>& texPaths = {});
        static Entity createPlateEntity(Scene* scene, const std::vector<std::string>& texPaths = {});
        static Entity createCubemapEntity(Scene* scene, const std::vector<std::string>& texPaths);
        static Entity createSpotLightEntity(Scene* scene, GLfloat innerRadius = 15.f, GLfloat outerRadius = 20.f, GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f);
        static Entity createPointLightEntity(Scene* scene, GLfloat constant = 1.f, GLfloat linear = 0.045f, GLfloat quadratic = 0.0075f);
        static Entity createDirLightEntity(Scene* scene);
        static std::vector<Entity> createModelEntity(Scene* scene, ModelLoaderData* modelLoaderData);
    private:
        static GLfloat cubeVertices[];
        static GLuint cubeIndices[];
        static GLfloat plateVertices[];
        static GLuint plateIndices[];
        static GLuint cubemapIndices[];
    };
}

#endif