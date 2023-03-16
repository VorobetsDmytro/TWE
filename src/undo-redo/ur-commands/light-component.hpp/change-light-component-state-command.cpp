#include "undo-redo/ur-commands/light-component.hpp/change-light-component-state-command.hpp"

namespace TWE {
    ChangeLightComponentStateCommand::ChangeLightComponentStateCommand(const Entity& entity, const LightComponent& oldState, const LightComponent& newState) 
    : _entity(entity), _oldState(oldState), _newState(newState) {}

    void ChangeLightComponentStateCommand::execute() {
        if(!_entity.hasComponent<LightComponent>())
            return;
        auto& lightComponent = _entity.getComponent<LightComponent>();
        lightComponent.setInnerRadius(_newState.getInnerRadius());
        lightComponent.setOuterRadius(_newState.getOuterRadius());
        lightComponent.setColor(_newState.getColor());
        lightComponent.setCastShadows(_newState.getCastShadows());
        lightComponent.setConstant(_newState.getConstant());
        lightComponent.setLinear(_newState.getLinear());
        lightComponent.setQuadratic(_newState.getQuadratic());
        lightComponent.setType(_newState.getType());
        if(_newState.getFBO())
            lightComponent.setShadowMapSize(_newState.getFBO()->getSize().width);
        lightComponent.setFBO(_newState.getFBO());
        lightComponent.setLightProjectionAspect(_newState.getLightProjectionAspect());
    }

    void ChangeLightComponentStateCommand::unExecute() {
        if(!_entity.hasComponent<LightComponent>())
            return;
        auto& lightComponent = _entity.getComponent<LightComponent>();
        lightComponent.setInnerRadius(_oldState.getInnerRadius());
        lightComponent.setOuterRadius(_oldState.getOuterRadius());
        lightComponent.setColor(_oldState.getColor());
        lightComponent.setCastShadows(_oldState.getCastShadows());
        lightComponent.setConstant(_oldState.getConstant());
        lightComponent.setLinear(_oldState.getLinear());
        lightComponent.setQuadratic(_oldState.getQuadratic());
        lightComponent.setType(_oldState.getType());
        if(_oldState.getFBO())
            lightComponent.setShadowMapSize(_oldState.getFBO()->getSize().width);
        lightComponent.setFBO(_oldState.getFBO());
        lightComponent.setLightProjectionAspect(_oldState.getLightProjectionAspect());
    }
}