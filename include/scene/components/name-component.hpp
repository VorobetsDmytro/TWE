#ifndef NAME_COMPONENT_HPP
#define NAME_COMPONENT_HPP

#include <string>

namespace TWE {
    class NameComponent {
    public:
        NameComponent();
        NameComponent(const std::string& name);
        void setName(const std::string& name);
        [[nodiscard]] std::string getName() const noexcept;
    private:
        std::string _name;
    };
}

#endif