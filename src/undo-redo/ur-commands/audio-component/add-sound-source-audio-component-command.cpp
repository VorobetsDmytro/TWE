#include "undo-redo/ur-commands/audio-component/add-sound-source-audio-component-command.hpp"

namespace TWE {
    AddSoundSourceAudioComponentCommand::AddSoundSourceAudioComponentCommand(const Entity& entity, const std::filesystem::path& soundSourcePath)
        : _entity(entity), _soundSourcePath(soundSourcePath) {}

    void AddSoundSourceAudioComponentCommand::execute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        auto& audioComponent = _entity.getComponent<AudioComponent>();
        audioComponent.addSoundSource(_soundSourcePath, false);
    }

    void AddSoundSourceAudioComponentCommand::unExecute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        auto& audioComponent = _entity.getComponent<AudioComponent>();
        auto soundSourceSpec = audioComponent.getSoundSourceSpec(_soundSourcePath);
        audioComponent.removeSoundSource(soundSourceSpec);
    }
}