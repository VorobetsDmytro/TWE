#include "undo-redo/ur-commands/audio-component/change-audio-component-properties-command.hpp"

namespace TWE {
    ChangeAudioComponentPropertiesCommand::ChangeAudioComponentPropertiesCommand(const Entity& entity, const std::filesystem::path& soundSourcePath,
        const AudioComponentPropertiesSpecification& oldState, const AudioComponentPropertiesSpecification& newState)
        : _entity(entity), _soundSourcePath(soundSourcePath), _oldState(oldState), _newState(newState) {}

    void ChangeAudioComponentPropertiesCommand::execute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        auto& audioComponent = _entity.getComponent<AudioComponent>();
        auto soundSource = audioComponent.getSoundSourceSpec(_soundSourcePath);
        if(!soundSource)
            return;
        soundSource->setIs3D(_newState.is3d);
        soundSource->setPlayLooped(_newState.playLooped);
        soundSource->setStartPaused(_newState.startPaused);
        soundSource->setVolume(_newState.volume);
        soundSource->setMinDistance(_newState.minDistance);
        soundSource->setMaxDistance(_newState.maxDistance);
        soundSource->setPlaybackSpeed(_newState.playbackSpeed);
    }

    void ChangeAudioComponentPropertiesCommand::unExecute() {
        if(!_entity.hasComponent<AudioComponent>())
            return;
        auto& audioComponent = _entity.getComponent<AudioComponent>();
        auto soundSource = audioComponent.getSoundSourceSpec(_soundSourcePath);
        if(!soundSource)
            return;
        soundSource->setIs3D(_oldState.is3d);
        soundSource->setPlayLooped(_oldState.playLooped);
        soundSource->setStartPaused(_oldState.startPaused);
        soundSource->setVolume(_oldState.volume);
        soundSource->setMinDistance(_oldState.minDistance);
        soundSource->setMaxDistance(_oldState.maxDistance);
        soundSource->setPlaybackSpeed(_oldState.playbackSpeed);
    }
}