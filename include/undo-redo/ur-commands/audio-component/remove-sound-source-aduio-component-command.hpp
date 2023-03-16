#ifndef REMOVE_SOUND_SOURCE_AUDIO_COMPONENT_COMMAND_HPP
#define REMOVE_SOUND_SOURCE_AUDIO_COMPONENT_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class RemoveSoundSourceAudioComponentCommand: public IURCommand {
    public:
        RemoveSoundSourceAudioComponentCommand(const Entity& entity, const std::filesystem::path& soundSourcePath);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
        SoundSourceSpecification _soundSourceSpecification;
        std::filesystem::path _soundSourcePath;
    };
}

#endif