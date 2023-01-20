#ifndef SOUND_SOURCE_SPECIFICATION_HPP
#define SOUND_SOURCE_SPECIFICATION_HPP

#include <vector>
#include <filesystem>
#include <algorithm>
#include <glm/glm.hpp>

#include "irrKlang/include/irrKlang.h"

namespace TWE {
    class SoundSourceSpecification {
    public:
        SoundSourceSpecification();
        SoundSourceSpecification(irrklang::ISoundSource* soundSource, irrklang::ISoundEngine* soundEngine, bool is3D, bool playLooped = false, bool startPaused = false);
        SoundSourceSpecification(const SoundSourceSpecification& soundSourceSpecification);
        void setVolume(float volume);
        void setMinDistance(float value);
        void setMaxDistance(float value);
        void setPlaybackSpeed(float value);
        void setIs3D(bool is3D);
        void setPlayLooped(bool playLooped);
        void setStartPaused(bool startPaused);
        irrklang::ISound* play(const glm::vec3& position = glm::vec3(0.f));
        [[nodiscard]] irrklang::ISoundSource* getSoundSource();
        [[nodiscard]] irrklang::ISoundEngine* getSoundEngine();
        [[nodiscard]] std::vector<irrklang::ISound*>& getSounds();
        [[nodiscard]] std::filesystem::path getSoundSourcePath() const noexcept;
        [[nodiscard]] float getVolume() const noexcept;
        [[nodiscard]] float getMinDistance() const noexcept;
        [[nodiscard]] float getMaxDistance() const noexcept;
        [[nodiscard]] float getPlaybackSpeed() const noexcept;
        [[nodiscard]] bool getIs3D() const noexcept;
        [[nodiscard]] bool getPlayLooped() const noexcept;
        [[nodiscard]] bool getStartPaused() const noexcept;
    private:
        bool _is3D;
        bool _playLooped;
        bool _startPaused;
        float _volume;
        float _minDistance;
        float _maxDistance;
        float _playbackSpeed;
        std::filesystem::path _soundSourcePath;
        std::vector<irrklang::ISound*> _sounds;
        irrklang::ISoundEngine* _soundEngine;
    };
}

#endif