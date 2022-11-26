#include "scene/components/creation-type-component.hpp"

namespace TWE {
    CreationTypeComponent::CreationTypeComponent(): _type(EntityCreationType::None){}

    CreationTypeComponent::CreationTypeComponent(EntityCreationType type)
    : _type(type) {}

    void CreationTypeComponent::setType(EntityCreationType type) {
        _type = type;
    }

    EntityCreationType CreationTypeComponent::getType() const noexcept { return _type; }
}