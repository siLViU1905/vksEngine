#ifndef VKSENGINE_IMNODESHANDLER_H
#define VKSENGINE_IMNODESHANDLER_H
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

#endif //VKSENGINE_IMNODESHANDLER_H
