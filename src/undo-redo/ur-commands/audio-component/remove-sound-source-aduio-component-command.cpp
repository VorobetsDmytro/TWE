#include "undo-redo/ur-commands/audio-component/remove-sound-source-aduio-component-command.hpp"

namespace TWE {
    RemoveSoundSourceAudioComponentCommand::RemoveSoundSourceAudioComponentCommand(const Entity& entity, const std::filesystem::path& soundSourcePath)
        : _entity(entity), _soundSourcePath(soundSourcePath) {}

    void RemoveSoundSourceAudioComponentCommand::execute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        auto& audioComponent = _entity.getComponent<AudioComponent>();
        auto soundSource = audioComponent.getSoundSourceSpec(_soundSourcePath);
        if(!soundSource)
            return;
        _soundSourceSpecification = *soundSource;
        audioComponent.removeSoundSource(soundSource);
    }

    void RemoveSoundSourceAudioComponentCommand::unExecute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        auto& audioComponent = _entity.getComponent<AudioComponent>();
        auto soundSource = audioComponent.addSoundSource(_soundSourceSpecification.getSoundSourcePath(), _soundSourceSpecification.getIs3D());
        if(!soundSource)
            return;
        soundSource->setPlayLooped(_soundSourceSpecification.getPlayLooped());
        soundSource->setStartPaused(_soundSourceSpecification.getStartPaused());
        soundSource->setVolume(_soundSourceSpecification.getVolume());
    }
}