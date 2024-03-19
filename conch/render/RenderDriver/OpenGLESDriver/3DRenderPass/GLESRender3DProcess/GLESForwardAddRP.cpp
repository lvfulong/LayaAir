#include "GLESForwardAddRP.h"

namespace laya {

    GLESForwardAddRP::GLESForwardAddRP() {

    }

    GLESForwardAddRP::~GLESForwardAddRP() {

    }

    void GLESForwardAddRP::clearBeforeImageEffectCmds()
    {
        _beforeImageEffectCMDS.clear();
    }

    void GLESForwardAddRP::addBeforeImageEffectCmds(const std::vector<GLESRenderCMD*>& cmds)
    {
        _beforeImageEffectCMDS.push_back(cmds);
    }

    void GLESForwardAddRP::clearAfterAllRenderCmds()
    {
        _afterAllRenderCMDS.clear();
    }

    void GLESForwardAddRP::addAfterAllRenderCmds(const std::vector<GLESRenderCMD*>& cmds)
    {
        _afterAllRenderCMDS.push_back(cmds);
    }

    void GLESForwardAddRP::setPostProcess(const std::vector<GLESRenderCMD*>& cmds) {
        postProcessCMDS = cmds;
    }
}