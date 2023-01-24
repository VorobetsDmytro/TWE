#include "undo-redo/ur-commands/audio-component/remove-audio-component-command.hpp"

namespace TWE {
    RemoveAudioComponentCommand::RemoveAudioComponentCommand(const Entity& entity): _entity(entity) {}

    void RemoveAudioComponentCommand::execute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        auto& audioComponent = _entity.getComponent<AudioComponent>();
        _audioComponent = audioComponent;
        audioComponent.clean();
        _entity.removeComponent<AudioComponent>();
    }

    void RemoveAudioComponentCommand::unExecute() {
        if(_entity.hasComponent<AudioComponent>())
            return;
        _entity.addComponent<AudioComponent>(_audioComponent);
    }
}