#ifndef SCENE_AUDIO_HPP
#define SCENE_AUDIO_HPP

#include <vector>
#include <entt/entt.hpp>
#include "irrKlang/include/irrKlang.h"

#include "scene/iscene-audio.hpp"
#include "scene/components/components.hpp"

namespace TWE {
    class SceneAudio: public ISceneAudio {
    public:
        SceneAudio();
        ~SceneAudio();
        void reset() override;
        void startAudioOnRun(entt::registry* registry) override;
        void setAudioPauseState(entt::registry* registry, bool paused) override;
        bool updateAudioListenerPosition(const SceneCameraSpecification& cameraSpec) override;
        irrklang::ISoundEngine* getSoundEngine() override;
    private:
        irrklang::ISoundEngine* _soundEngine;
        std::vector<irrklang::ISound*> _pausedSounds;
    };
}

#endif