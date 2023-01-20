#ifndef SCENE_AUDIO_HPP
#define SCENE_AUDIO_HPP

#include "irrKlang/include/irrKlang.h"

namespace TWE {
    class SceneAudio {
    public:
        SceneAudio();
        ~SceneAudio();
        void reset();
        irrklang::ISoundEngine* getSoundEngine();
    private:
        irrklang::ISoundEngine* _soundEngine;
    };
}

#endif