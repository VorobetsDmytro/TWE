#ifndef GUI_STATE_SPECIFICATION_HPP
#define GUI_STATE_SPECIFICATION_HPP

#include <vector>
#include <future>
#include <imgui.h>
#include <ImGuizmo.h>

#include "scene/iscene.hpp"
#include "stream/project-creator.hpp"
#include "input/window.hpp"

namespace TWE {
    enum class GizmoOperation {
        None = -1,
        Translate = ImGuizmo::OPERATION::TRANSLATE,
        Rotate = ImGuizmo::OPERATION::ROTATE,
        Scale = ImGuizmo::OPERATION::SCALE,
    };

    class GUIStateSpecification {
    public:
        GUIStateSpecification() = default;
        IScene* scene;
        Entity selectedEntity;
        ProjectData* projectData;
        Window* window;
        std::vector<std::shared_future<void>> bgFuncsInRun;
        int readPixelFBOData;
        bool isMouseOnViewport;
        bool isFocusedOnViewport;
        GizmoOperation gizmoOperation = GizmoOperation::Translate;
        int preUndoCount = 0;
        int undoCount = 0;
    };
}

#endif