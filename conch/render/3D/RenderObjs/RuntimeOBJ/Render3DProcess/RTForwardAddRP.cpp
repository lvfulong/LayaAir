#include "RTForwardAddRP.h"

namespace laya {

    RTForwardAddRP::RTForwardAddRP() {

    }

    RTForwardAddRP::~RTForwardAddRP() {

    }

    void RTForwardAddRP::clearBeforeImageEffectCmds()
    {
        _beforeImageEffectCMDS.clear();
    }

    void RTForwardAddRP::addBeforeImageEffectCmds(const std::vector<GLESRenderCMD*>& cmds)
    {
        _beforeImageEffectCMDS.push_back(cmds);
    }

    void RTForwardAddRP::clearAfterAllRenderCmds()
    {
        _afterAllRenderCMDS.clear();
    }

    void RTForwardAddRP::addAfterAllRenderCmds(const std::vector<GLESRenderCMD*>& cmds)
    {
        _afterAllRenderCMDS.push_back(cmds);
    }
}