#include "scene/components/parent-childs-component.hpp"

namespace TWE {
    ParentChildsComponent::ParentChildsComponent(): parent(entt::null) {}

    ParentChildsComponent::ParentChildsComponent(entt::entity parent, const std::vector<entt::entity>& childs)
        : parent(parent), childs(childs) {}

    ParentChildsComponent::ParentChildsComponent(const ParentChildsComponent& parentChildsComponent) {
        this->parent = parentChildsComponent.parent;
        this->childs = parentChildsComponent.childs;
    }
}