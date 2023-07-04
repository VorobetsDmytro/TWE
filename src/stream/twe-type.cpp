#include "stream/twe-type.hpp"

namespace TWE {
    const std::string TWE_TYPE_OPTIONS[2] = {
        "Debug",
        "Release"
    };

    std::string getTWEType() noexcept {
        short tweType;
        #ifdef TWE_DEBUG
        tweType = TWETypes::Debug;
        #else
        tweType = TWETypes::Release;
        #endif
        return TWE_TYPE_OPTIONS[tweType];
    }
}