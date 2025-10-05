#include "Key.h"

namespace vks_engine
{
    /*Key::Key(): m_Key(0), m_Scancode(0), m_Action(0), m_Mod(0)
    {
    }*/

    Key::Key(): m_Key(0), m_Action(0)
    {
    }

    /* Key::Key(int key, int scancode, int action, int mod): m_Key(key), m_Scancode(scancode), m_Action(action), m_Mod(mod)
     {}*/

    Key::Key(int key, int action): m_Key(key), m_Action(action)
    {
    }

    /*bool Key::operator==(const Key &key) const
    {
        return m_Key == key.m_Key && m_Scancode == key.m_Scancode && m_Action == key.m_Action && m_Mod == key.m_Mod;
    }*/

    bool Key::operator==(const Key &key) const
    {
        return m_Key == key.m_Key && m_Action == key.m_Action;
    }
}
