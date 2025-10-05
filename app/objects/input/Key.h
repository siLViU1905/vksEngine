#ifndef VKSENGINEKEY_H
#define VKSENGINEKEY_H
#include <xhash>

namespace vks_engine
{
    struct Key
    {
        int m_Key;

        //int m_Scancode;

        int m_Action;

        //int m_Mod;

        Key();

        //Key(int key, int scancode, int action, int mod);

        Key(int key, int action);

        bool operator==(const Key &key) const;
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
            // size_t h4 = hash<int>{}(key.m_Mod);

            //return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
            return h1 ^ (h3 << 1);
        }
    };
}

#endif //VKSENGINEKEY_H
