#include "undo-redo/ur-control.hpp"

namespace TWE {
    void URControl::undo() {
        if(_undoStack.empty())
            return;
        auto command = _undoStack.top();
        _undoStack.pop();
        command->unExecute();
        _redoStack.push(command);
    }

    void URControl::redo() {
        if(_redoStack.empty())
            return;
        auto command = _redoStack.top();
        _redoStack.pop();
        command->execute();
        _undoStack.push(command);
    }

    void URControl::execute(IURCommand* command) {
        command->execute();
        _undoStack.push(command);
        while(!_redoStack.empty()) {
            delete _redoStack.top();
            _redoStack.pop();
        }
    }

    void URControl::reset() {
        while(!_undoStack.empty()) {
            delete _undoStack.top();
            _undoStack.pop();
        }
        while(!_redoStack.empty()) {
            delete _redoStack.top();
            _redoStack.pop();
        }
    }

    const std::stack<IURCommand*>& URControl::getUndoStack() const noexcept { return _undoStack; }
    const std::stack<IURCommand*>& URControl::getRedoStack() const noexcept { return _redoStack; }
}