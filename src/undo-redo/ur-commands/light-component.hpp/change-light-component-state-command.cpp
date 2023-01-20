#include "undo-redo/ur-commands/light-component.hpp/change-light-component-state-command.hpp"

namespace TWE {
    ChangeLightComponentStateCommand::ChangeLightComponentStateCommand(const Entity& entity, const LightComponent& oldState, const LightComponent& newState) 
    : _entity(entity), _oldState(oldState), _newState(newState) {}

    void ChangeLightComponentStateCommand::execute() {
        if(!_entity.hasComponent<LightComponent>())
            return;
        auto& lightComponent = _entity.getComponent<LightComponent>();
        lightComponent.innerRadius = _newState.innerRadius;
        lightComponent.outerRadius = _newState.outerRadius;
        lightComponent.color = _newState.color;
        lightComponent.castShadows = _newState.castShadows;
        lightComponent.constant = _newState.constant;
        lightComponent.linear = _newState.linear;
        lightComponent.quadratic = _newState.quadratic;
        lightComponent.type = _newState.type;
        if(_newState.getFBO())
            lightComponent.setShadowMapSize(_newState.getFBO()->getSize().first);
        lightComponent.setFBO(_newState.getFBO());
        lightComponent.setLightProjectionAspect(_newState.getLightProjectionAspect());
    }

    void ChangeLightComponentStateCommand::unExecute() {
        if(!_entity.hasComponent<LightComponent>())
            return;
        auto& lightComponent = _entity.getComponent<LightComponent>();
        lightComponent.innerRadius = _oldState.innerRadius;
        lightComponent.outerRadius = _oldState.outerRadius;
        lightComponent.color = _oldState.color;
        lightComponent.castShadows = _oldState.castShadows;
        lightComponent.constant = _oldState.constant;
        lightComponent.linear = _oldState.linear;
        lightComponent.quadratic = _oldState.quadratic;
        lightComponent.type = _oldState.type;
        if(_newState.getFBO())
            lightComponent.setShadowMapSize(_oldState.getFBO()->getSize().first);
        lightComponent.setFBO(_oldState.getFBO());
        lightComponent.setLightProjectionAspect(_oldState.getLightProjectionAspect());
    }
}