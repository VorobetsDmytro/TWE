#ifndef ADD_SOUND_SOURCE_AUDIO_COMPONENT_COMMAND_HPP
#define ADD_SOUND_SOURCE_AUDIO_COMPONENT_COMMAND_HPP

#include "scene/iscene.hpp"
#include "scene/components/components.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class AddSoundSourceAudioComponentCommand: public IURCommand {
    public:
        AddSoundSourceAudioComponentCommand(const Entity& entity, const std::filesystem::path& soundSourcePath);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
        std::filesystem::path _soundSourcePath;
    };
}

#endif