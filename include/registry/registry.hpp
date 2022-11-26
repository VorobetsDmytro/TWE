#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include <map>
#include <string>
#include <iostream>
#include <algorithm>

namespace TWE {
    template<typename T>
    class Registry {
    public:
        template<typename Instance = T>
        void add(const std::string& key);
        T* get(const std::string& key);
        void erase(const std::string& key);
    private:
        std::map<std::string, T*> _registry;
    };

    template<typename T>
    template<typename Instance>
    void Registry<T>::add(const std::string& key) {
        auto item = _registry.find(key);
        if(item != _registry.end())
            return;
        _registry.insert({key, new Instance});
    }

    template<typename T>
    T* Registry<T>::get(const std::string& key) {
        auto item = _registry.find(key);
        if(item == _registry.end())
            return nullptr;
        return item->second;
    }

    template<typename T>
    void Registry<T>::erase(const std::string& key) {
        auto item = _registry.find(key);
        if(item == _registry.end())
            return;
        _registry.erase(key);
    }
}

#endif