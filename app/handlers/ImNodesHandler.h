#ifndef VKSENGINEIMNODESHANDLER_H
#define VKSENGINEIMNODESHANDLER_H
#include "imnodes.h"

namespace vks_engine
{
    class ImNodesHandler
    {
    public:
        ImNodesHandler();

        void init();

        ~ImNodesHandler();

    private:
         ImNodesContext* m_Context;
    };
}

#endif //VKSENGINEIMNODESHANDLER_H
