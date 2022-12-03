#include "gui/gui-components.hpp"

namespace TWE {
    void GUIComponents::showComponentPanel(Entity& entity) {
        ImGui::Begin("Components");
        if(entity.getSource() != entt::null) {
            showNameComponent(entity);
            showTransformComponent(entity);
            showMeshComponent(entity);
            showMeshRendererComponent(entity);
            showCameraComponent(entity);
            showLightComponent(entity);
            showPhysicsComponent(entity);
            showScriptComponent(entity);
        }
        ImGui::End();
    }

    void GUIComponents::setScene(Scene* scene) {
        _scene = scene;
    }

    void GUIComponents::showNameComponent(Entity& entity) {
        if(entity.hasComponent<NameComponent>()) {
            auto& nameComponent = entity.getComponent<NameComponent>();
            auto& name = nameComponent.getName();
            ImGui::Text("Name");
            ImGui::SameLine();
            if(ImGui::InputText("##Name", &name))
                nameComponent.setName(name);
        }
    }

    void GUIComponents::showTransformComponent(Entity& entity) {
        if(entity.hasComponent<TransformComponent>()) {
            auto id = (void*)(typeid(TransformComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Transform")) {
                auto& transformComponent = entity.getComponent<TransformComponent>();
                bool hasPhysics = entity.hasComponent<PhysicsComponent>();
                if(dragFloat3("Position", transformComponent.position, 0.05f, 0.f)) {
                    transformComponent.setPosition(transformComponent.position);
                    if(hasPhysics)
                        entity.getComponent<PhysicsComponent>().setPosition(transformComponent.position);
                }
                auto rotation = transformComponent.rotation * 180.f / PI;
                if(dragFloat3("Rotation", rotation, hasPhysics ? 1.5f : 0.5f, 0.f)) {
                    rotation *= PI / 180.f;
                    transformComponent.setRotation(rotation);
                    if(hasPhysics)
                        entity.getComponent<PhysicsComponent>().setRotation(rotation);
                }
                if(dragFloat3("Scale", transformComponent.size, 0.01f, 1.f, 0.01f, 999999.f)) {
                    transformComponent.setSize(transformComponent.size);
                    if(hasPhysics)
                        entity.getComponent<PhysicsComponent>()
                            .setSize(_scene->getDynamicWorld(), transformComponent.size);
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponents::showMeshComponent(Entity& entity) {
        if(entity.hasComponent<MeshComponent>()) {
            auto id = (void*)(typeid(MeshComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh")) {
                auto& meshComponent = entity.getComponent<MeshComponent>();
                if(!meshComponent.textures.empty())
                    ImGui::Image((void*)(uint64_t)meshComponent.textures[0]->getId(), {30.f, 30.f});
                ImGui::TreePop();
            }
        }
    }

    void GUIComponents::showMeshRendererComponent(Entity& entity) {
        if(entity.hasComponent<MeshRendererComponent>()) {
            auto id = (void*)(typeid(MeshRendererComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Mesh Renderer")) {
                auto& meshRendererComponent = entity.getComponent<MeshRendererComponent>();
                if(ImGui::TreeNodeEx(id, flags, "Material")) {
                    colorEdit3("Color", meshRendererComponent.material->objColor);
                    dragFloat("Ambient", meshRendererComponent.material->ambient, 0.01f);
                    dragFloat("Diffuse", meshRendererComponent.material->diffuse, 0.1f, 0.1f, 999999.f);
                    dragFloat("Specular", meshRendererComponent.material->specular, 0.1f, 0.f, 999999.f);
                    dragFloat("Shininess", meshRendererComponent.material->shininess, 1.f, 12.f, 999999.f);
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponents::showCameraComponent(Entity& entity) {
        if(entity.hasComponent<CameraComponent>()) {
            auto id = (void*)(typeid(CameraComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Camera")) {
                auto& cameraComponent = entity.getComponent<CameraComponent>();
                auto* cameraSource = cameraComponent.getSource();
                int typeIndex = combo("Projection", cameraProjectionTypes[cameraSource->getType()], cameraProjectionTypes, 105.f);
                if(typeIndex == CameraProjectionType::Perspective) {
                    auto& perspectiveSpecification = cameraSource->getPerspectiveSpecification();
                    cameraSource->setPerspective(perspectiveSpecification.fov, perspectiveSpecification.wndWidth, 
                        perspectiveSpecification.wndHeight, perspectiveSpecification.nearDepth, perspectiveSpecification.farDepth);
                } else if(typeIndex == CameraProjectionType::Orthographic) {
                    auto& orthographicSpecification = cameraSource->getOrthographicSpecification();
                    cameraSource->setOrthographic(orthographicSpecification.left, orthographicSpecification.right, 
                        orthographicSpecification.bottom, orthographicSpecification.top, orthographicSpecification.nearDepth, orthographicSpecification.farDepth);
                }
                auto isFocusedOn = cameraComponent.isFocusedOn();
                if(checkBox("Is focused on", isFocusedOn, 105.f))
                    cameraComponent.setFocuse(isFocusedOn);
                if(cameraSource->getType() == CameraProjectionType::Perspective) {
                    auto& perspectiveSpecification = cameraSource->getPerspectiveSpecification();
                    if(dragFloat("FOV", perspectiveSpecification.fov, 1.f, 1.f, 140.f, 105.f))
                        cameraSource->setPerspective(perspectiveSpecification.fov, perspectiveSpecification.wndWidth, 
                            perspectiveSpecification.wndHeight, perspectiveSpecification.nearDepth, perspectiveSpecification.farDepth);
                } else if(cameraSource->getType() == CameraProjectionType::Orthographic) {
                    auto& orthographicSpecification = cameraSource->getOrthographicSpecification();
                    auto aspect = orthographicSpecification.right;
                    if(dragFloat("Aspect", aspect, 0.2f, 0.2f, 100.f, 105.f))
                        cameraSource->setOrthographic(-aspect, aspect, -aspect, aspect, 
                            orthographicSpecification.nearDepth, orthographicSpecification.farDepth);
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponents::showLightComponent(Entity& entity) {
        if(entity.hasComponent<LightComponent>()) {
            auto id = (void*)(typeid(LightComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Light")) {
                auto& lightComponent = entity.getComponent<LightComponent>();
                std::string selectedType = lightTypes[lightComponent.type];
                int typeIndex = combo("Type", lightTypes[lightComponent.type], lightTypes, 100.f);
                if(typeIndex > -1)
                    lightComponent.setType(static_cast<LightType>(typeIndex));
                colorEdit3("Color", lightComponent.color, 100.f);
                if(lightComponent.type == LightType::Spot) {
                    dragFloat("Inner radius", lightComponent.innerRadius, 0.2f, 0.f, 999999.f, 100.f);
                    dragFloat("Outer radius", lightComponent.outerRadius, 0.2f, 0.f, 999999.f, 100.f);
                }
                if(lightComponent.type == LightType::Spot || lightComponent.type == LightType::Point) {
                    dragFloat("Constant", lightComponent.constant, 0.05f, 0.01f, 999999.f, 100.f);
                    dragFloat("Linear", lightComponent.linear, 0.01f, 0.01f, 999999.f, 100.f);
                    dragFloat("Quadratic", lightComponent.quadratic, 0.001f, 0.001f, 999999.f, 100.f);
                }
                if(lightComponent.type == LightType::Dir) {
                    if(checkBox("Cast shadows", lightComponent.castShadows, 100.f))
                        lightComponent.setCastShadows(lightComponent.castShadows);
                }
                ImGui::TreePop();
            }
        }
    }

    void GUIComponents::showPhysicsComponent(Entity& entity) {
        if(entity.hasComponent<PhysicsComponent>()) {
            auto id = (void*)(typeid(PhysicsComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Physics")) {
                auto& physicsComponent = entity.getComponent<PhysicsComponent>();
                auto mass = physicsComponent.getMass();
                if(dragFloat("Mass", mass, 0.01f, 0.f, 999999.f))
                    physicsComponent.setMass(_scene->getDynamicWorld(), mass);
                ImGui::TreePop();
            }
        }
    }

    void GUIComponents::showScriptComponent(Entity& entity) {
        if(entity.hasComponent<ScriptComponent>()) {
            auto id = (void*)(typeid(ScriptComponent).hash_code());
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
            if(ImGui::TreeNodeEx(id, flags, "Script")) {
                auto& scriptComponent = entity.getComponent<ScriptComponent>();
                text("Name", scriptComponent.getBehaviorClassName());
                ImGui::TreePop();
            }
        }
    }

    void GUIComponents::text(const std::string& label, const std::string& value, float labelColumnWidth) {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        ImGui::Text(value.c_str());
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
    }

    bool GUIComponents::checkBox(const std::string& label, bool& value, float labelColumnWidth) {
        bool isInteracted = false;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());
        
        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        if(ImGui::Checkbox("##Checkbox", &value))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }

    int GUIComponents::combo(const std::string& label, std::string& selected, std::vector<std::string>& options, float labelColumnWidth) {
        int res = -1;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        int size = options.size();
        if(ImGui::BeginCombo("##Combo", selected.c_str())) {
            for(int i = 0; i < size; ++i) {
                bool isSelected = options[i] == selected;
                if(ImGui::Selectable(options[i].c_str(), isSelected))
                    res = i;
                if(isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return res;
    }

    bool GUIComponents::colorEdit3(const std::string& label, glm::vec3& values, float labelColumnWidth) {
        bool isInteracted = false;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        if(ImGui::ColorEdit3("##ColorEdit3", glm::value_ptr(values)))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }

    bool GUIComponents::dragFloat(const std::string& label, float& value, float step, float min, float max, float labelColumnWidth) {
        bool isInteracted = false;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushItemWidth(ImGui::CalcItemWidth());
        if(ImGui::DragFloat("##Value", &value, step, min, max, "%.3f"))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }

    bool GUIComponents::dragFloat3(const std::string& label, glm::vec3& values, float step, float resetValue, float min, float max, float labelColumnWidth) {
        bool isInteracted = false;
        ImGuiButtonFlags btnFlags = ImGuiButtonFlags_PressedOnDoubleClick;
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, labelColumnWidth);
        ImGui::Text(label.c_str());

        ImGui::NextColumn();
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.f, 3.f });
        float height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 size = { height + 3.f, height };

        if(ImGui::ButtonEx("X", size, btnFlags)) {
            isInteracted = true;
            values.x = resetValue;
        }
        ImGui::SameLine();
        if(ImGui::DragFloat("##X", &values.x, step, min, max, "%.2f"))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if(ImGui::ButtonEx("Y", size, btnFlags)) {
            isInteracted = true;
            values.y = resetValue;
        }
        ImGui::SameLine();
        if(ImGui::DragFloat("##Y", &values.y, step, min, max, "%.2f"))
            isInteracted = true;
        ImGui::PopItemWidth();

        ImGui::SameLine();
        if(ImGui::ButtonEx("Z", size, btnFlags)) {
            isInteracted = true;
            values.z = resetValue;
        }
        ImGui::SameLine();
        if(ImGui::DragFloat("##Z", &values.z, step, min, max, "%.2f"))
            isInteracted = true;
        ImGui::PopItemWidth();
        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
        return isInteracted;
    }
}