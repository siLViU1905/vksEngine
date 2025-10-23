#ifndef VKSENGINEBUTTON_H
#define VKSENGINEBUTTON_H
#include <unordered_map>

#include <GLFW/glfw3.h>

namespace vks_engine
{
    struct Button
    {
        int m_Button;

        int m_Action;

        int m_Mod;

        Button();

        Button(int button, int action, int mod);

        bool operator==(const Button &button) const;

        //=============== BUTTONS ===============
        static constexpr int BUTTON_LEFT = GLFW_MOUSE_BUTTON_1;
        static constexpr int BUTTON_RIGHT = GLFW_MOUSE_BUTTON_2;
        static constexpr int BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_3;
        static constexpr int BUTTON_4 = GLFW_MOUSE_BUTTON_4;
        static constexpr int BUTTON_5 = GLFW_MOUSE_BUTTON_5;
        static constexpr int BUTTON_6 = GLFW_MOUSE_BUTTON_6;
        static constexpr int BUTTON_7 = GLFW_MOUSE_BUTTON_7;
        static constexpr int BUTTON_8 = GLFW_MOUSE_BUTTON_8;

        //=============== ACTIONS ===============
        static constexpr int BUTTON_ACTION_RELEASE = GLFW_RELEASE;
        static constexpr int BUTTON_ACTION_PRESS = GLFW_PRESS;

        //=============== MODS ===============
        static constexpr int BUTTON_MOD_NONE = 0;
        static constexpr int BUTTON_MOD_SHIFT = GLFW_MOD_SHIFT;
        static constexpr int BUTTON_MOD_CONTROL = GLFW_MOD_CONTROL;
        static constexpr int BUTTON_MOD_ALT = GLFW_MOD_ALT;
        static constexpr int BUTTON_MOD_SUPER = GLFW_MOD_SUPER;
        static constexpr int BUTTON_MOD_CAPS_LOCK = GLFW_MOD_CAPS_LOCK;
        static constexpr int BUTTON_MOD_NUM_LOCK = GLFW_MOD_NUM_LOCK;
    };
}

namespace std
{
    template<>
    struct hash<vks_engine::Button>
    {
        size_t operator()(vks_engine::Button const &button) const noexcept
        {
            size_t h1 = hash<int>{}(button.m_Button);
            size_t h2 = hash<int>{}(button.m_Action);
            size_t h3 = hash<int>{}(button.m_Mod);

            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

#endif //VKSENGINEBUTTON_H
