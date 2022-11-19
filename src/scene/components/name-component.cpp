#include "scene/components/name-component.hpp"

namespace TWE {
    NameComponent::NameComponent(): _name("") {}

    NameComponent::NameComponent(const std::string& name): _name(name) {}

    void NameComponent::setName(const std::string& name){
        _name = name;
    }

    [[nodiscard]] std::string NameComponent::getName() const noexcept { return _name; }
}