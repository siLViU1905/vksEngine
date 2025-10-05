#include "Button.h"

namespace vks_engine
{
    Button::Button():m_Button(0), m_Action(0)
    {
    }

    Button::Button(int button, int action):m_Button(button), m_Action(action)
    {
    }

    bool Button::operator==(const Button &button) const
    {
        return m_Button == button.m_Button && m_Action == button.m_Action;
    }
}
