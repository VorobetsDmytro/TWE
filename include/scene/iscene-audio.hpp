#ifndef ISCENE_AUDIO_HPP
#define ISCENE_AUDIO_HPP

#include <entt/entt.hpp>
#include "irrKlang/include/irrKlang.h"

#include "scene/scene-camera-specification.hpp"

namespace TWE {
    class ISceneAudio {
    public:
        ISceneAudio() = default;
        virtual void reset() {}
        virtual void startAudioOnRun(entt::registry* registry) {}
        virtual void setAudioPauseState(entt::registry* registry, bool paused) {}
        virtual bool updateAudioListenerPosition(const SceneCameraSpecification& cameraSpec) { return false; }
        virtual irrklang::ISoundEngine* getSoundEngine() { return nullptr; }
    };
}

#endif