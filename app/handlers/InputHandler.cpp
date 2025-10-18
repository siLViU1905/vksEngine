#include "InputHandler.h"

namespace vks_engine
{
    void InputHandler::handleKeyboard(int key, int scancode, int action, int mods)
    {
        Key _key(key, action, mods);

        if (auto it = m_KeyFunctionCallbacks.find(_key); it != m_KeyFunctionCallbacks.end())
            it->second();
    }

    void InputHandler::handleMouse(int button, int action, int mods)
    {
        Button btn(button, action, mods);

        if (auto it = m_ButtonFunctionCallbacks.find(btn); it != m_ButtonFunctionCallbacks.end())
            it->second();
    }

    void InputHandler::setKeyCallbackFunction(const Key &key, std::function<void()> function)
    {
        m_KeyFunctionCallbacks[key] = std::move(function);
    }

    void InputHandler::setButtonCallbackFunction(const Button &button, std::function<void()> function)
    {
        m_ButtonFunctionCallbacks[button] = std::move(function);
    }
}
