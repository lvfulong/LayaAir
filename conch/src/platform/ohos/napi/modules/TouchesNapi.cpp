#include "TouchesNapi.h"


#include <utils/InputTypes.h>
#include "JCConch.h"
using namespace laya;
extern "C"
{
    void nativeHandleTouchDown(int id, int x, int y)
    {
        inputEvent e;
        e.nType = E_ONTOUCHSTART;
        e.nTouchType = 0;
        e.posX = x;
        e.posY = y;
        e.id = id;
        strncpy(e.type, "touchstart", 256);
        JCConch::s_pConch->dispatchInputEvent(e);
    }

    void nativeHandleTouchUp(int id, int x, int y)
    {
        inputEvent e;
        e.nType = E_ONTOUCHEND;
        e.nTouchType = 6;
        e.posX = x;
        e.posY = y;
        e.id = id;
        strncpy(e.type, "touchend", 256);
        JCConch::s_pConch->dispatchInputEvent(e);
    }

    void nativeHandleTouchMove(int id, int x, int y)
    {
        inputEvent e;
        e.nType = E_ONTOUCHMOVE;
        e.nTouchType = 2;
        e.posX = x;
        e.posY = y;
        e.id = id;
        strncpy(e.type, "touchmove", 256);
        JCConch::s_pConch->dispatchInputEvent(e);
    }

    void nativeHandleTouchCancel(int id, int x, int y)
    {
        inputEvent e;
        e.nType = E_ONTOUCHEND;
        e.nTouchType = 3;
        e.posX = x;
        e.posY = y;
        e.id = id;
        strncpy(e.type, "touchCancel", 256);
        JCConch::s_pConch->dispatchInputEvent(e);
    }
}
