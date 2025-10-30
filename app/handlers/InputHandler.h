#ifndef VKSENGINE_INPUTHANDLER_H
#define VKSENGINE_INPUTHANDLER_H

#include <functional>
#include <unordered_map>

#include "../objects/input/Button.h"
#include "../objects/input/Key.h"

namespace vks_engine
{
    class InputHandler
    {
    public:
        void handleKeyboard(int key, int scancode, int action, int mods);

        void handleMouse(int button, int action, int mods);

        void setKeyCallbackFunction(const Key &key, std::function<void()> function);

        void setButtonCallbackFunction(const Button &button, std::function<void()> function);

    private:
        std::unordered_map<Key, std::function<void()> > m_KeyFunctionCallbacks;

        std::unordered_map<Button, std::function<void()> > m_ButtonFunctionCallbacks;
    };
}

#endif //VKSENGINE_INPUTHANDLER_H
