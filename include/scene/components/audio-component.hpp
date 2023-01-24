#ifndef AUDIO_COMPONENT_HPP
#define AUDIO_COMPONENT_HPP

#include <vector>
#include <filesystem>
#include <algorithm>
#include <glm/glm.hpp>
#include "irrKlang/include/irrKlang.h"

#include "audio-specifications/sound-source-specification.hpp"

namespace TWE {
    class AudioComponent {
    public:
        AudioComponent();
        AudioComponent(irrklang::ISoundEngine* soundEngine);
        AudioComponent(const AudioComponent& AudioComponent);
        void clean();
        SoundSourceSpecification* addSoundSource(const std::filesystem::path& path, bool is3D, float defaultVolume = 0.5f);
        bool removeSoundSource(SoundSourceSpecification* soundSourceSpec);
        [[nodiscard]] SoundSourceSpecification* getSoundSourceSpec(const std::filesystem::path& path);
        [[nodiscard]] SoundSourceSpecification* getSoundSourceSpec(int index);
        [[nodiscard]] std::vector<SoundSourceSpecification*> getSoundSources();
        [[nodiscard]] std::vector<std::string> getPathsOfSoundSources();
        [[nodiscard]] irrklang::ISoundEngine* getSoundEngine();
    private:
        irrklang::ISoundEngine* _soundEngine;
        std::vector<SoundSourceSpecification*> _soundSources;
    };
}

#endif