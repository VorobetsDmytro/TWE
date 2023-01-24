#ifndef SCENE_AUDIO_HPP
#define SCENE_AUDIO_HPP

#include <vector>
#include <entt/entt.hpp>
#include "irrKlang/include/irrKlang.h"

#include "scene/components/components.hpp"
#include "scene/scene-camera-specification.hpp"

namespace TWE {
    class SceneAudio {
    public:
        SceneAudio();
        ~SceneAudio();
        void reset();
        void startAudioOnRun(entt::registry* registry);
        void setAudioPauseState(entt::registry* registry, bool paused);
        bool updateAudioListenerPosition(const SceneCameraSpecification& cameraSpec);
        irrklang::ISoundEngine* getSoundEngine();
    private:
        irrklang::ISoundEngine* _soundEngine;
        std::vector<irrklang::ISound*> _pausedSounds;
    };
}

#endif