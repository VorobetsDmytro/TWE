#ifndef GUI_DRAG_AND_DROP_TYPE_HPP
#define GUI_DRAG_AND_DROP_TYPE_HPP

#include <vector>
#include <string>

namespace TWE {
    enum GUIDragAndDropType {
        DirectoryItem = 0
    };

    extern std::vector<std::string> guiDragAndDropTypes;
}

#endif