#include "undo-redo/ur-commands/audio-component/add-audio-component-command.hpp"

namespace TWE {
    AddAudioComponentCommand::AddAudioComponentCommand(const Entity& entity): _entity(entity) {}

    void AddAudioComponentCommand::execute() {
        if(_entity.hasComponent<AudioComponent>())
            return;
        _entity.addComponent<AudioComponent>(_entity.getScene()->getSceneAudio()->getSoundEngine());
    }

    void AddAudioComponentCommand::unExecute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        _entity.removeComponent<AudioComponent>();
    }
}