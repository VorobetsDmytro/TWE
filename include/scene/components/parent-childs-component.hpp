#ifndef PARENT_CHILDS_COMPONENT_HPP
#define PARENT_CHILDS_COMPONENT_HPP

#include <vector>

#include "entt/entt.hpp"

namespace TWE {
    class ParentChildsComponent {
    public:
        ParentChildsComponent();
        ParentChildsComponent(entt::entity parent, const std::vector<entt::entity>& childs);
        ParentChildsComponent(const ParentChildsComponent& parentChildsComponent);
        entt::entity parent;
        std::vector<entt::entity> childs;
    };
}

#endif