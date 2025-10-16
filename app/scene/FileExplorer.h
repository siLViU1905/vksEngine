#ifndef VKSENGINEFILEEXPLORER_H
#define VKSENGINEFILEEXPLORER_H
#include <functional>
#include <string>

namespace vks_engine
{
    class FileExplorer
    {
    public:
        FileExplorer();

        void onFileSelected(std::function<void(const std::string&)> function);

        void open();

    private:
        std::function<void(const std::string&)> m_FileSelectedCallback;
    };
}

#endif //VKSENGINEFILEEXPLORER_H
