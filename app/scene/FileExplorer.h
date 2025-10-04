#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H
#include <string>


class FileExplorer
{
public:
    FileExplorer();

    void open();

    constexpr const std::string& getPath() const {return m_Path;}

private:
    std::string m_Path;
};


#endif //FILEEXPLORER_H
