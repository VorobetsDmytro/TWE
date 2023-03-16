#ifndef CHANGE_AUDIO_COMPONENT_PROPERTIES_COMMAND_HPP
#define CHANGE_AUDIO_COMPONENT_PROPERTIES_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    struct AudioComponentPropertiesSpecification {
        AudioComponentPropertiesSpecification() = default;
        AudioComponentPropertiesSpecification(bool is3d, bool startPaused, bool playLooped, float volume, float minDistance, float maxDistance, float playbackSpeed)
            : is3d(is3d), startPaused(startPaused), playLooped(playLooped), volume(volume), minDistance(minDistance), 
            maxDistance(maxDistance), playbackSpeed(playbackSpeed) {}
        bool operator==(const AudioComponentPropertiesSpecification& other) {
            return this->is3d == other.is3d
                && this->startPaused == other.startPaused
                && this->playLooped == other.playLooped
                && this->volume == other.volume
                && this->minDistance == other.minDistance
                && this->maxDistance == other.maxDistance
                && this->playbackSpeed == other.playbackSpeed;
        }
        bool operator!=(const AudioComponentPropertiesSpecification& other) {
            return !(*this == other);
        }
        bool is3d;
        bool startPaused;
        bool playLooped;
        float volume;
        float minDistance;
        float maxDistance;
        float playbackSpeed;
    };

    class ChangeAudioComponentPropertiesCommand: public IURCommand {
    public:
        ChangeAudioComponentPropertiesCommand(const Entity& entity, const std::filesystem::path& soundSourcePath, 
           const AudioComponentPropertiesSpecification& oldState, const AudioComponentPropertiesSpecification& newState);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
        std::filesystem::path _soundSourcePath;
        AudioComponentPropertiesSpecification _oldState;
        AudioComponentPropertiesSpecification _newState;
    };
}

#endif