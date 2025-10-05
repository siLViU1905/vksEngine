#ifndef VKSENGINEBUTTON_H
#define VKSENGINEBUTTON_H
#include <xhash>

namespace vks_engine
{
    struct Button
    {
        int m_Button;

        int m_Action;

        Button();

        Button(int button, int action);

        bool operator==(const Button &button) const;
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

            return h1 ^ (h2 << 1);
        }
    };
}

#endif //VKSENGINEBUTTON_H
