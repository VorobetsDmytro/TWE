#ifndef UR_CONTROL_HPP
#define UR_CONTROL_HPP

#include <stack>

#include "undo-redo/iur-command.hpp"

namespace TWE {
    class URControl {
    public:
        void undo();
        void redo();
        void execute(IURCommand* command);
        void reset();
        [[nodiscard]] const std::stack<IURCommand*>& getUndoStack() const noexcept;
        [[nodiscard]] const std::stack<IURCommand*>& getRedoStack() const noexcept;
    private:
        std::stack<IURCommand*> _undoStack;
        std::stack<IURCommand*> _redoStack;
    };
}

#endif