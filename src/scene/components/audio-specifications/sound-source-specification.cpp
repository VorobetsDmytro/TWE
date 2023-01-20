#include "scene/components/audio-specifications/sound-source-specification.hpp"

namespace TWE {
    SoundSourceSpecification::SoundSourceSpecification()
        : _is3D(false), _playLooped(false), _startPaused(false), _volume(0.f), _minDistance(0.f), _maxDistance(0.f), _playbackSpeed(0.f) {}

    SoundSourceSpecification::SoundSourceSpecification(irrklang::ISoundSource* soundSource, irrklang::ISoundEngine* soundEngine, 
    bool is3D, bool playLooped, bool startPaused)
    : _soundEngine(soundEngine), _is3D(is3D), _playLooped(playLooped), _startPaused(startPaused), _minDistance(1.f), _maxDistance(100.f), _playbackSpeed(1.f) {
        if(!soundSource)
            return;
        _soundSourcePath = soundSource->getName();
        _volume = soundSource->getDefaultVolume();
    }

    SoundSourceSpecification::SoundSourceSpecification(const SoundSourceSpecification& soundSourceSpecification) {
        this->_is3D = soundSourceSpecification._is3D;
        this->_playLooped = soundSourceSpecification._playLooped;
        this->_soundEngine = soundSourceSpecification._soundEngine;
        this->_sounds = soundSourceSpecification._sounds;
        this->_soundSourcePath = soundSourceSpecification._soundSourcePath;
        this->_startPaused = soundSourceSpecification._startPaused;
        this->_volume = soundSourceSpecification._volume;
        this->_minDistance = soundSourceSpecification._minDistance;
        this->_maxDistance = soundSourceSpecification._maxDistance;
        this->_playbackSpeed = soundSourceSpecification._playbackSpeed;
    }

    irrklang::ISound* SoundSourceSpecification::play(const glm::vec3& position) {
        auto soundSource = _soundEngine->getSoundSource(_soundSourcePath.string().c_str());
        if(!soundSource)
            return nullptr;
        irrklang::ISound* sound;
        if(_is3D)
            sound = _soundEngine->play3D(soundSource, {position.x, position.y, position.z}, _playLooped, true);
        else
            sound = _soundEngine->play2D(soundSource, _playLooped, true);
        sound->setVolume(_volume);
        sound->setIsPaused(_startPaused);
        sound->setMinDistance(_minDistance);
        sound->setMaxDistance(_maxDistance);
        sound->setPlaybackSpeed(_playbackSpeed);
        _sounds.push_back(sound);
        return sound;
    }

    void SoundSourceSpecification::setVolume(float volume) {
        _volume = volume;
        for(auto sound : _sounds)
            if(sound)
                sound->setVolume(volume);
    }
    
    void SoundSourceSpecification::setMinDistance(float value) {
        _minDistance = value;
        for(auto sound : _sounds)
            if(sound)
                sound->setMinDistance(value);
    }

    void SoundSourceSpecification::setMaxDistance(float value) {
        _maxDistance = value;
        for(auto sound : _sounds)
            if(sound)
                sound->setMaxDistance(value);
    }

    void SoundSourceSpecification::setPlaybackSpeed(float value) {
        _playbackSpeed = value;
        for(auto sound : _sounds)
            if(sound)
                sound->setPlaybackSpeed(value);
    }


    void SoundSourceSpecification::setIs3D(bool is3D) {
        _is3D = is3D;
    }

    void SoundSourceSpecification::setPlayLooped(bool playLooped) {
        _playLooped = playLooped;
    }

    void SoundSourceSpecification::setStartPaused(bool startPaused) {
        _startPaused = startPaused;
    }

    irrklang::ISoundSource* SoundSourceSpecification::getSoundSource() {
        return _soundEngine->getSoundSource(_soundSourcePath.string().c_str());
    }

    irrklang::ISoundEngine* SoundSourceSpecification::getSoundEngine() { return _soundEngine; }
    std::vector<irrklang::ISound*>& SoundSourceSpecification::getSounds() { return _sounds; }
    std::filesystem::path SoundSourceSpecification::getSoundSourcePath() const noexcept { return _soundSourcePath; }
    float SoundSourceSpecification::getVolume() const noexcept { return _volume; }
    float SoundSourceSpecification::getMinDistance() const noexcept { return _minDistance; }
    float SoundSourceSpecification::getMaxDistance() const noexcept { return _maxDistance; }
    float SoundSourceSpecification::getPlaybackSpeed() const noexcept { return _playbackSpeed; }
    bool SoundSourceSpecification::getIs3D() const noexcept { return _is3D; }
    bool SoundSourceSpecification::getPlayLooped() const noexcept { return _playLooped; }
    bool SoundSourceSpecification::getStartPaused() const noexcept { return _startPaused; }
}