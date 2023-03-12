#include "scene/components/transform-component.hpp"

namespace TWE {
    TransformComponent::TransformComponent(): _needRecache(true) {}

    TransformComponent::TransformComponent(const TransformComponent& transform) {
        this->_transform = transform._transform;
        this->_preTransform = transform._preTransform;
        this->_model = transform._model;
        this->_needRecache = transform._needRecache;
    }

    void TransformComponent::move(const glm::vec3& pos, bool acceptToChilds) {
        auto model = glm::translate(getModel(), pos / _transform.size);
        _transform.position = model[3];
        if(!acceptToChilds)
            _preTransform.position = _transform.position;
        _needRecache = true;
    }

    void TransformComponent::rotate(float angle, const glm::vec3& axis, bool acceptToChilds) {
        setRotation({ _transform.rotation.x + (axis.x * (angle * PI / 180.f)), 
                      _transform.rotation.y + (axis.y * (angle * PI / 180.f)),
                      _transform.rotation.z + (axis.z * (angle * PI / 180.f)) }, acceptToChilds);
    }

    void TransformComponent::scale(const glm::vec3& size, bool acceptToChilds) {
        _transform.size *= size;
        if(!acceptToChilds)
            _preTransform.size = _transform.size;
        _needRecache = true;
    }

    void TransformComponent::setPosition(const glm::vec3& pos, bool acceptToChilds) {
        _transform.position = pos;
        if(!acceptToChilds)
            _preTransform.position = _transform.position;
        _needRecache = true;
    }

    void TransformComponent::setRotation(float angle, const glm::vec3& axis, bool acceptToChilds) {
        _transform.rotation = glm::eulerAngles(glm::angleAxis(angle, axis)) * PI / 180.f;
        if(!acceptToChilds)
            _preTransform.rotation = _transform.rotation;
        _needRecache = true;
    }

    void TransformComponent::setRotation(const glm::vec3& angles, bool acceptToChilds) {
        _transform.rotation = angles;
        if(!acceptToChilds)
            _preTransform.rotation = _transform.rotation;
        _needRecache = true;
    }

    void TransformComponent::rotateAroundOrigin(const glm::vec3& angles, const glm::vec3& centerPosition, bool acceptToChilds) {
        glm::vec3 zeroPos = getZeroRotationAroundPos(centerPosition);
        _transform.rotation += angles;
        auto translate1 = glm::translate(glm::mat4(1.f), -centerPosition);
        auto rotation = glm::toMat4(glm::quat(_transform.rotation));
        auto translate2 = glm::translate(glm::mat4(1.f), centerPosition);
        auto transform = translate2 * rotation * translate1;
        _transform.position = transform * glm::vec4(zeroPos, 1.f);
        if(!acceptToChilds) {
            _preTransform.rotation = _transform.rotation;
            _preTransform.position = _transform.position;
        }
        _needRecache = true;
    }

    glm::vec3 TransformComponent::getZeroRotationAroundPos(const glm::vec3& centerPosition) {
        auto translate1 = glm::translate(glm::mat4(1.f), -centerPosition);
        auto rotation = glm::inverse(glm::toMat4(glm::quat(_transform.rotation)));
        auto translate2 = glm::translate(glm::mat4(1.f), centerPosition);
        auto transform = translate2 * rotation * translate1;
        return transform * glm::vec4(_transform.position, 1.f);
    }

    void TransformComponent::setSize(const glm::vec3& size, bool acceptToChilds) {
        _transform.size = size;
        if(!acceptToChilds)
            _preTransform.size = _transform.size;
        _needRecache = true;
    }

    glm::mat4 TransformComponent::getModel() {
        if(_needRecache) {
            _model = glm::translate(glm::mat4(1.f), _transform.position)
                  * glm::toMat4(glm::quat(_transform.rotation))
                  * glm::scale(glm::mat4(1.f), _transform.size);
            _needRecache = false;
        }
        return _model;
    }

    const ModelSpecification& TransformComponent::getTransform() const noexcept { return _transform; }
    const glm::vec3& TransformComponent::getPosition() const noexcept { return _transform.position; }
    const glm::vec3& TransformComponent::getRotation() const noexcept { return _transform.rotation; }
    const glm::vec3& TransformComponent::getSize() const noexcept { return _transform.size; }
    glm::vec3 TransformComponent::getForward() { return glm::normalize(glm::vec3(getModel()[2])); }
    glm::vec3 TransformComponent::getRight() { return glm::normalize(glm::vec3(getModel()[0])); }
    glm::vec3 TransformComponent::getUp() { return glm::normalize(glm::vec3(getModel()[1])); }

    void TransformComponent::setPreTransform(const ModelSpecification& preTransform) {
        _preTransform = preTransform;
    }

    const ModelSpecification& TransformComponent::getPreTransform() const noexcept { return _preTransform; }
}