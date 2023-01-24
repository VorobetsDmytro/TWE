#ifndef REMOVE_AUDIO_COMPONENT_COMMAND_HPP
#define REMOVE_AUDIO_COMPONENT_COMMAND_HPP

#include "scene/scene.hpp"
#include "undo-redo/iur-command.hpp"

namespace TWE {
    class RemoveAudioComponentCommand: public IURCommand  {
    public:
        RemoveAudioComponentCommand(const Entity& entity);
        void execute() override;
        void unExecute() override;
    private:
        Entity _entity;
        AudioComponent _audioComponent;
    };
}

#endif