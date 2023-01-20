#include "scene/components/audio-component.hpp"

namespace TWE {
    AudioComponent::AudioComponent(): _soundEngine(nullptr) {}

    AudioComponent::AudioComponent(irrklang::ISoundEngine* soundEngine): _soundEngine(soundEngine) {}

    AudioComponent::AudioComponent(const AudioComponent& audioComponent) {
        this->_soundEngine = audioComponent._soundEngine;
        this->_soundSources = audioComponent._soundSources;
    }

    SoundSourceSpecification* AudioComponent::addSoundSource(const std::filesystem::path& path, bool is3D, float defaultVolume) {
        if(!_soundEngine || !std::filesystem::exists(path))
            return nullptr;
        auto it = std::find_if(_soundSources.begin(), _soundSources.end(), [&](const SoundSourceSpecification* soundSourceItem) {
            return path == soundSourceItem->getSoundSourcePath();
        });
        if(it != _soundSources.end())
            return *it;
        irrklang::ISoundSource* soundSource;
        soundSource = _soundEngine->getSoundSource(path.string().c_str());
        if(!soundSource)
            soundSource = _soundEngine->addSoundSourceFromFile(path.string().c_str());
        soundSource->setDefaultVolume(defaultVolume);
        SoundSourceSpecification* soundSourceSpecification = new SoundSourceSpecification{ soundSource, _soundEngine, is3D };
        soundSourceSpecification->setVolume(defaultVolume);
        _soundSources.push_back(soundSourceSpecification);
        return soundSourceSpecification;
    }

    SoundSourceSpecification* AudioComponent::getSoundSourceSpec(const std::filesystem::path& path) {
        auto it = std::find_if(_soundSources.begin(), _soundSources.end(), [&](const SoundSourceSpecification* soundSourceItem) {
            return path == soundSourceItem->getSoundSourcePath();
        });
        if(it == _soundSources.end())
            return nullptr;
        return *it;
    }

    SoundSourceSpecification* AudioComponent::getSoundSourceSpec(int index) {
        if(_soundSources.empty() || index < 0)
            return nullptr;
        return _soundSources[index];
    }

    bool AudioComponent::removeSoundSource(SoundSourceSpecification* soundSourceSpec) {
        auto it = std::find_if(_soundSources.begin(), _soundSources.end(), [&](const SoundSourceSpecification* soundSourceItem) {
            return soundSourceSpec->getSoundSourcePath() == soundSourceItem->getSoundSourcePath();
        });
        if(it == _soundSources.end())
            return false;
        auto soundSource = _soundEngine->getSoundSource(soundSourceSpec->getSoundSourcePath().string().c_str());
        if(!soundSource)
            return false;
        auto& sounds = soundSourceSpec->getSounds();
        for(auto sound : sounds) {
            sound->stop();
            sound->drop();
        }
        sounds.clear();
        _soundSources.erase(it);
        return true;
    }

    void AudioComponent::clean() {
        for(auto soundSourceSpec : _soundSources)
            removeSoundSource(soundSourceSpec);
    }

    std::vector<std::string> AudioComponent::getPathsOfSoundSources() {
        std::vector<std::string> res;
        for(auto soundSource : _soundSources)
            res.push_back(soundSource->getSoundSourcePath().string());
        return res;
    }

    std::vector<SoundSourceSpecification*> AudioComponent::getSoundSources() { return _soundSources; }
    irrklang::ISoundEngine* AudioComponent::getSoundEngine() { return _soundEngine; }
}