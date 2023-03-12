#include "scene/scene-audio.hpp"

namespace TWE {
    SceneAudio::SceneAudio() {
        _soundEngine = irrklang::createIrrKlangDevice();
    }

    SceneAudio::~SceneAudio() {
        _soundEngine->drop();
    }

    void SceneAudio::reset() {
        _soundEngine->stopAllSounds();
        _soundEngine->removeAllSoundSources();
        _pausedSounds.clear();
    }

    void SceneAudio::startAudioOnRun(entt::registry* registry) {
        registry->view<AudioComponent, TransformComponent>().each([](entt::entity entity, 
        AudioComponent& audioComponent, TransformComponent& transformComponent){
            auto& soundSources = audioComponent.getSoundSources();
            for(auto soundSource : soundSources)
                soundSource->play(transformComponent.getPosition());
        });
    }

    void SceneAudio::setAudioPauseState(entt::registry* registry, bool paused) {
        if(paused) {
           registry->view<AudioComponent>().each([&](entt::entity entity, AudioComponent& audioComponent){
                auto& soundSources = audioComponent.getSoundSources();
                for(auto soundSource : soundSources) {
                    auto& sounds = soundSource->getSounds();
                    for(auto sound : sounds) {
                        if(!sound->getIsPaused()) {
                            sound->setIsPaused(true);
                            _pausedSounds.push_back(sound);
                        }
                    }
                }
            });
        } else {
            registry->view<AudioComponent>().each([&](entt::entity entity, AudioComponent& audioComponent){
                auto& soundSources = audioComponent.getSoundSources();
                for(auto soundSource : soundSources) {
                    auto& sounds = soundSource->getSounds();
                    for(auto sound : sounds) {
                        for(auto soundPaused : _pausedSounds)
                            if(sound == soundPaused)
                                soundPaused->setIsPaused(false);
                    }
                }
            });
            _pausedSounds.clear();
        }
    }

    bool SceneAudio::updateAudioListenerPosition(const SceneCameraSpecification& cameraSpec) {
        if(!cameraSpec.camera)
            return false;
        irrklang::vec3df pos = { cameraSpec.position.x, cameraSpec.position.y, cameraSpec.position.z };
        irrklang::vec3df lookdir = { -cameraSpec.forward.x, -cameraSpec.forward.y, -cameraSpec.forward.z };
        _soundEngine->setListenerPosition(pos, lookdir);
        return true;
    }

    irrklang::ISoundEngine* SceneAudio::getSoundEngine() { return _soundEngine; }
}