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
    }

    irrklang::ISoundEngine* SceneAudio::getSoundEngine() { return _soundEngine; }
}