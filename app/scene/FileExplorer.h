#ifndef VKSENGINEFILEEXPLORER_H
#define VKSENGINEFILEEXPLORER_H
#include <string>

namespace vks_engine
{
    class FileExplorer
    {
    public:
        FileExplorer();

        void open();

        constexpr const std::string &getPath() const { return m_Path; }

    private:
        std::string m_Path;
    };
}

#endif //VKSENGINEFILEEXPLORER_H
