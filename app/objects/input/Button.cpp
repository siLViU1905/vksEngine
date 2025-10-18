#include "Button.h"

namespace vks_engine
{
    Button::Button(): m_Button(0), m_Action(0), m_Mod(0)
    {
    }

    Button::Button(int button, int action, int mod): m_Button(button), m_Action(action), m_Mod(mod)
    {
    }

    bool Button::operator==(const Button &button) const
    {
        return m_Button == button.m_Button && m_Action == button.m_Action && m_Mod == button.m_Mod;
    }
}
