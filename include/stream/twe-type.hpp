#ifndef TWE_TYPE_HPP
#define TWE_TYPE_HPP

#include <string>

namespace TWE {
    enum TWETypes {
        Debug,
        Release
    };

    extern const std::string TWE_TYPE_OPTIONS[2];

    [[nodiscard]] std::string getTWEType() noexcept;
}

#endif