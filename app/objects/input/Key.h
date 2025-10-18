#ifndef VKSENGINEKEY_H
#define VKSENGINEKEY_H
#include <xhash>
#include <GLFW/glfw3.h>

namespace vks_engine
{
    struct Key
    {
        int m_Key;

        //int m_Scancode;

        int m_Action;

        int m_Mod;

        Key();

        //Key(int key, int scancode, int action, int mod);

        Key(int key, int action, int mod);

        bool operator==(const Key &key) const;

        //=============== KEYS ===============
        static constexpr int KEY_UNKNOWN = GLFW_KEY_UNKNOWN;

        static constexpr int KEY_SPACE = GLFW_KEY_SPACE;
        static constexpr int KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE;
        static constexpr int KEY_COMMA = GLFW_KEY_COMMA;
        static constexpr int KEY_MINUS = GLFW_KEY_MINUS;
        static constexpr int KEY_PERIOD = GLFW_KEY_PERIOD;
        static constexpr int KEY_SLASH = GLFW_KEY_SLASH;
        static constexpr int KEY_0 = GLFW_KEY_0;
        static constexpr int KEY_1 = GLFW_KEY_1;
        static constexpr int KEY_2 = GLFW_KEY_2;
        static constexpr int KEY_3 = GLFW_KEY_3;
        static constexpr int KEY_4 = GLFW_KEY_4;
        static constexpr int KEY_5 = GLFW_KEY_5;
        static constexpr int KEY_6 = GLFW_KEY_6;
        static constexpr int KEY_7 = GLFW_KEY_7;
        static constexpr int KEY_8 = GLFW_KEY_8;
        static constexpr int KEY_9 = GLFW_KEY_9;
        static constexpr int KEY_SEMICOLON = GLFW_KEY_SEMICOLON;
        static constexpr int KEY_EQUAL = GLFW_KEY_EQUAL;
        static constexpr int KEY_A = GLFW_KEY_A;
        static constexpr int KEY_B = GLFW_KEY_B;
        static constexpr int KEY_C = GLFW_KEY_C;
        static constexpr int KEY_D = GLFW_KEY_D;
        static constexpr int KEY_E = GLFW_KEY_E;
        static constexpr int KEY_F = GLFW_KEY_F;
        static constexpr int KEY_G = GLFW_KEY_G;
        static constexpr int KEY_H = GLFW_KEY_H;
        static constexpr int KEY_I = GLFW_KEY_I;
        static constexpr int KEY_J = GLFW_KEY_J;
        static constexpr int KEY_K = GLFW_KEY_K;
        static constexpr int KEY_L = GLFW_KEY_L;
        static constexpr int KEY_M = GLFW_KEY_M;
        static constexpr int KEY_N = GLFW_KEY_N;
        static constexpr int KEY_O = GLFW_KEY_O;
        static constexpr int KEY_P = GLFW_KEY_P;
        static constexpr int KEY_Q = GLFW_KEY_Q;
        static constexpr int KEY_R = GLFW_KEY_R;
        static constexpr int KEY_S = GLFW_KEY_S;
        static constexpr int KEY_T = GLFW_KEY_T;
        static constexpr int KEY_U = GLFW_KEY_U;
        static constexpr int KEY_V = GLFW_KEY_V;
        static constexpr int KEY_W = GLFW_KEY_W;
        static constexpr int KEY_X = GLFW_KEY_X;
        static constexpr int KEY_Y = GLFW_KEY_Y;
        static constexpr int KEY_Z = GLFW_KEY_Z;
        static constexpr int KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET;
        static constexpr int KEY_BACKSLASH = GLFW_KEY_BACKSLASH;
        static constexpr int KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET;
        static constexpr int KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT;

        static constexpr int KEY_WORLD_1 = GLFW_KEY_WORLD_1;
        static constexpr int KEY_WORLD_2 = GLFW_KEY_WORLD_2;
        static constexpr int KEY_ESCAPE = GLFW_KEY_ESCAPE;
        static constexpr int KEY_ENTER = GLFW_KEY_ENTER;
        static constexpr int KEY_TAB = GLFW_KEY_TAB;
        static constexpr int KEY_BACKSPACE = GLFW_KEY_BACKSPACE;
        static constexpr int KEY_INSERT = GLFW_KEY_INSERT;
        static constexpr int KEY_DELETE = GLFW_KEY_DELETE;
        static constexpr int KEY_RIGHT = GLFW_KEY_RIGHT;
        static constexpr int KEY_LEFT = GLFW_KEY_LEFT;
        static constexpr int KEY_DOWN = GLFW_KEY_DOWN;
        static constexpr int KEY_UP = GLFW_KEY_UP;
        static constexpr int KEY_PAGE_UP = GLFW_KEY_PAGE_UP;
        static constexpr int KEY_PAGE_DOWN = GLFW_KEY_PAGE_DOWN;
        static constexpr int KEY_HOME = GLFW_KEY_HOME;
        static constexpr int KEY_END = GLFW_KEY_END;
        static constexpr int KEY_CAPS_LOCK = GLFW_KEY_CAPS_LOCK;
        static constexpr int KEY_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK;
        static constexpr int KEY_NUM_LOCK = GLFW_KEY_NUM_LOCK;
        static constexpr int KEY_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN;
        static constexpr int KEY_PAUSE = GLFW_KEY_PAUSE;
        static constexpr int KEY_F1 = GLFW_KEY_F1;
        static constexpr int KEY_F2 = GLFW_KEY_F2;
        static constexpr int KEY_F3 = GLFW_KEY_F3;
        static constexpr int KEY_F4 = GLFW_KEY_F4;
        static constexpr int KEY_F5 = GLFW_KEY_F5;
        static constexpr int KEY_F6 = GLFW_KEY_F6;
        static constexpr int KEY_F7 = GLFW_KEY_F7;
        static constexpr int KEY_F8 = GLFW_KEY_F8;
        static constexpr int KEY_F9 = GLFW_KEY_F9;
        static constexpr int KEY_F10 = GLFW_KEY_F10;
        static constexpr int KEY_F11 = GLFW_KEY_F11;
        static constexpr int KEY_F12 = GLFW_KEY_F12;
        static constexpr int KEY_F13 = GLFW_KEY_F13;
        static constexpr int KEY_F14 = GLFW_KEY_F14;
        static constexpr int KEY_F15 = GLFW_KEY_F15;
        static constexpr int KEY_F16 = GLFW_KEY_F16;
        static constexpr int KEY_F17 = GLFW_KEY_F17;
        static constexpr int KEY_F18 = GLFW_KEY_F18;
        static constexpr int KEY_F19 = GLFW_KEY_F19;
        static constexpr int KEY_F20 = GLFW_KEY_F20;
        static constexpr int KEY_F21 = GLFW_KEY_F21;
        static constexpr int KEY_F22 = GLFW_KEY_F22;
        static constexpr int KEY_F23 = GLFW_KEY_F23;
        static constexpr int KEY_F24 = GLFW_KEY_F24;
        static constexpr int KEY_F25 = GLFW_KEY_F25;
        static constexpr int KEY_KP_0 = GLFW_KEY_KP_0;
        static constexpr int KEY_KP_1 = GLFW_KEY_KP_1;
        static constexpr int KEY_KP_2 = GLFW_KEY_KP_2;
        static constexpr int KEY_KP_3 = GLFW_KEY_KP_3;
        static constexpr int KEY_KP_4 = GLFW_KEY_KP_4;
        static constexpr int KEY_KP_5 = GLFW_KEY_KP_5;
        static constexpr int KEY_KP_6 = GLFW_KEY_KP_6;
        static constexpr int KEY_KP_7 = GLFW_KEY_KP_7;
        static constexpr int KEY_KP_8 = GLFW_KEY_KP_8;
        static constexpr int KEY_KP_9 = GLFW_KEY_KP_9;
        static constexpr int KEY_KP_DECIMAL = GLFW_KEY_KP_DECIMAL;
        static constexpr int KEY_KP_DIVIDE = GLFW_KEY_KP_DIVIDE;
        static constexpr int KEY_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY;
        static constexpr int KEY_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT;
        static constexpr int KEY_KP_ADD = GLFW_KEY_KP_ADD;
        static constexpr int KEY_KP_ENTER = GLFW_KEY_KP_ENTER;
        static constexpr int KEY_KP_EQUAL = GLFW_KEY_KP_EQUAL;
        static constexpr int KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
        static constexpr int KEY_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL;
        static constexpr int KEY_LEFT_ALT = GLFW_KEY_LEFT_ALT;
        static constexpr int KEY_LEFT_SUPER = GLFW_KEY_LEFT_SUPER;
        static constexpr int KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
        static constexpr int KEY_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL;
        static constexpr int KEY_RIGHT_ALT = GLFW_KEY_RIGHT_ALT;
        static constexpr int KEY_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER;
        static constexpr int KEY_MENU = GLFW_KEY_MENU;

        //=============== ACTIONS ===============
        static constexpr int KEY_ACTION_RELEASE = GLFW_RELEASE;
        static constexpr int KEY_ACTION_PRESS = GLFW_PRESS;
        static constexpr int KEY_ACTION_REPEAT = GLFW_REPEAT;

        //=============== MODS ===============
        static constexpr int KEY_MOD_NONE = 0;
        static constexpr int KEY_MOD_SHIFT = GLFW_MOD_SHIFT;
        static constexpr int KEY_MOD_CONTROL = GLFW_MOD_CONTROL;
        static constexpr int KEY_MOD_ALT = GLFW_MOD_ALT;
        static constexpr int KEY_MOD_SUPER = GLFW_MOD_SUPER;
        static constexpr int KEY_MOD_CAPS_LOCK = GLFW_MOD_CAPS_LOCK;
        static constexpr int KEY_MOD_NUM_LOCK = GLFW_MOD_NUM_LOCK;
    };
}

namespace std
{
    template<>
    struct hash<vks_engine::Key>
    {
        size_t operator()(vks_engine::Key const &key) const noexcept
        {
            size_t h1 = hash<int>{}(key.m_Key);
            //size_t h2 = hash<int>{}(key.m_Scancode);
            size_t h3 = hash<int>{}(key.m_Action);
            size_t h4 = hash<int>{}(key.m_Mod);

            //return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
            return h1 ^ (h3 << 1) ^ (h4 << 2);
        }
    };
}

#endif //VKSENGINEKEY_H
